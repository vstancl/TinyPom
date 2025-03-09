#include "PausableTimer.h"
#include <QDateTime>
#include <QDebug>

PausableTimer::PausableTimer(int duration, QObject* parent /*= nullptr*/) 
	: QObject(parent)
	, totalDuration(duration)
	, remainingTime(duration)
{
	timer.setSingleShot(true);
	connect(&timer, &QTimer::timeout, this, &PausableTimer::timeout);
}

void PausableTimer::setDuration(int duration)
{
	totalDuration = duration;
}

void PausableTimer::setDurationMinutes(int duration)
{
	setDuration(1000 * duration);
}

void PausableTimer::start(int duration /*= -1*/)
{
	// Stop currently running timer and reset duration...
	if(duration <= 0)
		stop();

	paused = false;

	startTime = QDateTime::currentMSecsSinceEpoch();
	timer.start(remainingTime);
}

void PausableTimer::stop()
{
	paused = true;
	remainingTime = totalDuration;
	timer.stop();
}

void PausableTimer::pause()
{
	paused = timer.isActive();

	if (timer.isActive()) {
		remainingTime -= (QDateTime::currentMSecsSinceEpoch() - startTime);
		qDebug() << "Timer pause. Remaining time: " << remainingTime;
		timer.stop();
	}
}

void PausableTimer::resume()
{
	if (!isPaused())
		return;
	qDebug() << "Timer resume. Remaining time: " << remainingTime;
	start(remainingTime);
}

bool PausableTimer::isPaused() const
{
	return paused;
}

