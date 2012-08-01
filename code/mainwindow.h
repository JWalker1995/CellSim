#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "simulation.h"
#include "atomeditor.h"
#include "document.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtGui>
#include <QTimer>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* e);

    Document* curDoc;
    bool docOpen;
    
private:
    Ui::MainWindow *ui;

    QLabel* atoms;
    QLabel* bonds;

public slots:
    void playPauseSim(bool run);
    void setFrameSpeed(bool speedSlow);

    void newDoc();
    void openDoc(QString path);
    void changeDoc(int i);
    void closeDoc(int i);

    void updateStatus();

private slots:
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H
