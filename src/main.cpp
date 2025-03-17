#include <QApplication>
#include "SettingsDialog.h"

#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QResource>

#include <QHotkey>
#include <QKeySequence>
#include <QSettings>

QSharedPointer<QHotkey> registerHotKeyIfPresent(QApplication *app, SettingsDialog *dialog)
{
	QSettings settings;
	// #TODO add hotkey sequence combination reading from settings
	QSharedPointer<QHotkey> hotkey(new QHotkey(QKeySequence("Ctrl+Alt+P"), true, app));

	qDebug() << "Is segistered:" << hotkey->isRegistered();

	QObject::connect(hotkey.get(), &QHotkey::activated,
		[&]() {
			dialog->showNormal();  // Show the window
			dialog->raise();
			dialog->activateWindow();
		}
	);

	return hotkey;
}

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("TinyMuff");
	QCoreApplication::setApplicationName("TinyPom");

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
	auto hk(registerHotKeyIfPresent(&app, &dialog));

	return app.exec();
}