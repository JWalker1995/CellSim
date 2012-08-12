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

    LogBlock b = Globals::log->scopedBlock("Atom editor initializing");

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QSettings settings;
    settings.beginGroup("AtomEditor");
    restoreGeometry(settings.value("geometry").toByteArray());
    settings.endGroup();

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

AtomEditor::~AtomEditor()
{
    QSettings settings;
    settings.beginGroup("AtomEditor");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();

    delete ui;
}

void AtomEditor::updateAdd()
{
    // Called after a single atom is selected.
    if (sim->selected.count() == 1)
    {
        // There is only one atom selected. Populate fields to match it.
        Atom* a = sim->selected.first();
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
        addAtom(sim->selected.last());
    }
}

void AtomEditor::updateRemove()
{
    // Called after any number of atoms are unselected.
    int c = sim->selected.count();
    if (c)
    {
        // Populate fields with first atom
        Atom* a = sim->selected.first();
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
            addAtom(sim->selected.at(i));
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
    if (!sim) {return false;}
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
    int c = sim->selected.count();
    while (i < c)
    {
        sim->replaceAtom(i, sim->selected.at(i), Globals::getAtom(type));
        i++;
    }
}
void AtomEditor::changeState(int state)
{
    int i = 0;
    int c = sim->selected.count();
    while (i < c)
    {
        sim->selected.at(i)->changeState(state);
        i++;
    }
}
void AtomEditor::changeReaction(QString reactionStr)
{
    int reactionArr[18];
    QString error = reactionStrToArr(reactionStr, reactionArr);
    if (error.isEmpty())
    {
        ui->reactionError->setVisible(false);
    }
    else
    {
        ui->reactionError->setToolTip(error);
        ui->reactionError->setVisible(true);
        return;
    }

    int i = 0;
    int c = sim->selected.count();
    while (i < c)
    {
        memcpy(sim->selected.at(i)->reaction, reactionArr, sizeof(int) * 18);// Copy reactionArr to atom->reaction.
        sim->selected.at(i)->reactionStr = reactionStr;
        i++;
    }
}


QString AtomEditor::reactionStrToArr(QString str, int (&arr)[18])
{
    str.remove(' ');// Remove spaces

    if (str.isEmpty()) {arr[0] = -5; return QString();}
    /*
    if (reactionCache.contains(str))
    {
        return reactionCache.value(str);
    }
    */

    int strlen = str.length();
    if (strlen >= 64)
    {
        return QString("Invalid reaction: must have less than 64 characters.");
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

    // If strlen is not 0, there are extra characters not matched.

    if (count != 4)
    {
        return QString("Invalid reaction syntax: Not enough reactants or products");
    }
    if (strlen)
    {
        return QString("Invalid reaction syntax: Extra characters");
    }
    if (caps[1][6] != ">")
    {
        return QString("Invalid reaction syntax: Missing or incorrectly placed '>' operator.");
    }

    if (caps[0][1] != caps[2][1])
    {
        return QString("Atom 1 type cannot change from "+caps[0][1]+" to "+caps[2][1]);
    }
    else if (caps[1][1] != caps[3][1])
    {
        return QString("Atom 2 type cannot change from "+caps[1][1]+" to "+caps[3][1]);
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
            return QString("Invalid element/variable: "+element);
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
                    arr[2 + i] = -1;// Anything
                    arr[4 + i] = 0;
                    arr[6 + i] = 0;
                    arr[8 + i] = 0;
                    vVals[v] = i - 2;
                }
                else
                {
                    // Var is set
                    arr[2 + i] = 0;// Equals
                    arr[4 + i] = vVals[v];// Var
                    arr[6 + i] = 0;// Plus
                    arr[8 + i] = 0;// Zero
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
                return QString("Invalid state/variable: "+simpleState);
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
                return QString("Cannot divide "+q1+" by zero");
            }

            // Input 1
            v = vKeys.indexOf(q1);
            if (v > -1)
            {
                // Variable
                if (vVals[v] == -5)
                {
                    return QString("Variable "+q1+" is not set");
                }
                arr[10 + i] = vVals[v];
            }
            else if ((v = toInt(q1)) > -1)
            {
                arr[10 + i] = v;
            }
            else
            {
                return QString("Invalid state/variable: "+q1);
            }

            // Operator
            v = ops.indexOf(op);
            if (v < 4)
            {
                // Not found or is not an arithmetic operator
                return QString("Invalid operator: "+op);
            }
            arr[12 + i] = v;

            // Input 2
            v = vKeys.indexOf(q2);
            if (v > -1)
            {
                // Variable
                if (vVals[v] == -5)
                {
                    return QString("Variable "+q2+" is not set");
                }
                arr[14 + i] = vVals[v];
            }
            else if ((v = toInt(q2)) > -1)
            {
                arr[14 + i] = v;
            }
            else
            {
                return QString("Invalid state/variable: "+q2);
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
                    return QString("Variable "+simpleState+" is not set");
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
                return QString("Invalid state/variable: "+simpleState);
            }
        }
        i++;
    }

    if (caps[0][6] != "" && caps[0][6] != "+")
    {
        return QString("Invalid reaction syntax: "+caps[0][6]);
    }
    if (caps[2][6] != "" && caps[2][6] != "+")
    {
        return QString("Invalid reaction syntax: "+caps[2][6]);
    }
    arr[10] = caps[0][6].isEmpty();// Match bonded?
    arr[11] = caps[2][6].isEmpty();// Product bonded?

    //reactionCache.insert(str, arr);
    return QString();
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
