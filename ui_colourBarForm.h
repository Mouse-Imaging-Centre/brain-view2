/********************************************************************************
** Form generated from reading ui file 'colourBarForm.ui'
**
** Created: Wed Aug 20 14:14:43 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_COLOURBARFORM_H
#define UI_COLOURBARFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "Quarter/QuarterWidget.h"

QT_BEGIN_NAMESPACE

class Ui_colourBarForm
{
public:
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *upperLimit;
    SIM::Coin3D::Quarter::QuarterWidget *colourBarViewer;
    QPlainTextEdit *lowerLimit;

    void setupUi(QWidget *colourBarForm)
    {
    if (colourBarForm->objectName().isEmpty())
        colourBarForm->setObjectName(QString::fromUtf8("colourBarForm"));
    colourBarForm->resize(150, 442);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(colourBarForm->sizePolicy().hasHeightForWidth());
    colourBarForm->setSizePolicy(sizePolicy);
    colourBarForm->setMaximumSize(QSize(150, 16777215));
    verticalLayout = new QVBoxLayout(colourBarForm);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    upperLimit = new QPlainTextEdit(colourBarForm);
    upperLimit->setObjectName(QString::fromUtf8("upperLimit"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(upperLimit->sizePolicy().hasHeightForWidth());
    upperLimit->setSizePolicy(sizePolicy1);
    upperLimit->setMaximumSize(QSize(16777215, 35));

    verticalLayout->addWidget(upperLimit);

    colourBarViewer = new SIM::Coin3D::Quarter::QuarterWidget(colourBarForm);
    colourBarViewer->setObjectName(QString::fromUtf8("colourBarViewer"));
    QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(colourBarViewer->sizePolicy().hasHeightForWidth());
    colourBarViewer->setSizePolicy(sizePolicy2);
    colourBarViewer->setMinimumSize(QSize(60, 200));
    colourBarViewer->setBaseSize(QSize(100, 100));

    verticalLayout->addWidget(colourBarViewer);

    lowerLimit = new QPlainTextEdit(colourBarForm);
    lowerLimit->setObjectName(QString::fromUtf8("lowerLimit"));
    sizePolicy1.setHeightForWidth(lowerLimit->sizePolicy().hasHeightForWidth());
    lowerLimit->setSizePolicy(sizePolicy1);
    lowerLimit->setMaximumSize(QSize(16777215, 35));

    verticalLayout->addWidget(lowerLimit);


    retranslateUi(colourBarForm);

    QMetaObject::connectSlotsByName(colourBarForm);
    } // setupUi

    void retranslateUi(QWidget *colourBarForm)
    {
    colourBarForm->setWindowTitle(QApplication::translate("colourBarForm", "Form", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    colourBarViewer->setToolTip(QApplication::translate("colourBarForm", "QuarterWidget", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_WHATSTHIS
    colourBarViewer->setWhatsThis(QApplication::translate("colourBarForm", "The QuarterWidget displays Open Inventor scene graphs.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS

    Q_UNUSED(colourBarForm);
    } // retranslateUi

};

namespace Ui {
    class colourBarForm: public Ui_colourBarForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLOURBARFORM_H
