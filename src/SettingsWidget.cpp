#include "SettingsWidget.h"
#include "ui_SettingsWidget.h"
#include "MainDialog.h"
#include "HotkeyPushButton.h"

SettingsWidget::SettingsWidget(QDialog* parent)
	: ui(new Ui::SettingsWidget)
	, mainDialog(dynamic_cast<MainDialog *>(parent))
{
	ui->setupUi(this);

	ui->pushButtonEditShowWindowHotkey->setEmptyText("Set Show Window Shortcut");
	ui->pushButtonEditShowWindowHotkey->setPrefixText("Show Window: ");

	ui->pushButtonEditResetTimerHotkey->setEmptyText("Set Start/Reset Timer Shortcut");
	ui->pushButtonEditResetTimerHotkey->setPrefixText("Start/Reset Timer: ");

	ui->pushButtonEditPauseTimerHotkey->setEmptyText("Set Pause Timer Shortcut");
	ui->pushButtonEditPauseTimerHotkey->setPrefixText("Pause Timer: ");

	// Hotkeys scannig signals
	connect(ui->pushButtonEditShowWindowHotkey, &HotkeyPushButton::hotkeyChanged, this, &SettingsWidget::on_hotkeySet);
	connect(ui->pushButtonEditResetTimerHotkey, &HotkeyPushButton::hotkeyChanged, this, &SettingsWidget::on_hotkeySet);
	connect(ui->pushButtonEditPauseTimerHotkey, &HotkeyPushButton::hotkeyChanged, this, &SettingsWidget::on_hotkeySet);

	registerShowWindowHotkey();
	registerResetTimerHotkey();
	registerPauseTimerHotkey();

	// Fill hotkey edits
	ui->pushButtonEditShowWindowHotkey->setKeySequence(m_settings.getShowWindowKeySequence());
	ui->pushButtonEditResetTimerHotkey->setKeySequence(m_settings.getResetTimerKeySequence());
	ui->pushButtonEditPauseTimerHotkey->setKeySequence(m_settings.getPauseTimerKeySequence());

	// Set checkboxes
	ui->checkBoxPinOnTop->setCheckState(m_settings.getStayOnTop() ? Qt::Checked : Qt::Unchecked);

	ui->checkBoxShowWindowTimerEnd->setCheckState(m_settings.getShowOnTimerEnd() ? Qt::Checked : Qt::Unchecked);

	// Fill dialog from settings
	ui->spinBoxTimer->setValue(m_settings.getTimerDurationMin());

}

SettingsWidget::~SettingsWidget()
{

}

void SettingsWidget::on_pushButtonEditShowWindowHotkey_clicked()
{
	// Disable other windows scanning
	disableHotkeysScanning();
	/** \brief	Default constructor */
// 	ui->lineEditShowWindowHotkey->setFocus();
// 	ui->lineEditShowWindowHotkey->setScanning(true);
}

void SettingsWidget::on_pushButtonEditResetTimerHotkey_clicked()
{
	// Disable other windows scanning
	disableHotkeysScanning();
	/** \brief	Default constructor */
// 	ui->lineEditResetTimerHotkey->setFocus();
// 	ui->lineEditResetTimerHotkey->setScanning(true);
}

void SettingsWidget::on_pushButtonEditPauseTimerHotkey_clicked()
{
	// Disable other windows scanning
	disableHotkeysScanning();
	/** \brief	Default constructor */
// 	ui->lineEditPauseTimerHotkey->setFocus();
// 	ui->lineEditPauseTimerHotkey->setScanning(true);
}

void SettingsWidget::on_checkBoxPinOnTop_checkStateChanged(Qt::CheckState state)
{
	Qt::WindowFlags flags = windowFlags();
	if (state == Qt::Checked)
	{
		m_settings.setStayOnTop(true);
		mainDialog->setWindowFlags(flags | Qt::WindowStaysOnTopHint);  // Add the flag
	}
	else
	{
		m_settings.setStayOnTop(false);
		mainDialog->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);  // Remove the flag
	}

	// #Todo - call parent window
	mainDialog->show();
}

void SettingsWidget::on_checkBoxShowWindowTimerEnd_checkStateChanged(Qt::CheckState state)
{
	m_settings.setShowOnTimerEnd(state == Qt::Checked);
}

void SettingsWidget::on_spinBoxTimer_valueChanged(int value)
{
	m_settings.setTimerDurationMin(value);
}

void SettingsWidget::on_hotkeySet(const QList<int>& keys, Qt::KeyboardModifiers modifiers, HotkeyPushButton* sender)
{
	QKeySequence keySeq(modifiers | keys.last());

	if (sender == ui->pushButtonEditShowWindowHotkey)
	{
		m_settings.setShowWindowKeySequence(keySeq);
		registerShowWindowHotkey();

	}

	if (sender == ui->pushButtonEditResetTimerHotkey)
	{
		m_settings.setResetTimerKeySequence(keySeq);
		registerResetTimerHotkey();
	}

	if (sender == ui->pushButtonEditPauseTimerHotkey)
	{
		m_settings.setPauseTimerKeySequence(keySeq);
		registerPauseTimerHotkey();
	}
}

void SettingsWidget::registerShowWindowHotkey()
{
	if (m_showWindowHotkey)
	{
		m_showWindowHotkey->setRegistered(false);
		m_showWindowHotkey = nullptr;
	}

	// Reading the key sequence
	m_showWindowHotkey = registerHotKeyIfPresent(m_settings.getShowWindowKeySequence(),
		[&]()
		{
			if (mainDialog->isVisible())
			{
				mainDialog->hide();
			}
			else
			{
				mainDialog->showNormal();  // Show the window
				mainDialog->raise();
				mainDialog->activateWindow();
			}
		}

	);
}

void SettingsWidget::registerResetTimerHotkey()
{
	if (m_resetTimerHotkey)
	{
		m_resetTimerHotkey->setRegistered(false);
		m_resetTimerHotkey = nullptr;
	}

	// Reading the key sequence
	m_resetTimerHotkey = registerHotKeyIfPresent(m_settings.getResetTimerKeySequence(),
		[&]()
		{
			mainDialog->resetTimer();
		}
	);

}

void SettingsWidget::registerPauseTimerHotkey()
{
	if (m_pauseTimerHotkey)
	{
		m_pauseTimerHotkey->setRegistered(false);
		m_pauseTimerHotkey = nullptr;
	}

	// Reading the key sequence
	m_pauseTimerHotkey = registerHotKeyIfPresent(m_settings.getPauseTimerKeySequence(),
		[&]()
		{
			mainDialog->pauseTimer();
		}
	);
}

void SettingsWidget::disableHotkeysScanning()
{
//	ui->lineEditShowWindowHotkey->setScanning(false);
}

QSharedPointer<QHotkey> SettingsWidget::registerHotKeyIfPresent(const QKeySequence& keySequence, std::function<void()> callbackFunction)
{
	// Get the application pointer
	QApplication* app = qobject_cast<QApplication*>(QApplication::instance());
	if (!app)
		return nullptr;

	// #TODO add hotkey sequence combination reading from settings
	QSharedPointer<QHotkey> hotkey(new QHotkey(keySequence, true, app));

	qDebug() << "Is registered:" << hotkey->isRegistered();

	QObject::connect(hotkey.get(), &QHotkey::activated, callbackFunction);

	return hotkey;

}
