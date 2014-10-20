#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "paintarea.h"

using namespace std;

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

    QAction *buildPcaAct;
    QAction *clearScreenAct;

    QAction *testPrincompAct;

public slots:
    void testPrincomp();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
