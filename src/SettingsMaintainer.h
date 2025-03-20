#pragma once

#include <QString>
#include <QKeySequence>

class SettingsMaintainer
{
public:
	int getTimerDurationMin() const;
	void setTimerDurationMin(int value);

	QKeySequence getShowWindowKeySequence() const; 
	void setShowWindowKeySequence(const QKeySequence& keySeq);

	QKeySequence getResetTimerKeySequence() const;
	void setResetTimerKeySequence(const QKeySequence& keySeq);

	QKeySequence getPauseTimerKeySequence() const;
	void setPauseTimerKeySequence(const QKeySequence& keySeq);

protected:
	const QString timerDurationSettingsKey = "TinyPom_timer_duration";
	const QString m_showWindowHotkeySettingsKey = "hotkey/sequence/ShowWindow";
	const QString m_resetTimerHotkeySettingsKey = "hotkey/sequence/ResetTimer";
	const QString m_pauseTimerHotkeySettingsKey = "hotkey/sequence/PauseTimer";

	const int timerDurationDefaultValueMin = 15;
};

