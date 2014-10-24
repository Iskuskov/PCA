#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <QDebug>
#include <QGraphicsView>

#include "princompscene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(trUtf8("Метод главных компонент для 2D точек"));

    // Область рисования
    m_view = new QGraphicsView;
    m_view->setRenderHint(QPainter::Antialiasing, true);
    setCentralWidget(m_view);
    m_scene = new pca::PrincompScene;
    m_view->setScene(m_scene);
    m_view->show();

    // Действия
    m_clearScreenAct = new QAction(trUtf8("&Очистить сцену"), this);
    m_clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(m_clearScreenAct, SIGNAL(triggered()), m_scene, SLOT(clearScene()));

    // Меню
    m_optionMenu = new QMenu(trUtf8("&Опции"), this);
    m_optionMenu->addAction(m_clearScreenAct);
    menuBar()->addMenu(m_optionMenu);

    // Обработчики
    //connect(ui->btnClear, SIGNAL(clicked()), m_scene, SLOT(clear()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
