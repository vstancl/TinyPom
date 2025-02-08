#include <QDialog>

namespace Ui {
	class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public: 
	SettingsDialog();
	
	void setVisible(bool visible) override;

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	Ui::SettingsDialog* ui;

};