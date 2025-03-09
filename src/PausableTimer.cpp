#include "PausableTimer.h"
#include <QDateTime>
#include <QDebug>

PausableTimer::PausableTimer(int duration, QObject* parent /*= nullptr*/) 
	: QObject(parent)
	, m_totalDuration(duration)
	, m_remainingTime(duration)
	, m_tickDuration(1000)
{
	m_timer.setSingleShot(true);
	connect(&m_timer, &QTimer::timeout, this, &PausableTimer::on_timeout);
	connect(&m_tickTimer, &QTimer::timeout, this, &PausableTimer::on_updateTick);
}

void PausableTimer::setDuration(int duration)
{
	m_totalDuration = duration;
}

void PausableTimer::setDurationMinutes(int duration)
{
	setDuration(60 * 1000 * duration);
}

void PausableTimer::start(int duration /*= -1*/)
{
	// Stop currently running timer and reset duration...
	//if(duration <= 0)
		stop();

	m_paused = false;

	m_startTime = QDateTime::currentMSecsSinceEpoch();
	m_timer.start(m_remainingTime);

	if(m_tickDuration > 0)
		m_tickTimer.start(m_tickDuration);  
}

void PausableTimer::stop()
{
	m_paused = true;
	m_remainingTime = m_totalDuration;
	m_timer.stop();
	m_tickTimer.stop();
}

void PausableTimer::pause()
{
	m_paused = m_timer.isActive();

	if (m_timer.isActive()) {
		m_remainingTime -= (QDateTime::currentMSecsSinceEpoch() - m_startTime);
		qDebug() << "Timer pause. Remaining time: " << m_remainingTime;
		m_timer.stop();
		m_tickTimer.stop();
	}
}

void PausableTimer::resume()
{
	if (!isPaused())
		return;
	qDebug() << "Timer resume. Remaining time: " << m_remainingTime;
	start(m_remainingTime);
}

bool PausableTimer::isPaused() const
{
	return m_paused;
}

QString PausableTimer::formatTime(int milliseconds)
{
	int totalSeconds = milliseconds / 1000;
	int hours = totalSeconds / 3600;
	int minutes = (totalSeconds % 3600) / 60;
	int seconds = totalSeconds % 60;

	return QString::asprintf("%02d:%02d:%02d", hours, minutes, seconds);
}

void PausableTimer::on_updateTick()
{
	qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - m_startTime;
	qint64 remaining = m_remainingTime - elapsed;

	if (remaining < 0) 
	{
		elapsed = m_totalDuration;
		remaining = 0;
	}

	emit tick(elapsed, remaining);
	//qDebug() << "Tick: Remaining time = " << formatTime(remaining) << "ms";
}

void PausableTimer::on_timeout()
{
	stop();
	emit timeout();
}

