/************************************************************************/
/* (c) 2025 Vit Stancl <stancl.vit on gmail>								*/
/************************************************************************/

#pragma once

#include<QTimer>

/** \brief	A pausable timer. Normal Qt timer is "one-shot" thing. This timer can be paused-resumed and additional timer is used to 
			generate "ticking"
*/
class PausableTimer : public QObject {
	Q_OBJECT
public:
	PausableTimer(int duration, QObject* parent = nullptr);

	/**
	 * \brief	Sets duration in milliseconds
	 *
	 * \param 	duration	The duration.
	 */

	void setDurationMS(int duration);

	/**
	 * \brief	Sets duration in minutes
	 *
	 * \param 	duration	The duration.
	 */

	void setDurationMinutes(int duration);

	void start(int duration = -1);
	void stop();

	void pause();

	void resume();

	bool isStarted() const;
	bool isPaused() const;

	void setTickDurationMSec(int tickDuration) { m_tickDuration = tickDuration; }
	int getTickDurationMSec() const { return m_tickDuration; }

	static QString formatTime(int milliseconds);

private slots:
	void on_updateTick();  // Slot for tick timer
	void on_timeout();

signals:
	void timeout();
	void tick(int currentTime, int remainingTime);


private:
	bool m_paused, m_started;
	QTimer m_timer, m_tickTimer;
	qint64 m_startTime = 0;
	int m_totalDuration;
	int m_remainingTime;
	int m_tickDuration;
};

