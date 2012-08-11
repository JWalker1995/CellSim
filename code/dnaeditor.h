#ifndef DNAEDITOR_H
#define DNAEDITOR_H

#include <QDialog>

namespace Ui {
class DnaEditor;
}

class DnaEditor : public QDialog
{
    Q_OBJECT
    
public:
    explicit DnaEditor(QWidget *parent = 0);
    ~DnaEditor();

    QStringList reactionArrToDna(const int arr[18]);
    int compressNum(int num);
    QString stateToDna(long long &dna, const int *arr, bool comp);
    
private:
    Ui::DnaEditor *ui;

    void changeReaction(int row);

private slots:
    void on_dnaTable_cellChanged(int row, int col);
};

#endif // DNAEDITOR_H
