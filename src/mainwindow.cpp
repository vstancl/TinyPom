#include "mainwindow.h"
#include "ui_mainwindow.h"

SettingsDialog::SettingsDialog()
	: ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

