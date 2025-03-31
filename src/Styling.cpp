/************************************************************************/
/* (c) 2025 Vit Stancl <stancl.vit on gmail>								*/
/************************************************************************/

#include "Styling.h"
#include <QDialog>

void Styling::setButtonStyling(QPushButton* button)
{
	if (!button) 
		return; // Safety check

	QString styleSheet = R"(
		QPushButton
		{
			background-color: rgb(66, 66, 66);
			border: 1px solid #b5e5cf;
			padding: 4px;
		}

		QPushButton:hover
		{
			background-color: rgb(115, 115, 115);
			border: 1px solid #b5e5cf;
		}

		QPushButton:pressed
		{
			background-color: rgb(50, 50, 50);
			border: 1px solid #d8e6d1;
		}

		QPushButton:checked
		{
			background-color: rgb(50, 50, 50);
			border: 1px solid #d8e6d1;
		}
	)";

	//rgb(170, 214, 160);
	//rgb(255, 163, 132);
	button->setStyleSheet(styleSheet);
}

void Styling::setWidgetStyling(QWidget* widget)
{
	if (!widget) 
		return; // Safety check

	QString styleSheet = R"(
		background-color: rgb(80, 80, 80);
		font: 10pt "Segoe UI";
		color: rgb(220, 220, 220);
	)";

	widget->setStyleSheet(styleSheet);

}
