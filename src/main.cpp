#include <QApplication>
#include "mainwindow.h"

#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QResource>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		auto choice = QMessageBox::critical(nullptr, QObject::tr("Systray"),
			QObject::tr("I couldn't detect any system tray on this system."),
			QMessageBox::Close | QMessageBox::Ignore);
		if (choice == QMessageBox::Close)
			return 1;
		// Otherwise "lurk": if a system tray is started later, the icon will appear.
	}

	QResource::registerResource("/res/resources.rcc");

	QApplication::setQuitOnLastWindowClosed(false);

	SettingsDialog dialog;
	dialog.show();

	return app.exec();
}