#pragma once

#include <QPushButton>

class HotkeyPushButton : public QPushButton {
	Q_OBJECT

public:
	explicit HotkeyPushButton(QWidget* parent = nullptr);

	QKeySequence keySequence() const;

	void setEmptyText(const QString& text);

	void setPrefixText(const QString& text);

	bool isValid() const;

	void setKeySequence(const QKeySequence& keys);

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

private:
	bool m_recording;
	
	QList<int> m_currentKeys;
	Qt::KeyboardModifiers m_keyModifiers;

	QString m_emptyText;
	QString m_hotkeyPrefix;
};
