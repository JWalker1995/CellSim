#include "atomeditor.h"
#include "ui_atomeditor.h"
#include "globals.h"

#include <QString>
#include <QFont>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>

#include <QDebug>

AtomEditor::AtomEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AtomEditor)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    prevElement = -1;
    prevState = -1;
    prevReaction = QString();
    element = -1;
    state = -1;
    reaction = QString();

    // Populate atom types
    int i = 0;
    while (i < Globals::numElements)
    {
        Atom* a = Globals::getAtom(i);
        ui->type->addItem(Globals::getElementName(i)+" ("+a->symbol+")");
        i++;
    }

    connect(ui->type, SIGNAL(highlighted(int)), this, SLOT(changeType(int)));
    connect(ui->state, SIGNAL(valueChanged(int)), this, SLOT(changeState(int)));
    connect(ui->reaction, SIGNAL(textEdited(QString)), this, SLOT(changeReaction(QString)));
    ui->type->installEventFilter(this);
    ui->state->installEventFilter(this);
    ui->reaction->installEventFilter(this);
}

void AtomEditor::init(QList<Atom *> *sels)
{
    // Sets refrence to selected atoms. Should not be any atoms in it yet.
    selected = sels;
}

void AtomEditor::updateAdd()
{
    // Called after a single atom is selected.
    if (selected->count() == 1)
    {
        // There is only one atom selected. Populate fields to match it.
        Atom* a = selected->first();
        element = a->element;
        state = a->state;
        reaction = a->reactionStr;

        enableAll(true);
        ui->type->setCurrentIndex(element);
        ui->state->setValue(state);
        ui->reaction->setText(reaction);
    }
    else
    {
        // There is more than one atom selected, so update the fields to match them all.
        addAtom(selected->last());
    }
}

void AtomEditor::updateRemove()
{
    // Called after any number of atoms are unselected.
    int c = selected->count();
    if (c)
    {
        // Populate fields with first atom
        Atom* a = selected->first();
        element = a->element;
        state = a->state;
        reaction = a->reactionStr;

        enableAll(true);
        ui->type->setCurrentIndex(element);
        ui->state->setValue(state);
        ui->reaction->setText(reaction);

        // Loop through the rest of the atmos and make sure fields match.
        int i = 1;
        while (i < c)
        {
            addAtom(selected->at(i));
            i++;
        }
    }
    else
    {
        enableAll(false);
    }
}
void AtomEditor::addAtom(Atom* a)
{
    // Updates fields based on a new atom being selected
    if (a->element != element) {element = -1; ui->type->setEnabled(false);}
    if (a->state != state) {state = -1; ui->state->setEnabled(false);}
    if (a->reactionStr != reaction) {reaction.clear(); ui->reaction->setEnabled(false);}
}

void AtomEditor::enableAll(bool enable)
{
    ui->type->setEnabled(enable);
    ui->state->setEnabled(enable);
    ui->reaction->setEnabled(enable);
    //ui->eReaction->setEnabled(enable);
}

bool AtomEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        if (obj == ui->type)
        {
            ui->type->setEnabled(true);
            changeType(ui->type->currentIndex());
        }
        else if (obj == ui->state)
        {
            ui->state->setEnabled(true);
            changeState(ui->state->value());
        }
        else if (obj == ui->reaction)
        {
            ui->reaction->setEnabled(true);
            changeReaction(ui->reaction->text());
        }
    }
    return false;// Continue event propogation
}

void AtomEditor::changeType(int type)
{
    int i = 0;
    int c = selected->count();
    while (i < c)
    {
        if (Globals::mw->docOpen)
        {
            Globals::mw->curDoc->sim->replaceAtom(i, selected->at(i), Globals::getAtom(type));
        }
        i++;
    }
}
void AtomEditor::changeState(int state)
{
    int i = 0;
    int c = selected->count();
    while (i < c)
    {
        selected->at(i)->changeState(state);
        i++;
    }
}
void AtomEditor::changeReaction(QString reactionStr)
{
    int reactionArr[18];
    if (!reactionStrToArr(reactionStr, reactionArr))
    {
        return;
    }
    int i = 0;
    int c = selected->count();
    while (i < c)
    {
        memcpy(selected->at(i)->reaction, reactionArr, sizeof(int) * 18);// Copy reactionArr to atom->reaction.
        selected->at(i)->reactionStr = reactionStr;
        i++;
    }
}


bool AtomEditor::reactionStrToArr(QString str, int (&arr)[18])
{
    str.remove(' ');// Remove spaces

    if (str.isEmpty()) {arr[0] = -5; return true;}
    /*
    if (reactionCache.contains(str))
    {
        return reactionCache.value(str);
    }
    */

    int strlen = str.length();
    if (strlen >= 64)
    {
        reactionParseError("Invalid reaction: must have less than 64 characters.");
        return false;
    }

    QRegExp rx("([a-z]{1,3})(?:([a-z]|[\\d]+)|\\(([a-z]|[\\d]+)([\\=\\>\\<\\+\\-\\*\\/])([a-z\\d]+)\\))([\\>\\+]?)");
    QList<QStringList> caps;
    int i = 0;
    int count = 0;
    do
    {
        i = rx.indexIn(str, i);
        QStringList r = rx.capturedTexts();
        caps.append(r);
        i += r.first().length();
        count++;
        strlen -= r.first().length();
    } while (i > -1 && count < 4);

    qDebug() << caps;

    // If strlen is not 0, there are extra characters not matched.

    if (count != 4)
    {
        reactionParseError("Invalid reaction syntax: Not enough reactants or products");
        return false;
    }
    if (strlen)
    {
        reactionParseError("Invalid reaction syntax: Extra characters");
        return false;
    }
    if (caps[1][6] != ">")
    {
        reactionParseError("Invalid reaction syntax: Missing or incorrectly placed '>' operator.");
        return false;
    }

    if (caps[0][1] != caps[2][1])
    {
        reactionParseError("Atom 1 type cannot change from "+caps[0][1]+" to "+caps[2][1]);
        return false;
    }
    else if (caps[1][1] != caps[3][1])
    {
        reactionParseError("Atom 2 type cannot change from "+caps[1][1]+" to "+caps[3][1]);
        return false;
    }

    QStringList ops = QStringList() << "=" << "!=" << "<" << ">" << "+" << "-" << "*" << "/";
    QStringList vKeys = QStringList() << "x" << "y" << "z";
    int vVals[3] = {-5, -5, -5};

    int v;

    i = 0;
    while (i < 2)
    {
        // Reactant
        QString element = caps[i][1];
        v = vKeys.indexOf(element);
        if (v > -1)
        {
            // Variable
            if (vVals[v] == -5)
            {
                vVals[v] = i - 4;
            }
            arr[i] = vVals[v];
        }
        else if ((v = Globals::elementSymbols.indexOf(element)) > -1)
        {
            // Element
            arr[i] = v;
        }
        else
        {
            reactionParseError("Invalid element/variable: "+element);
            return false;
        }
        QString simpleState = caps[i][2];
        if (simpleState.isEmpty())
        {
            // Complex state
        }
        else
        {
            // Simple state
            v = vKeys.indexOf(simpleState);
            if (v > -1)
            {
                // Variable
                if (vVals[v] == -5)
                {
                    // Var is not set
                    arr[2 + i] = -1;
                    arr[4 + i] = 0;
                    arr[6 + i] = 0;
                    arr[8 + i] = 0;
                    vVals[v] = i - 2;
                }
                else
                {
                    // Var is set
                    arr[2 + i] = 0;
                    arr[4 + i] = vVals[v];
                    arr[6 + i] = 0;
                    arr[8 + i] = 0;
                }
            }
            else if ((v = toInt(simpleState)) > -1)
            {
                // Int
                arr[2 + i] = 0;
                arr[4 + i] = v;
                arr[6 + i] = 0;
                arr[8 + i] = 0;
            }
            else
            {
                // Error
                reactionParseError("Invalid state/variable: "+simpleState);
                return false;
            }
        }
        i++;
    }
    while (i < 4)
    {
        // Product
        QString simpleState = caps[i][2];
        if (simpleState.isEmpty())
        {
            // Complex state
            QString q1 = caps[i][3];
            QString op = caps[i][4];
            QString q2 = caps[i][5];

            // Check for division by zero
            if (op == "/" && q2 == "0")
            {
                reactionParseError("Cannot divide "+q1+" by zero");
                return false;
            }

            // Input 1
            v = vKeys.indexOf(q1);
            if (v > -1)
            {
                // Variable
                if (vVals[v] == -5)
                {
                    reactionParseError("Variable "+q1+" is not set");
                    return false;
                }
                arr[10 + i] = vVals[v];
            }
            else if ((v = toInt(q1)) > -1)
            {
                arr[10 + i] = v;
            }
            else
            {
                reactionParseError("Invalid state/variable: "+q1);
                return false;
            }

            // Operator
            v = ops.indexOf(op);
            if (v < 4)
            {
                // Not found or is not an arithmetic operator
                reactionParseError("Invalid operator: "+op);
                return false;
            }
            arr[12 + i] = v;

            // Input 2
            v = vKeys.indexOf(q2);
            if (v > -1)
            {
                // Variable
                if (vVals[v] == -5)
                {
                    reactionParseError("Variable "+q2+" is not set");
                    return false;
                }
                arr[14 + i] = vVals[v];
            }
            else if ((v = toInt(q2)) > -1)
            {
                arr[14 + i] = v;
            }
            else
            {
                reactionParseError("Invalid state/variable: "+q2);
                return false;
            }

        }
        else
        {
            // Simple state
            v = vKeys.indexOf(simpleState);
            if (v > -1)
            {
                // Variable
                if (vVals[v] == -5)
                {
                    // Var is not set
                    reactionParseError("Variable "+simpleState+" is not set");
                    return false;
                }
                else
                {
                    // Var is set
                    arr[10 + i] = vVals[v];
                    arr[12 + i] = 0;
                    arr[14 + i] = 0;
                }
            }
            else if ((v = toInt(simpleState)) > -1)
            {
                // Int
                arr[10 + i] = v;
                arr[12 + i] = 0;
                arr[14 + i] = 0;
            }
            else
            {
                // Error
                reactionParseError("Invalid state/variable: "+simpleState);
                return false;
            }
        }
        i++;
    }

    if (caps[0][6] != "" && caps[0][6] != "+")
    {
        reactionParseError("Invalid reaction syntax: "+caps[0][6]);
        return false;
    }
    if (caps[2][6] != "" && caps[2][6] != "+")
    {
        reactionParseError("Invalid reaction syntax: "+caps[2][6]);
        return false;
    }
    arr[10] = caps[0][6].isEmpty();// Match bonded?
    arr[11] = caps[2][6].isEmpty();// Product bonded?

    qDebug() << str;
    qDebug() << caps;
    i = 0;
    while (i < 18){qDebug() << arr[i++];}

    qDebug() << reactionArrToEnglish(arr);

    //reactionCache.insert(str, arr);
    reactionParseSuccess();
    return true;
}

QString AtomEditor::reactionArrToEnglish(int arr[18])
{
    // The items of arr							Replace Negatives with their values
    // arr[0] = a1 type must match              Yes
    // arr[1] = a2 type must match              Yes
    // arr[2] = a1 state comparison             No (=, !=, <, >)
    // arr[3] = a2 state comparison             No (=, !=, <, >)
    // arr[4] = a1 op1                          Yes
    // arr[5] = a2 op1                          Yes
    // arr[6] = a1 operand                      No (+, -, *, /)
    // arr[7] = a2 operand                      No (+, -, *, /)
    // arr[8] = a1 op2                          Yes
    // arr[9] = a2 op2                  		Yes
    // arr[10] = atom bond must match			No (0, 1)
    // arr[11] = atom bond is set to			No (0, 1)
    // arr[12] = a1 state is set to op1     	Yes
    // arr[13] = a2 state is set to op1     	Yes
    // arr[14] = a1 state is set to operator	No (+, -, *, /)
    // arr[15] = a2 state is set to operator	No (+, -, *, /)
    // arr[16] = a1 state is set to op2         Yes
    // arr[17] = a2 state is set to op2         Yes

    // As you can see, it just so happens that negatives are replaced in a pattern (Yes, Yes, No, No, Yes, Yes, No, No, ...)

    // -4 = a1 type
    // -3 = a2 type
    // -2 = a1 state
    // -1 = a2 state

    /*
    QStringList negStrs = QStringList() << "this atom's type" << "the other atom's type" << "this atom's state" << "the other atom's state";
    QStringList opsStrs = QStringList() << "equal to" << "not equal to" << "less than" << "greater than" << "plus" << "minus" << "multiplied by" << "divided by";

    QStringList conds = QStringList() << "" << "" << "" << "";

    switch (arr[0])
    {
    case -4: break;
    case -3: conds[0] = "both atoms are the same element"; break;
    case -2:
    case -1: conds[0] = "this atom's type is equal to " + negStrs[arr[0] + 4];
    }

    if (arr[0] < 0)
    {
        if (arr[0] == -3)
        {
            conds[0] = "both atoms are the same element";
        }
        else if (arr[0] == -2 || arr[0] == -1)
        {
            conds[0] = "this atom's type is equal to " + negStrs[arr[0] + 4];
        }
    }
    else
    {
        conds[0] = "a " + Globals::getElementName(arr[0]);
    }
    if (arr[2] )
    */

    return QString();
}

int AtomEditor::toInt(QString str)
{
    bool ok;
    int i = str.toInt(&ok);
    if (!ok || i > 65535 || i < 0) {return -1;}
    else {return i;}
}

void AtomEditor::reactionParseSuccess()
{
    ui->reactionError->setVisible(false);
}
void AtomEditor::reactionParseError(QString error)
{
    qDebug() << error;
    ui->reactionError->setToolTip(error);
    ui->reactionError->setVisible(true);
}

AtomEditor::~AtomEditor()
{
    delete ui;
}
