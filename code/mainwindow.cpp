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
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("JoelSoft");
    QCoreApplication::setApplicationName("CellSim");

    Globals::log = new Logger(this);

    LogBlock b = Globals::log->scopedBlock("CellSim initializing");

    ui->setupUi(this);

    connect(ui->actionPlayPause, SIGNAL(triggered(bool)), this, SLOT(playPauseSim(bool)));

    connect(ui->documents, SIGNAL(tabCloseRequested(int)), this, SLOT(closeDoc(int)));
    connect(ui->documents, SIGNAL(currentChanged(int)), this, SLOT(changeDoc(int)));

    Globals::mw = this;

    Globals::init();
    Globals::ae = new AtomEditor(this);
    Globals::ae->show();

    Globals::de = new DnaEditor(this);
    Globals::de->show();

    QSettings settings;
    settings.beginGroup("MainWindow");
    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());

    QStringList docs = settings.value("documents", QStringList() << "").toStringList();
    int i = 0;
    int c = docs.length();
    while (i < c)
    {
        openDoc(docs.at(i));
        i++;
    }

    settings.endGroup();

    //Globals::ae->reactionStrToArr(QString("ax + b(x+1) > a(x+2) b(x+3)"));

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

void MainWindow::setDocTitle(int i, QString title)
{
    ui->documents->setTabText(i, title);
}

void MainWindow::openDoc(QString path)
{
    Document* doc = new Document(this);
    if (!path.isEmpty())
    {
        doc->openFile(path);
    }
    doc->i = ui->documents->addTab(doc, "Untitled*");
}

void MainWindow::changeDoc(int i)
{
    if (i == -1)
    {
        docOpen = false;
        Globals::ae->sim = 0;
    }
    else
    {
        curDoc = static_cast<Document*>(ui->documents->widget(i));
        docOpen = true;
        Globals::ae->sim = curDoc->sim;
    }
}
void MainWindow::closeDoc(int i)
{
    Document* doc = static_cast<Document*>(ui->documents->widget(i));
    if (doc->confirmClose())
    {
        ui->documents->removeTab(i);
        delete doc;
    }
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    QStringList docs;
    int i = 0;
    int c = ui->documents->count();
    while (i < c)
    {
        // Loop through each document open
        Document* doc = static_cast<Document*>(ui->documents->widget(i));
        if (doc->confirmClose())// Display "Do you want to save?" dialog, and return if user wants to continue closing.
        {
            docs.append(doc->bridge.fileName());
        }
        else
        {
            e->ignore();
            return;
        }
        i++;
    }

    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("documents", docs);
    settings.endGroup();

    e->accept();
}

void MainWindow::on_actionNew_triggered()
{
    Document* doc = new Document(this);
    doc->i = ui->documents->addTab(doc, "Untitled*");
}

void MainWindow::on_actionOpen_triggered()
{
    if (docOpen) {curDoc->open();}
}

void MainWindow::on_actionSave_triggered()
{
    if (docOpen) {curDoc->save();}
}
