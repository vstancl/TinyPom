#pragma once

#include<QTimer>

class PausableTimer : public QObject {
	Q_OBJECT
public:
	PausableTimer(int duration, QObject* parent = nullptr);

	void setDuration(int duration);
	void setDurationMinutes(int duration);

	void start(int duration = -1);
	void stop();

	void pause();

	void resume();

	bool isPaused() const;

signals:
	void timeout();

private:
	bool paused;
	QTimer timer;
	qint64 startTime = 0;
	int totalDuration;
	int remainingTime;
};

