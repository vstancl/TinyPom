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

	void disableHotkeysScanning();

protected slots:
	void on_pushButtonEditShowWindowHotkey_clicked();
	void on_pushButtonEditResetTimerHotkey_clicked();
	void on_pushButtonEditPauseTimerHotkey_clicked();

	void on_checkBoxPinOnTop_checkStateChanged(Qt::CheckState state);
	void on_checkBoxStartWithAppVisible_checkStateChanged(Qt::CheckState state);
	void on_checkBoxShowWindowTimerEnd_checkStateChanged(Qt::CheckState state);

	void on_spinBoxTimer_valueChanged(int value);

protected:
	void on_hotkeySet(const QList<int>& keys, Qt::KeyboardModifiers modifiers, HotkeyPushButton* sender);

	void registerShowWindowHotkey();
	void registerResetTimerHotkey();
	void registerPauseTimerHotkey();

	void setStyling();

	QSharedPointer<QHotkey> registerHotKeyIfPresent(const QKeySequence& keySequence, std::function<void()> callbackFunction);

private:
	SettingsMaintainer m_settings;
	Ui::SettingsWidget *ui;
	MainDialog* mainDialog;
	QSharedPointer<QHotkey> m_showWindowHotkey, m_resetTimerHotkey, m_pauseTimerHotkey;
};

