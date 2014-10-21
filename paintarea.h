#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <vector>

using namespace std;

class PaintArea : public QWidget
{
    Q_OBJECT

public:
    PaintArea(QWidget *parent = 0);

    // Метод главных компонент (возвращает главную компоненту)
    QLineF getPCA(vector<QPointF> points);

    // Пернедикулярная к прямой
    QLineF getNormalLine(QLineF line);

    void computeCovarianceMatrix();
    void computeEigen();

public slots:
    void clearImage();
    void buildPCA(QPainter* painter);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void mousePressEvent(QMouseEvent *e);

private:
    void resizeImage(QImage *image, const QSize &newSize);

    QImage theImage;

    vector<QPointF> clickPoints;

    vector<double> means;
    vector< vector<double> > covarianceMatrix;
    vector<double> eigenValues;
};

#endif
