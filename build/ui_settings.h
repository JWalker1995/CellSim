/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created: Mon Jul 30 20:26:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *comboBox;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName(QString::fromUtf8("Settings"));
        Settings->resize(400, 300);
        formLayoutWidget = new QWidget(Settings);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 171, 80));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        comboBox = new QComboBox(formLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, comboBox);


        retranslateUi(Settings);

        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QApplication::translate("Settings", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Settings", "Accurate Simulation", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
