#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <QDebug>

#include "paintarea.h"
#include "princompview.h"
#include "princompscene.h"

#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Область рисования
//    paintArea = new PCA::PaintArea;
//    setCentralWidget(paintArea);

//    view = new pca::PrincompView();
//    scene = new QGraphicsScene(0, 0, 500, 500);
//    setCentralWidget(view);
//    view->setScene(scene);
//    view->show();


    view = new QGraphicsView;
    view->setRenderHint(QPainter::Antialiasing, true);
    scene = new pca::PrincompScene;
    setCentralWidget(view);
    view->setScene(scene);
    view->show();

    // Действия
    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
//    connect(clearScreenAct, SIGNAL(triggered()), paintArea, SLOT(clearImage()));
    connect(clearScreenAct, SIGNAL(triggered()), scene, SLOT(clear()));

    testPrincompAct = new QAction(tr("&Test princomp"), this);
    testPrincompAct->setShortcut(tr("Ctrl+T"));
    connect(testPrincompAct, SIGNAL(triggered()), this, SLOT(testPrincomp()));

    // Меню
    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(clearScreenAct);
    optionMenu->addAction(testPrincompAct);
    menuBar()->addMenu(optionMenu);

    // Обработчики
    //connect(ui->btnClear, SIGNAL(clicked()), paintArea, SLOT(clearImage()));
    //connect(ui->btnPCA,   SIGNAL(clicked()), paintArea, SLOT(buildPCA(new QPainter(paintArea->theImage))));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testPrincomp()
{
    vector< vector<double> > data;
    double x[] = {37, 84, 39,  7, 92, 72, 22, 68, 83, 54};
    double y[] = {42, 77, 74, 61, 91,  2, 10, 93, 70, 36};

    vector<double> vec_x;
    vector<double> vec_y;
    for (int i = 0; i < 10; ++i)
    {
        vec_x.push_back(x[i]);
        vec_y.push_back(y[i]);
    }
    data.push_back(vec_x);
    data.push_back(vec_y);
}
