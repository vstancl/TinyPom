/************************************************************************/
/*                                                                      */
/************************************************************************/

#include "MainDialog.h"
#include "ui_MainDialog.h"

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


#include "SettingsMaintainer.h"
#include "SettingsWidget.h"
#include "Styling.h"

MainDialog::MainDialog()
	: ui(new Ui::MainDialog)
{
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	ui->setupUi(this);
	ui->horizontalLayout->setSpacing(0);
	ui->horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
	
	setStyling();

	player = new QMediaPlayer(this);
 	audioOutput = new QAudioOutput();
 	player->setAudioOutput(audioOutput);

	connect(player, &QMediaPlayer::mediaStatusChanged,
		[=](QMediaPlayer::MediaStatus status) -> void { qDebug() << "QMediaPlayer::mediaStatusChanged" << status; });

	createActions();
	initializeTrayIcon();

	timer = new PausableTimer(1000, this);
	connect(timer, &PausableTimer::timeout, this, &MainDialog::onTimeout);
	connect(timer, &PausableTimer::tick, this, &MainDialog::onTick);

	showTime(false);

	if (m_settings.getStayOnTop())
	{
		Qt::WindowFlags flags = windowFlags();
		setWindowFlags(flags | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);  // Add the flag
	}

	// Expandable right panel
	ui->settingsWidget->setFixedWidth(0);  // Initially collapsed

	qDebug() << "Button Effective StyleSheet: " << ui->pushButtonStartTimer->styleSheet();

	// Animation
	animation = new QPropertyAnimation(ui->settingsWidget, "maximumWidth");
	animation->setDuration(300);  // Animation time in ms
	animation->setEasingCurve(QEasingCurve::InOutQuad);

	connect(ui->pushButtonShowSettings, &QPushButton::clicked, this, &MainDialog::togglePanel);

	updatePauseButtonState();

	if (m_settings.getStartVisible())
		show();
}

MainDialog::~MainDialog()
{
	delete ui;
}

void MainDialog::setVisible(bool visible)
{
	if (visible)
	{
		showSettingsAction->setText(tr("Hide Settings"));

	}
	else
	{
		showSettingsAction->setText(tr("Show Settings"));
	}

	QDialog::setVisible(visible);
}

void MainDialog::closeEvent(QCloseEvent* event)
{
	if (!event->spontaneous() || !isVisible())
		return;

	if (trayIcon->isVisible()) {
		hide();
		event->ignore();
	}
}

void MainDialog::onResetTimer()
{
	SettingsMaintainer settings;
	timer->setDurationMinutes(settings.getTimerDurationMin());
	timer->start();
	pauseTimerAction->setEnabled(true);

	updatePauseButtonState();

	// Update time label text and show it
	auto timeString = PausableTimer::formatTime(60 * 1000 * settings.getTimerDurationMin());
	ui->labelCurrentTime->setText(timeString);

	showTime(true);
}

void MainDialog::onPauseTimer()
{
	if (!timer->isStarted())
		return;

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

void MainDialog::onStopTimer()
{
	timer->stop();

	updatePauseButtonState();

	showTime(false);
	pauseTimerAction->setEnabled(false);
	trayIcon->setToolTip("");
}

void MainDialog::onShowSettings()
{
	if (this->isVisible())
		hide();
	else
		show();
}

void MainDialog::onTimeout()
{
	updatePauseButtonState();

	if(m_settings.getShowOnTimerEnd() && !isVisible())
		show();

	showTime(false);

	pauseTimerAction->setEnabled(false);
	trayIcon->setToolTip("");
	playSound();
}

void MainDialog::onTick(int elapsedTime, int remainingTime)
{
	auto timeString = PausableTimer::formatTime(remainingTime);
	trayIcon->setToolTip(timeString);
	ui->labelCurrentTime->setText(timeString);
}

void MainDialog::on_pushButtonHide_clicked()
{
	onShowSettings();
}

void MainDialog::on_pushButtonStartTimer_clicked()
{
	onResetTimer();
}

void MainDialog::on_pushButtonPauseTimer_clicked()
{
	onPauseTimer();
}

void MainDialog::on_pushButtonStopTimer_clicked()
{
	onStopTimer();
}

void MainDialog::on_pushButtonExit_clicked()
{
	QCoreApplication::quit();
}

void MainDialog::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick && !this->isVisible()) {
		onShowSettings();
	}
}

void MainDialog::togglePanel()
{
	int currentWidth = ui->settingsWidget->maximumWidth();
	int targetWidth = (currentWidth == 0) ? 354 : 0;  // Expand to 200px or collapse

	animation->setStartValue(currentWidth);
	animation->setEndValue(targetWidth);
	animation->start();

	this->adjustSize();
}

void MainDialog::setStyling()
{
	Styling::setWidgetStyling(this);

	Styling::setButtonStyling(ui->pushButtonStartTimer);
	Styling::setButtonStyling(ui->pushButtonPauseTimer);
	Styling::setButtonStyling(ui->pushButtonStopTimer);
	Styling::setButtonStyling(ui->pushButtonHide);
	Styling::setButtonStyling(ui->pushButtonExit);
	Styling::setButtonStyling(ui->pushButtonShowSettings);
}

void MainDialog::updatePauseButtonState()
{
	ui->pushButtonPauseTimer->setEnabled(timer->isStarted());
}

void MainDialog::initializeTrayIcon()
{
	auto menu = new QMenu(this);
	menu->addAction(resetTimerAction);
	menu->addAction(pauseTimerAction);
	menu->addAction(showSettingsAction);
	menu->addAction(closeAppAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(menu);

	// Connect the activated signal
	connect(trayIcon, &QSystemTrayIcon::activated, this, &MainDialog::onTrayActivated);

	setIcon();
	trayIcon->show();
}

void MainDialog::createActions()
{
	resetTimerAction = new QAction(tr("Start Timer"), this);
	connect(resetTimerAction, &QAction::triggered, this, &MainDialog::onResetTimer);

	pauseTimerAction = new QAction(tr("Pause Timer"), this);
	pauseTimerAction->setEnabled(false);
	connect(pauseTimerAction, &QAction::triggered, this, &MainDialog::onPauseTimer);

	showSettingsAction = new QAction(tr("Show Window"), this);
	connect(showSettingsAction, &QAction::triggered, this, &MainDialog::onShowSettings);

	closeAppAction = new QAction(tr("Exit"), this);
	connect(closeAppAction, &QAction::triggered, qApp, &QCoreApplication::quit);

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

void MainDialog::connectUI()
{
}

void MainDialog::setIcon()
{
	auto icon = QIcon(":png/images/smallPomodoro.png");
	
	if(!icon.isNull())
	{
		trayIcon->setIcon(icon);
		setWindowIcon(icon);
	}
}

void MainDialog::playSound()
{
	auto source = QUrl("qrc:/mp3/mp3/campana-40773.mp3");
	if (!source.isValid())
		return;

	player->setSource(source);
 	audioOutput->setVolume(100);
 	player->play();
}

void MainDialog::showTime(bool show)
{
	ui->labelCurrentTime->setVisible(show);
}

