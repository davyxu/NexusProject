/********************************************************************************
** Form generated from reading UI file 'HierarchyView.ui'
**
** Created: Tue Oct 4 18:26:22 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HIERARCHYVIEW_H
#define UI_HIERARCHYVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HierarchyView
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;

    void setupUi(QWidget *HierarchyView)
    {
        if (HierarchyView->objectName().isEmpty())
            HierarchyView->setObjectName(QString::fromUtf8("HierarchyView"));
        HierarchyView->resize(319, 494);
        HierarchyView->setAcceptDrops(false);
        verticalLayout = new QVBoxLayout(HierarchyView);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, -1);
        lineEdit = new QLineEdit(HierarchyView);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);


        retranslateUi(HierarchyView);

        QMetaObject::connectSlotsByName(HierarchyView);
    } // setupUi

    void retranslateUi(QWidget *HierarchyView)
    {
        HierarchyView->setWindowTitle(QApplication::translate("HierarchyView", "Hierarchy", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class HierarchyView: public Ui_HierarchyView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HIERARCHYVIEW_H
