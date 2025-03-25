#include "SettingsMaintainer.h"
#include <QSettings>

int SettingsMaintainer::getTimerDurationMin() const
{
	QSettings settings;
	int duration = settings.value(timerDurationSettingsKey, timerDurationDefaultValueMin).toInt();
	return duration;
}

void SettingsMaintainer::setTimerDurationMin(int value)
{
	QSettings settings;
	settings.setValue(timerDurationSettingsKey, value);
}

QKeySequence SettingsMaintainer::getShowWindowKeySequence() const
{
	QSettings settings;
	return settings.value(m_showWindowHotkeySettingsKey, "").value<QKeySequence>();
}

void SettingsMaintainer::setShowWindowKeySequence(const QKeySequence& keySeq)
{
	QSettings settings;
	settings.setValue(m_showWindowHotkeySettingsKey, keySeq);
}

QKeySequence SettingsMaintainer::getResetTimerKeySequence() const
{
	QSettings settings;
	return settings.value(m_resetTimerHotkeySettingsKey, "").value<QKeySequence>();
}

void SettingsMaintainer::setResetTimerKeySequence(const QKeySequence& keySeq)
{
	QSettings settings;
	settings.setValue(m_resetTimerHotkeySettingsKey, keySeq);
}

QKeySequence SettingsMaintainer::getPauseTimerKeySequence() const
{
	QSettings settings;
	return settings.value(m_pauseTimerHotkeySettingsKey, "").value<QKeySequence>();
}

void SettingsMaintainer::setPauseTimerKeySequence(const QKeySequence& keySeq)
{
	QSettings settings;
	settings.setValue(m_pauseTimerHotkeySettingsKey, keySeq);
}

bool SettingsMaintainer::getStayOnTop() const
{
	QSettings settings;
	return settings.value(m_stayOnTopSettingsKey, false).value<bool>();
}

void SettingsMaintainer::setStayOnTop(bool stay)
{
	QSettings settings;
	settings.setValue(m_stayOnTopSettingsKey, stay);
}

bool SettingsMaintainer::getStartVisible() const
{
	QSettings settings;
	return settings.value(m_startVisibleOnStartSettingsKey, true).value<bool>();
}

void SettingsMaintainer::setStartVisible(bool visible)
{
	QSettings settings;
	settings.setValue(m_startVisibleOnStartSettingsKey, visible);
}

bool SettingsMaintainer::getShowOnTimerEnd() const
{
	QSettings settings;
	return settings.value(m_showOnTimerEndSettingsKey, false).value<bool>();
}

void SettingsMaintainer::setShowOnTimerEnd(bool show)
{
	QSettings settings;
	settings.setValue(m_showOnTimerEndSettingsKey, show);

}
