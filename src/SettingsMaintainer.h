#pragma once

#include <QString>

class SettingsMaintainer
{
public:
	int getTimerDurationMin();
	void setTimerDurationMin(int value);

protected:
	const QString timerDurationSettingsKey = "TinyPom_timer_duration";
	const int timerDurationDefaultValueMin = 15;
};

