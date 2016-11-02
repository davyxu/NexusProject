/********************************************************************************
** Form generated from reading UI file 'MaterialView.ui'
**
** Created: Tue Oct 4 18:26:23 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALVIEW_H
#define UI_MATERIALVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaterialView
{
public:

    void setupUi(QWidget *MaterialView)
    {
        if (MaterialView->objectName().isEmpty())
            MaterialView->setObjectName(QString::fromUtf8("MaterialView"));
        MaterialView->resize(523, 400);

        retranslateUi(MaterialView);

        QMetaObject::connectSlotsByName(MaterialView);
    } // setupUi

    void retranslateUi(QWidget *MaterialView)
    {
        MaterialView->setWindowTitle(QApplication::translate("MaterialView", "Material", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MaterialView: public Ui_MaterialView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALVIEW_H
