#pragma once

#include <QWidget>
#include <QHotkey>
#include "SettingsMaintainer.h"

namespace Ui {
	class SettingsWidget;
}

class HotkeyPushButton;
class MainDialog;

class SettingsWidget : public QWidget
{
	Q_OBJECT

public:
	SettingsWidget(QDialog *parent);
	~SettingsWidget();

protected slots:
	void on_checkBoxPinOnTop_checkStateChanged(Qt::CheckState state);
	void on_checkBoxStartWithAppVisible_checkStateChanged(Qt::CheckState state);
	void on_checkBoxShowWindowTimerEnd_checkStateChanged(Qt::CheckState state);

	void on_spinBoxTimer_valueChanged(int value);

protected:
	void on_hotkeySet(const QList<int>& keys, Qt::KeyboardModifiers modifiers, HotkeyPushButton* sender);

	void registerShowWindowHotkey();

	void unregisterShowWindowHotkey();

	void registerResetTimerHotkey();

	void unregisterResetTimerHotkey();

	void registerPauseTimerHotkey();

	void unregisterPauseTimerHotkey();

	void setStyling();

	bool hotkeysEnabled() const;

	QSharedPointer<QHotkey> registerHotKeyIfPresent(const QKeySequence& keySequence, std::function<void()> callbackFunction);

private:
	SettingsMaintainer m_settings;
	Ui::SettingsWidget *ui;
	MainDialog* mainDialog;
	QSharedPointer<QHotkey> m_showWindowHotkey, m_resetTimerHotkey, m_pauseTimerHotkey;
};

