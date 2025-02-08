#include <QDialog>

class QSystemTrayIcon;

namespace Ui {
	class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public: 
	SettingsDialog();
	~SettingsDialog();

protected slots:
	void onResetTimer();
	void onPauseTimer();
	void onShowSettings();

protected:

	void initializeTrayIcon();

	void createActions();

	void setIcon();
private:
	Ui::SettingsDialog* ui;

	QAction* resetTimer;
	QAction* pauseTimer;
	QAction* showSettings;
	QAction* closeApp;

	QSystemTrayIcon* trayIcon;
};