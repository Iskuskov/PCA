#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace pca {
    class PaintArea;
    class PrincompScene;
}

namespace Ui {
    class MainWindow;
}

class QGraphicsView;
class QGraphicsScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    pca::PaintArea *paintArea;

    QMenu *optionMenu;
    QAction *clearScreenAct;
    QAction *testPrincompAct;

public slots:
    void testPrincomp();

private:
    Ui::MainWindow *ui;

private:
    QGraphicsView  *view;
    //QGraphicsScene *scene;
    pca::PrincompScene *scene;
};

#endif // MAINWINDOW_H
