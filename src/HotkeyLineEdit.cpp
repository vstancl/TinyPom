#include "HotkeyLineEdit.h"

HotkeyLineEdit::HotkeyLineEdit(QWidget* parent /*= nullptr*/) 
	: QLineEdit(parent)
	, m_doScanning(false)
{
	setReadOnly(true);
	setPlaceholderText(m_emptyPlaceholderText);
	setClearButtonEnabled(true);

	// Connect clear button to reset the hotkey
	connect(this, &QLineEdit::textChanged, this, [this](const QString& text) {
		if (!m_doScanning)
		{
			setPlaceholderText(m_emptyPlaceholderText);
			return;
		}

		if (text.isEmpty()) {
			m_currentKeys.clear();
			m_keyModifiers = Qt::NoModifier;
			emit hotkeyChanged(m_currentKeys, m_keyModifiers);
		}
		});
}

void HotkeyLineEdit::setScanning(bool scanning)
{
	m_doScanning = scanning;
}

bool HotkeyLineEdit::isValid() const
{
	// Hotkey is valid if it has at least one modifier and one key
	return m_keyModifiers != Qt::NoModifier && !m_currentKeys.isEmpty();
}

void HotkeyLineEdit::setKeySequence(const QKeySequence& keys)
{
	if (keys.isEmpty())
		return;

	// Extract the integer key code from the sequence
	int keyInt = keys[0];

	// Extract modifiers - mask with Qt::KeyboardModifierMask
	m_keyModifiers = static_cast<Qt::KeyboardModifiers>(keyInt & Qt::KeyboardModifierMask);

	// Extract the actual key - remove modifiers with ~Qt::KeyboardModifierMask
	int key = keyInt & ~Qt::KeyboardModifierMask;
	m_currentKeys.clear();
	m_currentKeys.append(key);

	setText(keySequenceToString(m_keyModifiers, key));
}

QKeySequence HotkeyLineEdit::keySequence() const
{
	if (!isValid()) {
		return QKeySequence();
	}

	// Convert to QKeySequence for easy usage with QHotkey
	return QKeySequence(m_keyModifiers | m_currentKeys.last());
}

void HotkeyLineEdit::keyPressEvent(QKeyEvent* event)
{
	if (!m_doScanning)
		return;

	// Get the pressed key
	int key = event->key();
	Qt::KeyboardModifiers modifiers = event->modifiers();

	// Ignore modifier keys when pressed alone
	if (key == Qt::Key_Control || key == Qt::Key_Shift ||
		key == Qt::Key_Alt || key == Qt::Key_Meta) {
		return;
	}

	// Clear previous keys if ESC is pressed
	if (key == Qt::Key_Escape) {
		clear();
		return;
	}

	// Store the key and modifiers
	m_currentKeys.clear();
	m_currentKeys.append(key);
	m_keyModifiers = modifiers;

	// Update the display
	setText(keySequenceToString(modifiers, key));

	// Emit the change signal
	emit hotkeyChanged(m_currentKeys, modifiers);

	// Prevent default handling
	event->accept();
}

QString HotkeyLineEdit::keySequenceToString(Qt::KeyboardModifiers modifiers, int key)
{
	QString result;

	// Add modifiers
	if (modifiers & Qt::ControlModifier)
		result += "Ctrl+";
	if (modifiers & Qt::ShiftModifier)
		result += "Shift+";
	if (modifiers & Qt::AltModifier)
		result += "Alt+";
	if (modifiers & Qt::MetaModifier)
		result += "Meta+";

	// Add the key
	result += QKeySequence(key).toString();

	return result;
}
