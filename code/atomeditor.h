#ifndef ATOMEDITOR_H
#define ATOMEDITOR_H

#include "atom.h"
#include "simulation.h"

#include <QDialog>
#include <QList>
#include <QLabel>
#include <QHash>

class Simulation;

namespace Ui {
class AtomEditor;
}

class AtomEditor : public QDialog
{
    Q_OBJECT
    
public:
    explicit AtomEditor(QWidget *parent = 0);
    ~AtomEditor();

    void updateAdd();
    void updateRemove();

    bool reactionStrToArr(QString str, int (&arr)[18]);
    QString reactionArrToEnglish(int arr[18]);

    Simulation* sim;
    
protected:
    Ui::AtomEditor *ui;

    int prevElement;
    int prevState;
    QString prevReaction;
    int element;
    int state;
    QString reaction;

    void addAtom(Atom* a);

    QHash<QString, int*> reactionCache;
    int toInt(QString str);

    void enableAll(bool enable);

    bool eventFilter(QObject *o, QEvent *e);

    void reactionParseSuccess();
    void reactionParseError(QString error);

private slots:
    void changeType(int type);
    void changeState(int state);
    void changeReaction(QString reaction);

};

#endif // ATOMEDITOR_H
