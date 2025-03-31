/************************************************************************/
/* (c) 2025 Vit Stancl <stancl.vit on gmail>								*/
/************************************************************************/

#pragma once

#include <QPushButton>
#include <QColor>

/** \brief	A styling using the hard-coded qss. */
class Styling
{
public:
	static void setButtonStyling(QPushButton* button);
	static void setWidgetStyling(QWidget* dialog);
};

