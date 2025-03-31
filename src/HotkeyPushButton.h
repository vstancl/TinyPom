/************************************************************************/
/* (c) 2025 Vit Stancl <stancl.vit on gmail>								*/
/************************************************************************/

#pragma once

#include <QPushButton>

/** \brief	This checkable button is used for hotkey obtaining. After press (setting to the "ckecked" state it waits for some
			usable keys combination and than it emmits the hotkeyChanged signal. If Esc is pressed, the waiting state is changed 
			to the normal and the signal is emmited with the current keys combination (if exists). 
*/
class HotkeyPushButton : public QPushButton {
	Q_OBJECT

public:
	explicit HotkeyPushButton(QWidget* parent = nullptr);

	QKeySequence keySequence() const;

	//! Set text displayed when no keys combination is set yet.
	void setEmptyText(const QString& text);

	//! Set text displayed in front of the keys combination (The result is something like this: "Show Window: Ctrl-Alt-W").
	void setPrefixText(const QString& text);

	//! Is currently stored key combination valid?
	bool isValid() const;

	void setKeySequence(const QKeySequence& keys);

	//! If we are not in the state when the hotkeys are enabled but are pressed, we will postpone them back to the system.
	void resendKey();

	bool isRecording() const { return m_recording; }

signals:
	void hotkeyChanged(const QList<int>& keys, Qt::KeyboardModifiers modifiers, HotkeyPushButton* sender);

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;

	QString keySequenceToString(Qt::KeyboardModifiers modifiers, int key);

private slots:
	void toggleRecording();

private:
	void startRecording();
	void stopRecording();

	void resendKey(Qt::Key key, Qt::KeyboardModifiers modifiers);

private:
	bool m_recording;
	
	QList<int> m_currentKeys;
	Qt::KeyboardModifiers m_keyModifiers;

	QString m_emptyText;
	QString m_hotkeyPrefix;
};
