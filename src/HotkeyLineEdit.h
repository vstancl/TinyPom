#pragma once

// #include <QApplication>
// #include <QWidget>
// #include <QVBoxLayout>
 #include <QLineEdit>
// #include <QLabel>
 #include <QKeyEvent>
// #include <QHotkey>

class HotkeyLineEdit : public QLineEdit 
{
	Q_OBJECT
public:
	HotkeyLineEdit(QWidget* parent = nullptr);

	void setScanning(bool scanning);

	bool isValid() const;

	void setKeySequence(const QKeySequence& keys);

	QKeySequence keySequence() const;

signals:
	void hotkeyChanged(const QList<int>& keys, Qt::KeyboardModifiers modifiers);

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
	QString keySequenceToString(Qt::KeyboardModifiers modifiers, int key);

	QList<int> m_currentKeys;
	Qt::KeyboardModifiers m_keyModifiers;
	const QString m_emptyPlaceholderText = QString(tr("Press hotkey combination..."));
	QString m_currentPlaceholderText;
	bool m_doScanning;
};


