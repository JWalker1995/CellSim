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
    
private:
    Ui::MainWindow *ui;

    QLabel* atoms;
    QLabel* bonds;

    Document* curDoc;
    bool docOpen;


public slots:
    void playPauseSim(bool run);
    void setFrameSpeed(bool speedSlow);

    void changeDoc(int i);
    void closeDoc(int i);

    void updateStatus();

private slots:
    void on_actionSave_triggered();
};

#endif // MAINWINDOW_H
