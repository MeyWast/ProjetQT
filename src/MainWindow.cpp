#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    this->mainScene = new MyScene;
    //this->setBackground(QBrush(QPixmap("source/Nouveau Dossier/beta.png")));
    this->mainView = new QGraphicsView;
    this->mainView->setScene(mainScene);
    mainView->resize(4000, 2400);
    mainView->setWindowTitle("View personnage");
    mainView->show();
    //this->setCentralWidget(mainView);
    this->setWindowTitle("Gaming");

    //helpMenu = menuBar()->addMenu(tr("&Help"));
    //QAction* actionHelp = new QAction(tr("&About"), this);
    //connect(actionHelp, SIGNAL(triggered()), this, SLOT(slot_aboutMenu()));
    //helpMenu->addAction(actionHelp);

    this->PlayerView = new QGraphicsView;
    this->PlayerView->setScene(mainScene);
    this->PlayerView->setFixedSize(500,500);
    //this->PlayerView->show();
}

MainWindow::~MainWindow(){

}

void MainWindow::slot_aboutMenu(){
    QMessageBox msgBox;
    msgBox.setText("A small QT/C++ projet...");
    msgBox.setModal(true); // on souhaite que la fenetre soit modale i.e qu'on ne puisse plus cliquer ailleurs
    msgBox.exec();
}