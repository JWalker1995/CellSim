#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "simulation.h"
#include "atomeditor.h"
#include "document.h"
#include "globals.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionPlayPause, SIGNAL(triggered(bool)), this, SLOT(playPauseSim(bool)));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));

    connect(ui->documents, SIGNAL(tabCloseRequested(int)), this, SLOT(closeDoc(int)));
    connect(ui->documents, SIGNAL(currentChanged(int)), this, SLOT(changeDoc(int)));

    Globals::init();
    Globals::ae = new AtomEditor(this);
    Globals::ae->show();


    //Globals::ae->reactionStrToArr(QString("ax + b(x+1) > a(x+2) b(x+3)"));

    Document* doc = new Document();

    //setCentralWidget(view);
    ui->documents->addTab(doc, "Untitled*");

    //view->setDragMode(QGraphicsView::RubberBandDrag);

    /*
    simTimer=new QTimer(this);
    connect(simTimer, SIGNAL(timeout()), sim, SLOT(fastFrame()));
    */

    /*
    atoms = new QLabel();
    bonds = new QLabel();
    statusBar()->addPermanentWidget(atoms);
    statusBar()->addPermanentWidget(bonds);
    connect(simTimer, SIGNAL(timeout()), this, SLOT(updateStatus()));
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFrameSpeed(bool speedSlow)
{
    /*
    if (speedSlow && !sim->frameSpeedSlow)
    {
        // Set slow speed
        disconnect(simTimer, SIGNAL(timeout()), sim, SLOT(fastFrame()));
        connect(simTimer, SIGNAL(timeout()), sim, SLOT(slowFrame()));
        sim->frameSpeedSlow=true;
    }
    else if (!speedSlow && sim->frameSpeedSlow)
    {
        // Set fast speed
        disconnect(simTimer, SIGNAL(timeout()), sim, SLOT(slowFrame()));
        connect(simTimer, SIGNAL(timeout()), sim, SLOT(fastFrame()));
        sim->frameSpeedSlow=false;
    }
    */
}

void MainWindow::playPauseSim(bool run)
{
    if (docOpen) { curDoc->sim->playPause(run); }
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if (e->modifiers() == Qt::ControlModifier)
    {
        switch (e->key())
        {
        case Qt::Key_A:
            if (docOpen) { curDoc->sim->selectAll(); }
            break;
        }
    }
}
void MainWindow::updateStatus()
{
    /*
    atoms->setText("Atoms: "+QString().setNum(sim->numAtoms)+" / "+QString().setNum(sim->numAtomsAlloc));
    bonds->setText("Bonds: "+QString().setNum(sim->numBonds)+" / "+QString().setNum(sim->numBondsAlloc));
    */
}

void MainWindow::changeDoc(int i)
{
    if (i == -1)
    {
        docOpen = false;
    }
    else
    {
        curDoc = static_cast<Document*>(ui->documents->widget(i));
        docOpen = true;
    }
}
void MainWindow::closeDoc(int i)
{
    ui->documents->removeTab(i);
}

void MainWindow::on_actionSave_triggered()
{
    if (docOpen) { curDoc->save(); }
}