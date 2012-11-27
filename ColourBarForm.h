#ifndef COLOURBARFORM_H_
#define COLOURBARFORM_H_

/** ColourBarForm
 ** Code to display the widget for showing the current colour bar in use
 ** and setting upper and lower limits. 
 ** The actual GUI stuff mostly sits in the UI form.
 **/

#include "ui_colourBarForm.h"

class ColourBarForm : public QWidget {
	Q_OBJECT
public:
	/* constructor */
	ColourBarForm(QWidget *parent);
	/* destructor */
	virtual ~ColourBarForm();
private:
	/* accessthe GUI form */
	Ui::colourBarForm ui;
};

#endif /*COLOURBARFORM_H_*/
