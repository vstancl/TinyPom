#include <QApplication>
#include "mainwindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	SettingsDialog dialog;
	dialog.show();

	return app.exec();
}