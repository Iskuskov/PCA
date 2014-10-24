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

private:
    Ui::MainWindow *ui;

    QMenu   *m_optionMenu;
    QAction *m_clearScreenAct;

    QGraphicsView *m_view;
    pca::PrincompScene *m_scene;
};

#endif // MAINWINDOW_H
