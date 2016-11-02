/********************************************************************************
** Form generated from reading UI file 'EditorFrame.ui'
**
** Created: Sat Oct 22 19:41:22 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITORFRAME_H
#define UI_EDITORFRAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorFrame
{
public:
    QAction *actionOpenScene;
    QAction *actionSaveLayout;
    QAction *actionExit;
    QAction *actionLoad_Layout;
    QAction *actionSaveScene;
    QAction *actionNewScene;
    QAction *actionSaveAsScene;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menu_Window;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EditorFrame)
    {
        if (EditorFrame->objectName().isEmpty())
            EditorFrame->setObjectName(QString::fromUtf8("EditorFrame"));
        EditorFrame->resize(1024, 768);
        actionOpenScene = new QAction(EditorFrame);
        actionOpenScene->setObjectName(QString::fromUtf8("actionOpenScene"));
        actionSaveLayout = new QAction(EditorFrame);
        actionSaveLayout->setObjectName(QString::fromUtf8("actionSaveLayout"));
        actionExit = new QAction(EditorFrame);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionLoad_Layout = new QAction(EditorFrame);
        actionLoad_Layout->setObjectName(QString::fromUtf8("actionLoad_Layout"));
        actionSaveScene = new QAction(EditorFrame);
        actionSaveScene->setObjectName(QString::fromUtf8("actionSaveScene"));
        actionNewScene = new QAction(EditorFrame);
        actionNewScene->setObjectName(QString::fromUtf8("actionNewScene"));
        actionSaveAsScene = new QAction(EditorFrame);
        actionSaveAsScene->setObjectName(QString::fromUtf8("actionSaveAsScene"));
        centralWidget = new QWidget(EditorFrame);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        EditorFrame->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EditorFrame);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 23));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_Window = new QMenu(menuBar);
        menu_Window->setObjectName(QString::fromUtf8("menu_Window"));
        EditorFrame->setMenuBar(menuBar);
        statusBar = new QStatusBar(EditorFrame);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        EditorFrame->setStatusBar(statusBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_Window->menuAction());
        menu_File->addAction(actionNewScene);
        menu_File->addAction(actionOpenScene);
        menu_File->addSeparator();
        menu_File->addAction(actionSaveScene);
        menu_File->addAction(actionSaveAsScene);
        menu_File->addSeparator();
        menu_File->addAction(actionExit);
        menu_Window->addAction(actionLoad_Layout);
        menu_Window->addAction(actionSaveLayout);

        retranslateUi(EditorFrame);

        QMetaObject::connectSlotsByName(EditorFrame);
    } // setupUi

    void retranslateUi(QMainWindow *EditorFrame)
    {
        EditorFrame->setWindowTitle(QApplication::translate("EditorFrame", "GalaxyEditor", 0, QApplication::UnicodeUTF8));
        actionOpenScene->setText(QApplication::translate("EditorFrame", "&Open", 0, QApplication::UnicodeUTF8));
        actionSaveLayout->setText(QApplication::translate("EditorFrame", "&Save Layout", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("EditorFrame", "E&xit", 0, QApplication::UnicodeUTF8));
        actionLoad_Layout->setText(QApplication::translate("EditorFrame", "&Load Layout", 0, QApplication::UnicodeUTF8));
        actionSaveScene->setText(QApplication::translate("EditorFrame", "&Save", 0, QApplication::UnicodeUTF8));
        actionNewScene->setText(QApplication::translate("EditorFrame", "&New", 0, QApplication::UnicodeUTF8));
        actionSaveAsScene->setText(QApplication::translate("EditorFrame", "Save &As...", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("EditorFrame", "&File", 0, QApplication::UnicodeUTF8));
        menu_Window->setTitle(QApplication::translate("EditorFrame", "&Window", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EditorFrame: public Ui_EditorFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORFRAME_H
