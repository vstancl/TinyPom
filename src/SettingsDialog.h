#pragma once

#include <QDialog>
#include <QSystemTrayIcon>

class PausableTimer;
class QMediaPlayer;
class QAudioOutput;

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
	void on_pushButtonHide_clicked();
	void on_pushButtonStartTimer_clicked();
	void on_pushButtonPauseTimer_clicked();
	void on_pushButtonExit_clicked();

	void on_spinBoxTimer_valueChanged(int value);

	void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
protected:

	void initializeTrayIcon();

	void createActions();
	void connectUI();
	void setIcon();
	void playSound();
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
};