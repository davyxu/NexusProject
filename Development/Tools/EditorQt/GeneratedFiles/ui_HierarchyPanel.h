/********************************************************************************
** Form generated from reading UI file 'HierarchyPanel.ui'
**
** Created: Sat Oct 22 19:41:22 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HIERARCHYPANEL_H
#define UI_HIERARCHYPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HierarchyPanel
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;

    void setupUi(QWidget *HierarchyPanel)
    {
        if (HierarchyPanel->objectName().isEmpty())
            HierarchyPanel->setObjectName(QString::fromUtf8("HierarchyPanel"));
        HierarchyPanel->resize(319, 494);
        HierarchyPanel->setAcceptDrops(false);
        verticalLayout = new QVBoxLayout(HierarchyPanel);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, -1);
        lineEdit = new QLineEdit(HierarchyPanel);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);


        retranslateUi(HierarchyPanel);

        QMetaObject::connectSlotsByName(HierarchyPanel);
    } // setupUi

    void retranslateUi(QWidget *HierarchyPanel)
    {
        HierarchyPanel->setWindowTitle(QApplication::translate("HierarchyPanel", "Hierarchy", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class HierarchyPanel: public Ui_HierarchyPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HIERARCHYPANEL_H
