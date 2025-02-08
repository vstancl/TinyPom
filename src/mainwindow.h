#include <QDialog>

namespace Ui {
	class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public: 
	SettingsDialog();
	~SettingsDialog();

protected:

private:
	Ui::SettingsDialog* ui;

};