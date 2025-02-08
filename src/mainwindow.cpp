#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSystemTrayIcon>
#include <QMenu>

SettingsDialog::SettingsDialog()
	: ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	createActions();
	initializeTrayIcon();
	
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::onResetTimer()
{

}

void SettingsDialog::onPauseTimer()
{

}

void SettingsDialog::onShowSettings()
{

}

void SettingsDialog::initializeTrayIcon()
{
	auto menu = new QMenu(this);
	menu->addAction(resetTimer);
	menu->addAction(pauseTimer);
	menu->addAction(showSettings);
	menu->addAction(closeApp);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(menu);

	setIcon();
	trayIcon->show();
}

void SettingsDialog::createActions()
{
	resetTimer = new QAction(tr("Reset Timer"), this);
	connect(resetTimer, &QAction::triggered, this, &SettingsDialog::onResetTimer);

	pauseTimer = new QAction(tr("Pause Timer"), this);
	connect(pauseTimer, &QAction::triggered, this, &SettingsDialog::onPauseTimer);

	showSettings = new QAction(tr("Settings"), this);
	connect(showSettings, &QAction::triggered, this, &SettingsDialog::onShowSettings);

	closeApp = new QAction(tr("Exit"), this);
	connect(closeApp, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void SettingsDialog::setIcon()
{
	auto icon = QIcon(":png/images/smallPomodoro.png");
	
	if(!icon.isNull())
	{
		trayIcon->setIcon(icon);
		setWindowIcon(icon);
	}
}

