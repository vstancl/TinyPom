#pragma once

#include <QPushButton>
#include <QColor>

class Styling
{
public:
	static void setButtonStyling(QPushButton* button, const QColor & borderColor);
	static void setDialogStyling(QDialog* dialog, const QColor& borderColor);

	inline const static QColor colorButtonBackground = QColor("#323232");
	inline const static QColor colorButtonPressedBackground = QColor("#adadad");
	inline const static QColor colorWindowBackground = QColor("#505050");
	inline const static QColor colorFont = QColor("#c9c9c9");
	inline const static QColor colorDecoration = QColor("#adadad");
};

