#include "Styling.h"
#include <QDialog>

void Styling::setButtonStyling(QPushButton* button, const QColor& borderColor)
{
	if (!button) return; // Safety check

	// Convert QColor to a hex string
	QString colorStr = borderColor.name(QColor::HexRgb);

	// Define the stylesheet with the dynamic border color
	QString styleSheet = QString(
		"QPushButton {"
		"    border: 3px solid %1;"  // Dynamic border color
		"    border-radius: 15px;"
		"    background-color: %2;"
		"    color: %3;"
		"    font-family: 'Poppins';"  /* Use quotes around font names */
		"    font-size: 18px;"
		"    padding: 5px 16px;"
		"    min-width: 120px;" /* Ensure button is wide enough */
		"    min-height: 50px;" /* Ensure button height aligns with the border-radius */
		"}"
		"QPushButton:hover {"
		"    background-color: #3b3b3b;"
		"}"
		"QPushButton:pressed {"
		"    background-color: %4;"  // Separate color for pressed state
		"    color: black;"
		"}"
	).arg(colorStr)
		.arg(colorButtonBackground.name(QColor::HexRgb))
		.arg(colorFont.name(QColor::HexRgb))
		.arg(colorButtonPressedBackground.name(QColor::HexRgb));  // Use a new variable for pressed bg color


	// Apply the stylesheet to the button
	button->setStyleSheet(styleSheet);
}

void Styling::setDialogStyling(QDialog* dialog, const QColor& borderColor)
{
	if (!dialog) return; // Safety check

	// Convert QColor to a hex string
	QString colorStr = borderColor.name(QColor::HexRgb);

	// Define the stylesheet with the dynamic border color
	QString styleSheet = QString(
		"QDialog {"
		"    border: 3px solid %1;"  // Dynamic border color
//		"    border-radius: 15px;"
		"    background-color: %2;"
		"    color: %3;"
		"    font-family: 'Poppins';"  /* Use quotes around font names */
		"    font-size: 18px;"
		"    padding: 5px 16px;"
		"}"
	).arg(colorStr)
		.arg(colorWindowBackground.name(QColor::HexRgb))
		.arg(colorFont.name(QColor::HexRgb));


	// Apply the stylesheet to the button
	dialog->setStyleSheet(styleSheet);

}
