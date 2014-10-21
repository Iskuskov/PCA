#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class PaintArea;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    PaintArea *paintArea;

    QMenu *optionMenu;
    QAction *clearScreenAct;
    QAction *testPrincompAct;

public slots:
    void testPrincomp();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
