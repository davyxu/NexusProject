/********************************************************************************
** Form generated from reading UI file 'ProjectView.ui'
**
** Created: Tue Oct 4 18:26:22 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTVIEW_H
#define UI_PROJECTVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectView
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *searchline;

    void setupUi(QWidget *ProjectView)
    {
        if (ProjectView->objectName().isEmpty())
            ProjectView->setObjectName(QString::fromUtf8("ProjectView"));
        ProjectView->resize(277, 639);
        verticalLayout = new QVBoxLayout(ProjectView);
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        searchline = new QLineEdit(ProjectView);
        searchline->setObjectName(QString::fromUtf8("searchline"));

        verticalLayout->addWidget(searchline);


        retranslateUi(ProjectView);

        QMetaObject::connectSlotsByName(ProjectView);
    } // setupUi

    void retranslateUi(QWidget *ProjectView)
    {
        ProjectView->setWindowTitle(QApplication::translate("ProjectView", "Project", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProjectView: public Ui_ProjectView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTVIEW_H
