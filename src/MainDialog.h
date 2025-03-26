#pragma once

#include <QDialog>
#include <QSystemTrayIcon>
#include <QHotkey>
#include <QSettings>
#include "SettingsMaintainer.h"
#include <QPropertyAnimation>

class PausableTimer;
class QMediaPlayer;
class QAudioOutput;
class HotkeyLineEdit;

namespace Ui {
	class MainDialog;
}

class SettingsWidget;

class MainDialog : public QDialog
{
	Q_OBJECT

public: 
	MainDialog();
	~MainDialog();

	void setVisible(bool visible) override;

	void resetTimer() { onResetTimer(); }
	void pauseTimer() { onPauseTimer(); }

	void enableHotkeys(bool enable);

protected:
	void closeEvent(QCloseEvent* event) override;

protected slots:
	void onResetTimer();
	void onPauseTimer();
	void onStopTimer();
	void onShowSettings();
	void onTimeout();
	void onTick(int elapsedTime, int remainingTime);
	void on_pushButtonHide_clicked();
	void on_pushButtonStartTimer_clicked();
	void on_pushButtonPauseTimer_clicked();
	void on_pushButtonStopTimer_clicked();
	void on_pushButtonExit_clicked();

	void onTrayActivated(QSystemTrayIcon::ActivationReason reason);

	void togglePanel();
protected:

	void setStyling();

	void updatePauseButtonState();

	void initializeTrayIcon();

	void createActions();
	void connectUI();
	void setIcon();
	void playSound();

	void showTime(bool show);


private:
	Ui::MainDialog* ui;

	QAction* resetTimerAction;
	QAction* pauseTimerAction;
	QAction* showSettingsAction;
	QAction* closeAppAction;

	QSystemTrayIcon* trayIcon;
	PausableTimer* timer;
	QMediaPlayer* player;
	QAudioOutput * audioOutput;

	SettingsMaintainer m_settings;

	QPropertyAnimation* animation;

};