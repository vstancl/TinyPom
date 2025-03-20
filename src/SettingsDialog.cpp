#include "SettingsDialog.h"
#include "ui_settingsdialog.h"

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
	connect(timer, &PausableTimer::tick, this, &SettingsDialog::onTick);

	// Hotkeys scannig signals
	connect(ui->lineEditShowWindowHotkey, &HotkeyLineEdit::hotkeyChanged, this, &SettingsDialog::on_hotkeySet);
	connect(ui->lineEditResetTimerHotkey, &HotkeyLineEdit::hotkeyChanged, this, &SettingsDialog::on_hotkeySet);
	connect(ui->lineEditPauseTimerHotkey, &HotkeyLineEdit::hotkeyChanged, this, &SettingsDialog::on_hotkeySet);

	registerShowWindowHotkey();
	registerResetTimerHotkey();
	registerPauseTimerHotkey();

	// Fill hotkey edits
	ui->lineEditShowWindowHotkey->setKeySequence(m_settings.getShowWindowKeySequence());
	ui->lineEditResetTimerHotkey->setKeySequence(m_settings.getResetTimerKeySequence());
	ui->lineEditPauseTimerHotkey->setKeySequence(m_settings.getPauseTimerKeySequence());
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
		ui->spinBoxTimer->setValue(m_settings.getTimerDurationMin());

	}
	else
	{
		showSettings->setText(tr("Show Settings"));
		disableHotkeysScanning();
	}

	QDialog::setVisible(visible);
}

void SettingsDialog::closeEvent(QCloseEvent* event)
{
	if (!event->spontaneous() || !isVisible())
		return;

	if (trayIcon->isVisible()) {
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
	trayIcon->setToolTip("");
	playSound();
}

void SettingsDialog::onTick(int elapsedTime, int remainingTime)
{
	trayIcon->setToolTip(PausableTimer::formatTime(remainingTime));
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

void SettingsDialog::on_pushButtonExit_clicked()
{
	QCoreApplication::quit();
}

void SettingsDialog::on_pushButtonEditShowWindowHotkey_clicked()
{
	// Disable other windows scanning
	disableHotkeysScanning();
	/** \brief	Default constructor */
	ui->lineEditShowWindowHotkey->setFocus();
	ui->lineEditShowWindowHotkey->setScanning(true);
}

void SettingsDialog::on_pushButtonEditResetTimerHotkey_clicked()
{
	// Disable other windows scanning
	disableHotkeysScanning();
	/** \brief	Default constructor */
	ui->lineEditResetTimerHotkey->setFocus();
	ui->lineEditResetTimerHotkey->setScanning(true);
}

void SettingsDialog::on_pushButtonEditPauseTimerHotkey_clicked()
{
	// Disable other windows scanning
	disableHotkeysScanning();
	/** \brief	Default constructor */
	ui->lineEditPauseTimerHotkey->setFocus();
	ui->lineEditPauseTimerHotkey->setScanning(true);
}

void SettingsDialog::on_spinBoxTimer_valueChanged(int value)
{
	SettingsMaintainer settings;
	settings.setTimerDurationMin(value);
}

void SettingsDialog::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick && !this->isVisible()) {
		onShowSettings();
	}
}

void SettingsDialog::on_hotkeySet(const QList<int>& keys, Qt::KeyboardModifiers modifiers, HotkeyLineEdit* sender)
{
	QKeySequence keySeq(modifiers | keys.last());

	if(sender == ui->lineEditShowWindowHotkey)
	{
		m_settings.setShowWindowKeySequence(keySeq);
		registerShowWindowHotkey();

	}

	if (sender == ui->lineEditResetTimerHotkey)
	{
		m_settings.setResetTimerKeySequence(keySeq);
		registerResetTimerHotkey();
	}

	if (sender == ui->lineEditPauseTimerHotkey)
	{
		m_settings.setPauseTimerKeySequence(keySeq);
		registerPauseTimerHotkey();
	}
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

	// Connect the activated signal
	connect(trayIcon, &QSystemTrayIcon::activated, this, &SettingsDialog::onTrayActivated);

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
	auto source = QUrl::fromLocalFile("C:/1_Personal/1_Projects/SmallApps/TinyPomodoro/res/mp3/campana-40773.mp3");
//	auto source = QUrl("qrc:///mp3/mp3/campana-40773.mp3");
	if (!source.isValid())
		return;

	player->setSource(source);
 	audioOutput->setVolume(100);
 	player->play();
}

void SettingsDialog::disableHotkeysScanning()
{
	ui->lineEditShowWindowHotkey->setScanning(false);
}

void SettingsDialog::registerShowWindowHotkey()
{
	// Reading the key sequence
	m_showWindowHotkey = registerHotKeyIfPresent(m_settings.getShowWindowKeySequence(),
		[&]() 
		{

			if (this->isVisible())
			{
				this->hide();
			}
			else
			{
				this->showNormal();  // Show the window
				this->raise();
				this->activateWindow();
			}
		}

	);
}

void SettingsDialog::registerResetTimerHotkey()
{
	// Reading the key sequence
	m_resetTimerHotkey = registerHotKeyIfPresent(m_settings.getResetTimerKeySequence(),
		[&]()
		{
			this->onResetTimer();
		}
	);
}

void SettingsDialog::registerPauseTimerHotkey()
{
	// Reading the key sequence
	m_pauseTimerHotkey = registerHotKeyIfPresent(m_settings.getPauseTimerKeySequence(),
		[&]()
		{
			this->onPauseTimer();
		}
	);
}

QSharedPointer<QHotkey> SettingsDialog::registerHotKeyIfPresent(const QKeySequence& keySequence, std::function<void()> callbackFunction)
{
	// Get the application pointer
	QApplication* app = qobject_cast<QApplication*>(QApplication::instance());
	if (!app)
		return nullptr;

	QSettings settings;
	// #TODO add hotkey sequence combination reading from settings
	QSharedPointer<QHotkey> hotkey(new QHotkey(keySequence, true, app));

	qDebug() << "Is segistered:" << hotkey->isRegistered();

	QObject::connect(hotkey.get(), &QHotkey::activated, callbackFunction);

	return hotkey;
}

