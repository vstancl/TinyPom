#include "SettingsMaintainer.h"
#include <QSettings>

int SettingsMaintainer::getTimerDurationMin()
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
