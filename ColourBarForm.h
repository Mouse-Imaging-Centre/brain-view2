#ifndef COLOURBARFORM_H_
#define COLOURBARFORM_H_

#include "ui_colourBarForm.h"

class ColourBarForm : public QWidget {
	Q_OBJECT
public:
	ColourBarForm(QWidget *parent);
	virtual ~ColourBarForm();
private:
	Ui::colourBarForm ui;
};

#endif /*COLOURBARFORM_H_*/
