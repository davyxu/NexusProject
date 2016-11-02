/********************************************************************************
** Form generated from reading UI file 'MaterialPanel.ui'
**
** Created: Sat Oct 22 19:41:22 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALPANEL_H
#define UI_MATERIALPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaterialPanel
{
public:

    void setupUi(QWidget *MaterialPanel)
    {
        if (MaterialPanel->objectName().isEmpty())
            MaterialPanel->setObjectName(QString::fromUtf8("MaterialPanel"));
        MaterialPanel->resize(523, 400);

        retranslateUi(MaterialPanel);

        QMetaObject::connectSlotsByName(MaterialPanel);
    } // setupUi

    void retranslateUi(QWidget *MaterialPanel)
    {
        MaterialPanel->setWindowTitle(QApplication::translate("MaterialPanel", "Material", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MaterialPanel: public Ui_MaterialPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALPANEL_H
