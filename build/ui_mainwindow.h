/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Jul 31 21:53:53 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionRecent;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionRevert;
    QAction *actionClose;
    QAction *actionPrint;
    QAction *actionTake_Snapshot;
    QAction *actionSetup_Snapshots;
    QAction *actionSet_Snapshot_to_Base;
    QAction *actionDelete_Snapshot;
    QAction *actionCreate_Simulation;
    QAction *actionSimulation_Settings;
    QAction *actionJump_to_Base;
    QAction *actionPrevious_Snapshot;
    QAction *actionNext_Snapshot;
    QAction *actionStep;
    QAction *actionPlayPause;
    QAction *actionJump_to_End;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *documents;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuSimulation;
    QMenu *menuSnapshot;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1100, 650);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionRecent = new QAction(MainWindow);
        actionRecent->setObjectName(QString::fromUtf8("actionRecent"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionRevert = new QAction(MainWindow);
        actionRevert->setObjectName(QString::fromUtf8("actionRevert"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/toolbarIcons/icons/close_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose->setIcon(icon);
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        actionTake_Snapshot = new QAction(MainWindow);
        actionTake_Snapshot->setObjectName(QString::fromUtf8("actionTake_Snapshot"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/toolbarIcons/icons/camera_add_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTake_Snapshot->setIcon(icon1);
        actionSetup_Snapshots = new QAction(MainWindow);
        actionSetup_Snapshots->setObjectName(QString::fromUtf8("actionSetup_Snapshots"));
        actionSet_Snapshot_to_Base = new QAction(MainWindow);
        actionSet_Snapshot_to_Base->setObjectName(QString::fromUtf8("actionSet_Snapshot_to_Base"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/toolbarIcons/icons/star_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSet_Snapshot_to_Base->setIcon(icon2);
        actionDelete_Snapshot = new QAction(MainWindow);
        actionDelete_Snapshot->setObjectName(QString::fromUtf8("actionDelete_Snapshot"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/toolbarIcons/icons/camera_close_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete_Snapshot->setIcon(icon3);
        actionCreate_Simulation = new QAction(MainWindow);
        actionCreate_Simulation->setObjectName(QString::fromUtf8("actionCreate_Simulation"));
        actionSimulation_Settings = new QAction(MainWindow);
        actionSimulation_Settings->setObjectName(QString::fromUtf8("actionSimulation_Settings"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/toolbarIcons/icons/advanced.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSimulation_Settings->setIcon(icon4);
        actionJump_to_Base = new QAction(MainWindow);
        actionJump_to_Base->setObjectName(QString::fromUtf8("actionJump_to_Base"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/toolbarIcons/icons/002_77.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionJump_to_Base->setIcon(icon5);
        actionPrevious_Snapshot = new QAction(MainWindow);
        actionPrevious_Snapshot->setObjectName(QString::fromUtf8("actionPrevious_Snapshot"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/toolbarIcons/icons/002_75.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrevious_Snapshot->setIcon(icon6);
        actionNext_Snapshot = new QAction(MainWindow);
        actionNext_Snapshot->setObjectName(QString::fromUtf8("actionNext_Snapshot"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/toolbarIcons/icons/002_76.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNext_Snapshot->setIcon(icon7);
        actionStep = new QAction(MainWindow);
        actionStep->setObjectName(QString::fromUtf8("actionStep"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/toolbarIcons/icons/002_79.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStep->setIcon(icon8);
        actionPlayPause = new QAction(MainWindow);
        actionPlayPause->setObjectName(QString::fromUtf8("actionPlayPause"));
        actionPlayPause->setCheckable(true);
        actionPlayPause->setChecked(false);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/toolbarIcons/icons/002_71.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon9.addFile(QString::fromUtf8(":/toolbarIcons/icons/002_72.png"), QSize(), QIcon::Normal, QIcon::On);
        actionPlayPause->setIcon(icon9);
        actionPlayPause->setAutoRepeat(true);
        actionPlayPause->setPriority(QAction::NormalPriority);
        actionJump_to_End = new QAction(MainWindow);
        actionJump_to_End->setObjectName(QString::fromUtf8("actionJump_to_End"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/toolbarIcons/icons/002_78.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionJump_to_End->setIcon(icon10);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        documents = new QTabWidget(centralWidget);
        documents->setObjectName(QString::fromUtf8("documents"));
        documents->setUsesScrollButtons(true);
        documents->setDocumentMode(true);
        documents->setTabsClosable(true);
        documents->setMovable(true);

        verticalLayout->addWidget(documents);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1100, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuSimulation = new QMenu(menuBar);
        menuSimulation->setObjectName(QString::fromUtf8("menuSimulation"));
        menuSnapshot = new QMenu(menuBar);
        menuSnapshot->setObjectName(QString::fromUtf8("menuSnapshot"));
        MainWindow->setMenuBar(menuBar);

        mainToolBar->addAction(actionTake_Snapshot);
        mainToolBar->addAction(actionDelete_Snapshot);
        mainToolBar->addAction(actionSimulation_Settings);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionJump_to_Base);
        mainToolBar->addAction(actionPrevious_Snapshot);
        mainToolBar->addAction(actionNext_Snapshot);
        mainToolBar->addAction(actionJump_to_End);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionPlayPause);
        mainToolBar->addAction(actionStep);
        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuSimulation->menuAction());
        menuBar->addAction(menuSnapshot->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionRecent);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionRevert);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuFile->addSeparator();
        menuFile->addAction(actionPrint);
        menuSimulation->addAction(actionCreate_Simulation);
        menuSimulation->addAction(actionSimulation_Settings);
        menuSimulation->addSeparator();
        menuSimulation->addAction(actionJump_to_Base);
        menuSimulation->addAction(actionPrevious_Snapshot);
        menuSimulation->addAction(actionNext_Snapshot);
        menuSimulation->addAction(actionJump_to_End);
        menuSimulation->addSeparator();
        menuSimulation->addAction(actionPlayPause);
        menuSimulation->addAction(actionStep);
        menuSnapshot->addAction(actionSetup_Snapshots);
        menuSnapshot->addAction(actionTake_Snapshot);
        menuSnapshot->addAction(actionSet_Snapshot_to_Base);
        menuSnapshot->addAction(actionDelete_Snapshot);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("MainWindow", "New Simulation", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("MainWindow", "Create a new simulation window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open Simulation", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("MainWindow", "Open a saved simulation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionRecent->setText(QApplication::translate("MainWindow", "Recent Simulations", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRecent->setToolTip(QApplication::translate("MainWindow", "Recent simulations", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As...", 0, QApplication::UnicodeUTF8));
        actionRevert->setText(QApplication::translate("MainWindow", "Revert to Last Saved", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRevert->setToolTip(QApplication::translate("MainWindow", "Revert current simulation to last saved version", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionClose->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionPrint->setText(QApplication::translate("MainWindow", "Print...", 0, QApplication::UnicodeUTF8));
        actionPrint->setShortcut(QApplication::translate("MainWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        actionTake_Snapshot->setText(QApplication::translate("MainWindow", "Take Snapshot", 0, QApplication::UnicodeUTF8));
        actionSetup_Snapshots->setText(QApplication::translate("MainWindow", "Setup Snapshots", 0, QApplication::UnicodeUTF8));
        actionSet_Snapshot_to_Base->setText(QApplication::translate("MainWindow", "Set Snapshot to Base", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSet_Snapshot_to_Base->setToolTip(QApplication::translate("MainWindow", "Sets the current snapshot to base", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionDelete_Snapshot->setText(QApplication::translate("MainWindow", "Delete Snapshot", 0, QApplication::UnicodeUTF8));
        actionCreate_Simulation->setText(QApplication::translate("MainWindow", "Create Simulation", 0, QApplication::UnicodeUTF8));
        actionSimulation_Settings->setText(QApplication::translate("MainWindow", "Simulation Settings", 0, QApplication::UnicodeUTF8));
        actionJump_to_Base->setText(QApplication::translate("MainWindow", "Jump to Base", 0, QApplication::UnicodeUTF8));
        actionPrevious_Snapshot->setText(QApplication::translate("MainWindow", "Previous Snapshot", 0, QApplication::UnicodeUTF8));
        actionNext_Snapshot->setText(QApplication::translate("MainWindow", "Next Snapshot", 0, QApplication::UnicodeUTF8));
        actionStep->setText(QApplication::translate("MainWindow", "Step", 0, QApplication::UnicodeUTF8));
        actionPlayPause->setText(QApplication::translate("MainWindow", "Play / Pause", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPlayPause->setToolTip(QApplication::translate("MainWindow", "Play / Pause Simulation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPlayPause->setShortcut(QApplication::translate("MainWindow", "P", 0, QApplication::UnicodeUTF8));
        actionJump_to_End->setText(QApplication::translate("MainWindow", "Jump to End", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuSimulation->setTitle(QApplication::translate("MainWindow", "Simulation", 0, QApplication::UnicodeUTF8));
        menuSnapshot->setTitle(QApplication::translate("MainWindow", "Snapshot", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
