#include "SettingsWidget.h"
#include "ui_SettingsWidget.h"
#include "MainDialog.h"
#include "HotkeyPushButton.h"
#include "Styling.h"

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
	ui->checkBoxStartWithAppVisible->setCheckState(m_settings.getStartVisible() ? Qt::Checked : Qt::Unchecked);

	ui->checkBoxShowWindowTimerEnd->setCheckState(m_settings.getShowOnTimerEnd() ? Qt::Checked : Qt::Unchecked);

	// Fill dialog from settings
	ui->spinBoxTimer->setValue(m_settings.getTimerDurationMin());

	setStyling();
}

SettingsWidget::~SettingsWidget()
{

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
}

void SettingsWidget::on_checkBoxStartWithAppVisible_checkStateChanged(Qt::CheckState state)
{
	m_settings.setStartVisible(state == Qt::Checked);
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
	unregisterShowWindowHotkey();

	qDebug() << "Register hotkey for: Show Window";

	// Reading the key sequence
	m_showWindowHotkey = registerHotKeyIfPresent(m_settings.getShowWindowKeySequence(),
		[&]()
		{
			if (!hotkeysEnabled())
			{
				ui->pushButtonEditShowWindowHotkey->resendKey();
				return;
			}

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

void SettingsWidget::unregisterShowWindowHotkey()
{
	if (m_showWindowHotkey)
	{
		qDebug() << "Unregister hotkey for: Show Window";
		m_showWindowHotkey->setRegistered(false);
		m_showWindowHotkey = nullptr;
	}
}

void SettingsWidget::registerResetTimerHotkey()
{
	unregisterResetTimerHotkey();

	qDebug() << "Register hotkey for: Reset Timer";

	// Reading the key sequence
	m_resetTimerHotkey = registerHotKeyIfPresent(m_settings.getResetTimerKeySequence(),
		[&]()
		{
			if (!hotkeysEnabled())
			{
				ui->pushButtonEditResetTimerHotkey->resendKey();
				return;
			}

			mainDialog->resetTimer();
		}
	);
}

void SettingsWidget::unregisterResetTimerHotkey()
{
	if (m_resetTimerHotkey)
	{
		qDebug() << "Unregister hotkey for: Reset Timer";
		m_resetTimerHotkey->setRegistered(false);
		m_resetTimerHotkey = nullptr;
	}
}

void SettingsWidget::registerPauseTimerHotkey()
{
	unregisterPauseTimerHotkey();

	qDebug() << "Register hotkey for: Pause Timer";

	// Reading the key sequence
	m_pauseTimerHotkey = registerHotKeyIfPresent(m_settings.getPauseTimerKeySequence(),
		[&]()
		{
			if (!hotkeysEnabled())
			{
				ui->pushButtonEditPauseTimerHotkey->resendKey();
				return;
			}

			mainDialog->pauseTimer();
		}
	);
}

void SettingsWidget::unregisterPauseTimerHotkey()
{
	if (m_pauseTimerHotkey)
	{
		qDebug() << "Unregister hotkey for: Pause Timer";
		m_pauseTimerHotkey->setRegistered(false);
		m_pauseTimerHotkey = nullptr;
	}
}

void SettingsWidget::setStyling()
{
	Styling::setWidgetStyling(this);
	Styling::setButtonStyling(ui->pushButtonEditPauseTimerHotkey);
	Styling::setButtonStyling(ui->pushButtonEditResetTimerHotkey);
	Styling::setButtonStyling(ui->pushButtonEditShowWindowHotkey);
}

bool SettingsWidget::hotkeysEnabled() const
{
	bool somebodyRecording = ui->pushButtonEditPauseTimerHotkey->isRecording() 
		|| ui->pushButtonEditResetTimerHotkey->isRecording()
		|| ui->pushButtonEditShowWindowHotkey->isRecording();

	qDebug() << "Somebody recording: " << somebodyRecording;

	return !somebodyRecording;
}

QSharedPointer<QHotkey> SettingsWidget::registerHotKeyIfPresent(const QKeySequence& keySequence, std::function<void()> callbackFunction)
{
	// Get the application pointer
	QApplication* app = qobject_cast<QApplication*>(QApplication::instance());
	if (!app)
		return nullptr;

	QSharedPointer<QHotkey> hotkey(new QHotkey(keySequence, true, app));

	qDebug() << "Is registered:" << hotkey->isRegistered();

	QObject::connect(hotkey.get(), &QHotkey::activated, callbackFunction);

	return hotkey;

}
