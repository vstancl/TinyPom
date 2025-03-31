/************************************************************************/
/* (c) 2025 Vit Stancl <stancl.vit on gmail>								*/
/************************************************************************/

#include "HotkeyPushButton.h"

#include <QDebug>
#include <QKeyEvent>
#include <QApplication>


HotkeyPushButton::HotkeyPushButton(QWidget* parent)
	: QPushButton("Set Hotkey", parent), m_recording(false)
	, m_emptyText("Set Hotkey")
{

	setCheckable(true);
	connect(this, &QPushButton::clicked, this, &HotkeyPushButton::startRecording);
}

QKeySequence HotkeyPushButton::keySequence() const
{
	if (!isValid()) {
		return QKeySequence();
	}

	// Convert to QKeySequence for easy usage with QHotkey
	return QKeySequence(m_keyModifiers | m_currentKeys.last());
}

void HotkeyPushButton::setEmptyText(const QString& text)
{
	m_emptyText = text;
}

void HotkeyPushButton::setPrefixText(const QString& text)
{
	m_hotkeyPrefix = text;
}

bool HotkeyPushButton::isValid() const
{
	// Hotkey is valid if it has at least one modifier and one key
	return m_keyModifiers != Qt::NoModifier && !m_currentKeys.isEmpty();
}

void HotkeyPushButton::setKeySequence(const QKeySequence& keys)
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

	setText(m_hotkeyPrefix + keySequenceToString(m_keyModifiers, key));
}

void HotkeyPushButton::startRecording() {
	if (!m_recording) {
		setText(m_emptyText);
		setChecked(true);
		m_recording = true;
	}
}

void HotkeyPushButton::stopRecording()
{
	if (isValid())
	{
		// Update the display
		setText(m_hotkeyPrefix + keySequenceToString(m_keyModifiers, m_currentKeys.last()));
	}
	else
		setText(m_emptyText);

	setChecked(false);
	m_recording = false;
}

void HotkeyPushButton::resendKey(Qt::Key key, Qt::KeyboardModifiers modifiers)
{
	QWidget* receiver = QApplication::focusWidget(); // Get the current focused widget
	if (!receiver) return; // Ensure there's a valid receiver

	// Create and post a new key event
	QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, key, modifiers);
	QApplication::postEvent(receiver, event);
}

void HotkeyPushButton::resendKey()
{
	if (isValid())
		resendKey(Qt::Key(m_currentKeys.front()), m_keyModifiers);
}

void HotkeyPushButton::keyPressEvent(QKeyEvent* event) {
	if (!m_recording) {
		QPushButton::keyPressEvent(event);
		return;
	}

	int key = event->key();
	if (key == Qt::Key_Escape) 
	{  
		// Emit the change signal with current settigns...
		if(isValid())
			emit hotkeyChanged(m_currentKeys, m_keyModifiers, this);

		stopRecording();
	}
	else 
	{
		// Get the pressed key
		Qt::KeyboardModifiers modifiers = event->modifiers();

		qDebug() << key;

		// Ignore modifier keys when pressed alone
		if (key == Qt::Key_Control || key == Qt::Key_Shift ||
			key == Qt::Key_Alt || key == Qt::Key_Meta) {
			QPushButton::keyPressEvent(event);
			return;
		}

		// Store the key and modifiers
		m_currentKeys.clear();
		m_currentKeys.append(key);
		m_keyModifiers = modifiers;

		

		// Emit the change signal
		emit hotkeyChanged(m_currentKeys, modifiers, this);

		// Prevent default handling
		event->accept();

		stopRecording();
	}
}

void HotkeyPushButton::focusOutEvent(QFocusEvent* event) {
	if (m_recording) {
		stopRecording();
	}

	QPushButton::focusOutEvent(event);
}

QString HotkeyPushButton::keySequenceToString(Qt::KeyboardModifiers modifiers, int key)
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

void HotkeyPushButton::toggleRecording()
{
	if (m_recording) {
		stopRecording();  // Clicking again stops recording
	}
	else {
		startRecording();
	}
}
