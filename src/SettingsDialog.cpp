#include "SettingsDialog.h"
#include "ui_settingsdialog.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include "PausableTimer.h"
#include <QStandardPaths>
#include <QFile>

#include <QMediaFormat>
#include <QDebug>
#include <QDirIterator>

#include "SettingsMaintainer.h";


SettingsDialog::SettingsDialog()
	: ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	player = new QMediaPlayer(this);
 	audioOutput = new QAudioOutput();
 	player->setAudioOutput(audioOutput);

	connect(player, &QMediaPlayer::mediaStatusChanged,
		[=](QMediaPlayer::MediaStatus status) -> void { qDebug() << "QMediaPlayer::mediaStatusChanged" << status; });

	createActions();
	initializeTrayIcon();

	timer = new PausableTimer(1000, this);
	connect(timer, &PausableTimer::timeout, this, &SettingsDialog::onTimeout);
	
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::setVisible(bool visible)
{
	if (visible)
	{
		showSettings->setText(tr("Hide Settings"));

		// Fill dialog from settings
		SettingsMaintainer settings;
		ui->spinBoxTimer->setValue(settings.getTimerDurationMin());

	}
	else
	{
		showSettings->setText(tr("Show Settings"));
	}

	QDialog::setVisible(visible);
}

void SettingsDialog::closeEvent(QCloseEvent* event)
{
	if (!event->spontaneous() || !isVisible())
		return;

	if (trayIcon->isVisible()) {
// #TODO: On first time run show this message (and store to QSettings that it was shown
// 		QMessageBox::information(this, tr("Systray"),
// 			tr("The program will keep running in the "
// 				"system tray. To terminate the program, "
// 				"choose <b>Quit</b> in the context menu "
// 				"of the system tray entry."));
		hide();
		event->ignore();
	}
}

void SettingsDialog::onResetTimer()
{
	SettingsMaintainer settings;
	timer->setDurationMinutes(settings.getTimerDurationMin());
	timer->start();
	pauseTimer->setEnabled(true);
}

void SettingsDialog::onPauseTimer()
{
	if (timer->isPaused())
	{
		ui->pushButtonPauseTimer->setText(tr("Pause"));
		timer->resume();
	}
	else
	{
		ui->pushButtonPauseTimer->setText(tr("Resume"));
		timer->pause();
	}
}

void SettingsDialog::onShowSettings()
{
	if (this->isVisible())
		hide();
	else
		show();
}

void SettingsDialog::onTimeout()
{
	pauseTimer->setEnabled(false);
	playSound();
}

void SettingsDialog::on_pushButtonHide_clicked()
{
	onShowSettings();
}

void SettingsDialog::on_pushButtonStartTimer_clicked()
{
	onResetTimer();
}

void SettingsDialog::on_pushButtonPauseTimer_clicked()
{
	onPauseTimer();
}

void SettingsDialog::on_spinBoxTimer_valueChanged(int value)
{
	SettingsMaintainer settings;
	settings.setTimerDurationMin(value);
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
	resetTimer = new QAction(tr("Start Timer"), this);
	connect(resetTimer, &QAction::triggered, this, &SettingsDialog::onResetTimer);

	pauseTimer = new QAction(tr("Pause Timer"), this);
	pauseTimer->setEnabled(false);
	connect(pauseTimer, &QAction::triggered, this, &SettingsDialog::onPauseTimer);

	showSettings = new QAction(tr("Show Window"), this);
	connect(showSettings, &QAction::triggered, this, &SettingsDialog::onShowSettings);

	closeApp = new QAction(tr("Exit"), this);
	connect(closeApp, &QAction::triggered, qApp, &QCoreApplication::quit);

	connect(player, &QMediaPlayer::mediaStatusChanged, [](QMediaPlayer::MediaStatus status) {
		qDebug() << "Media status changed:" << status;
		});

	connect(player, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString& errorString) {
		qDebug() << "QMediaPlayer Error:" << error << errorString;
		});

	connect(player, &QMediaPlayer::playbackStateChanged, this, [](QMediaPlayer::PlaybackState state) {
		qDebug() << "Playback State Changed:" << state;
		});

	connect(player, &QMediaPlayer::positionChanged, this, [](qint64 position) {
		qDebug() << "Playback Position:" << position;
		});

// 	connect(player, &QMediaPlayer::errorOccurred, [](QMediaPlayer::Error error, const QString& errorString) {
// 		qDebug() << "Error:" << error << errorString;
// 		});

}

void SettingsDialog::connectUI()
{
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

void SettingsDialog::playSound()
{
	auto source = QUrl::fromLocalFile("C:/1 Personal/1 Projects/SmallApps/TinyPomodoro/res/mp3/campana-40773.mp3");
//	auto source = QUrl("qrc:///mp3/mp3/campana-40773.mp3");
	if (!source.isValid())
		return;
	
	player->setSource(source);
 	audioOutput->setVolume(100);
 	player->play();
}

