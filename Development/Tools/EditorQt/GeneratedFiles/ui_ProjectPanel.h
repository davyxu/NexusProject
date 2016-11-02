/********************************************************************************
** Form generated from reading UI file 'ProjectPanel.ui'
**
** Created: Sat Oct 22 19:41:22 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTPANEL_H
#define UI_PROJECTPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectPanel
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *searchline;

    void setupUi(QWidget *ProjectPanel)
    {
        if (ProjectPanel->objectName().isEmpty())
            ProjectPanel->setObjectName(QString::fromUtf8("ProjectPanel"));
        ProjectPanel->resize(277, 639);
        verticalLayout = new QVBoxLayout(ProjectPanel);
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        searchline = new QLineEdit(ProjectPanel);
        searchline->setObjectName(QString::fromUtf8("searchline"));

        verticalLayout->addWidget(searchline);


        retranslateUi(ProjectPanel);

        QMetaObject::connectSlotsByName(ProjectPanel);
    } // setupUi

    void retranslateUi(QWidget *ProjectPanel)
    {
        ProjectPanel->setWindowTitle(QApplication::translate("ProjectPanel", "Project", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProjectPanel: public Ui_ProjectPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTPANEL_H
