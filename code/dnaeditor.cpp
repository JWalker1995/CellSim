#include "dnaeditor.h"
#include "ui_dnaeditor.h"

#include "globals.h"

#include <QDebug>

DnaEditor::DnaEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DnaEditor)
{
    ui->setupUi(this);
}

DnaEditor::~DnaEditor()
{
    delete ui;
}

void DnaEditor::on_dnaTable_cellChanged(int row, int col)
{
    switch (col)
    {
    case 0:
        // Reaction
        changeReaction(row);
        break;
    case 1:
        // Introns

        break;
    case 2:
        // Exons

        break;
    }
}

void DnaEditor::changeReaction(int row)
{
    QTableWidgetItem* item = ui->dnaTable->item(row, 0);
    int reactionArr[18];
    QString error = Globals::ae->reactionStrToArr(item->text(), reactionArr);
    if (error.isEmpty())
    {
        item->setForeground(QBrush());
        QStringList dna = reactionArrToDna(reactionArr);
        if (dna.length() == 4)
        {
            item->setData(50, dna);
        }
        else
        {
            item->setForeground(QBrush(Qt::red));
            qDebug() << dna.first();
        }
    }
    else
    {
        item->setForeground(QBrush(Qt::red));
    }
}

QStringList DnaEditor::reactionArrToDna(const int arr[18])
{
    /*
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

    /*
    Reactant 1:
        Type - 25
        State comparison - 4
        State - (3 + x)

    Reactant 2:
        Type - 25
        State comparison - 4
        Op 1 - 3
        Operand - 7 (+, -, *, /, - flipped, / flipped, use op2)
        Op 2 - (3 + x)

    Reactant bond - 2

    Product 1:
        Op 1 - 3
        Operand - 7 (+, -, *, /, - flipped, / flipped, use op2)
        Op 2 - (3 + x)

    Product 2:
        Op 1 - 3
        Operand - 7 (+, -, *, /, - flipped, / flipped, use op2)
        Op 2 - (3 + x)

    Product bond - 2

    Copying - 4

    */
    int vars[4] = {0,0,0,0};

    long long dna = 0;
    dna = dna * 25 + arr[0];
    dna = dna * 25 + arr[1];
    dna = dna * 4 + arr[2];
    dna = dna * 4 + arr[3];

    int in1 = arr[4];
    int op = arr[6] - 4;
    int in2 = arr[8];
    if (in1 >= 0)
    {
        // First input is a number
        if (in2 >= 0)
        {
            // Second input is also a number
            switch (op)
            {
            case 0:
                // Addition
                in1 += in2;
                break;
            case 1:
                // Subtraction
                in1 -= in2;
                break;
            case 2:
                // Multiplication
                in1 *= in2;
                break;
            case 3:
                // Division
                in1 /= in2;
                break;
            }

            return QStringList() << "Both operands of a complex state cannot be a number.";
        }
        else
        {
            // If the second input is not a number, switch the inputs around
            int z = in2;
            in2 = in1;
            in1 = z;

            if (op == 1) {op = 4;} // Subtraction
            else if (op == 3) {op = 5;} // Division
        }
    }

    dna = dna * 4 + arr[2];
    dna = dna * 4 + arr[3];


    dna = dna *
*/
    return QStringList();
    // (25^2 * 4^3 * 4^2 * 7^4 * 180^4 * 4 * 4)^(1/4)
}

int DnaEditor::compressNum(int num)
{
    // Return range: 3 <= x <= 192
    int i = 0;
    while (i < 190)
    {
        if (Globals::compNums[i] == num)
        {
            return i + 3;
        }
        else if (Globals::compNums[i] > num)
        {
            break;
        }
        i++;
    }
    return -i;
}

QString DnaEditor::stateToDna(long long& dna, const int *arr, bool comp)
{
    if (comp)
    {
        // Include state comparison

    }

    return QString();
}
