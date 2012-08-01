/********************************************************************************
** Form generated from reading UI file 'atomeditor.ui'
**
** Created: Tue Jul 31 19:20:13 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATOMEDITOR_H
#define UI_ATOMEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AtomEditor
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *layout;
    QLabel *label;
    QComboBox *type;
    QLabel *label_2;
    QSpinBox *state;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout;
    QLineEdit *reaction;
    QToolButton *reactionError;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *eReaction;

    void setupUi(QDialog *AtomEditor)
    {
        if (AtomEditor->objectName().isEmpty())
            AtomEditor->setObjectName(QString::fromUtf8("AtomEditor"));
        AtomEditor->resize(300, 200);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AtomEditor->sizePolicy().hasHeightForWidth());
        AtomEditor->setSizePolicy(sizePolicy);
        AtomEditor->setMinimumSize(QSize(300, 200));
        AtomEditor->setMaximumSize(QSize(300, 200));
        formLayoutWidget = new QWidget(AtomEditor);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 281, 181));
        layout = new QFormLayout(formLayoutWidget);
        layout->setObjectName(QString::fromUtf8("layout"));
        layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        layout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);

        layout->setWidget(0, QFormLayout::LabelRole, label);

        type = new QComboBox(formLayoutWidget);
        type->setObjectName(QString::fromUtf8("type"));
        type->setEnabled(false);
        type->setMaximumSize(QSize(125, 16777215));
        type->setMaxVisibleItems(20);

        layout->setWidget(0, QFormLayout::FieldRole, type);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        layout->setWidget(1, QFormLayout::LabelRole, label_2);

        state = new QSpinBox(formLayoutWidget);
        state->setObjectName(QString::fromUtf8("state"));
        state->setEnabled(false);
        state->setMaximumSize(QSize(60, 16777215));
        QFont font1;
        font1.setPointSize(9);
        state->setFont(font1);
        state->setMaximum(65535);

        layout->setWidget(1, QFormLayout::FieldRole, state);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);

        layout->setWidget(2, QFormLayout::LabelRole, label_6);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        reaction = new QLineEdit(formLayoutWidget);
        reaction->setObjectName(QString::fromUtf8("reaction"));
        reaction->setEnabled(false);

        horizontalLayout->addWidget(reaction);

        reactionError = new QToolButton(formLayoutWidget);
        reactionError->setObjectName(QString::fromUtf8("reactionError"));

        horizontalLayout->addWidget(reactionError);


        layout->setLayout(2, QFormLayout::FieldRole, horizontalLayout);

        label_7 = new QLabel(formLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);

        layout->setWidget(3, QFormLayout::LabelRole, label_7);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        eReaction = new QLineEdit(formLayoutWidget);
        eReaction->setObjectName(QString::fromUtf8("eReaction"));
        eReaction->setEnabled(false);

        horizontalLayout_2->addWidget(eReaction);


        layout->setLayout(3, QFormLayout::FieldRole, horizontalLayout_2);


        retranslateUi(AtomEditor);

        QMetaObject::connectSlotsByName(AtomEditor);
    } // setupUi

    void retranslateUi(QDialog *AtomEditor)
    {
        AtomEditor->setWindowTitle(QApplication::translate("AtomEditor", "Atom Editor", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AtomEditor", "Element:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AtomEditor", "State:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("AtomEditor", "Reaction:", 0, QApplication::UnicodeUTF8));
        reactionError->setText(QApplication::translate("AtomEditor", "...", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("AtomEditor", "E Reaction:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AtomEditor: public Ui_AtomEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATOMEDITOR_H
