#pragma once

#include <QDialog>
#include <QSystemTrayIcon>
#include <QHotkey>
#include <QSettings>
#include "SettingsMaintainer.h"

class PausableTimer;
class QMediaPlayer;
class QAudioOutput;
class HotkeyLineEdit;

namespace Ui {
	class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public: 
	SettingsDialog();
	~SettingsDialog();

	void setVisible(bool visible) override;

protected:
	void closeEvent(QCloseEvent* event) override;

protected slots:
	void onResetTimer();
	void onPauseTimer();
	void onShowSettings();
	void onTimeout();
	void onTick(int elapsedTime, int remainingTime);
	void on_pushButtonHide_clicked();
	void on_pushButtonStartTimer_clicked();
	void on_pushButtonPauseTimer_clicked();
	void on_pushButtonExit_clicked();
	
	void on_pushButtonEditShowWindowHotkey_clicked();
	void on_pushButtonEditResetTimerHotkey_clicked();
	void on_pushButtonEditPauseTimerHotkey_clicked();

	void on_spinBoxTimer_valueChanged(int value);

	void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
protected:

	void on_hotkeySet(const QList<int>& keys, Qt::KeyboardModifiers modifiers, HotkeyLineEdit *sender);

	void initializeTrayIcon();

	void createActions();
	void connectUI();
	void setIcon();
	void playSound();
	void disableHotkeysScanning();

	void registerShowWindowHotkey();
	void registerResetTimerHotkey();
	void registerPauseTimerHotkey();


	QSharedPointer<QHotkey> registerHotKeyIfPresent(const QKeySequence &keySequence, std::function<void()> callbackFunction);

private:
	Ui::SettingsDialog* ui;

	QAction* resetTimer;
	QAction* pauseTimer;
	QAction* showSettings;
	QAction* closeApp;

	QSystemTrayIcon* trayIcon;

	PausableTimer* timer;
	QMediaPlayer* player;
	QAudioOutput * audioOutput;

	SettingsMaintainer m_settings;

	QSharedPointer<QHotkey> m_showWindowHotkey, m_resetTimerHotkey, m_pauseTimerHotkey;
};