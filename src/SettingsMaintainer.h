/************************************************************************/
/* (c) 2025 Vit Stancl <stancl.vit on gmail>								*/
/************************************************************************/

#pragma once

#include <QString>
#include <QKeySequence>

/** \brief	The settings maintainer. Small wrapper class around the QSettings - just for the simplification of the calls */
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

	bool getStayOnTop() const;
	void setStayOnTop(bool stay);

	bool getStartVisible() const;
	void setStartVisible(bool visible);

	bool getShowOnTimerEnd() const;
	void setShowOnTimerEnd(bool show);

protected:
	const QString timerDurationSettingsKey = "timer/duration";
	const QString m_showWindowHotkeySettingsKey = "hotkey/sequence/ShowWindow";
	const QString m_resetTimerHotkeySettingsKey = "hotkey/sequence/ResetTimer";
	const QString m_pauseTimerHotkeySettingsKey = "hotkey/sequence/PauseTimer";
	const QString m_stayOnTopSettingsKey = "window/stayOnTop";
	const QString m_startVisibleOnStartSettingsKey = "window/stayOnTop";
	const QString m_showOnTimerEndSettingsKey = "window/showOnTimerEnd";

	const int timerDurationDefaultValueMin = 15;
};

