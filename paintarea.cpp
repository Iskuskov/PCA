#include <QtGui>

#include "paintarea.h"
#include <stdlib.h>
#include <math.h>

PaintArea::PaintArea(QWidget *parent)
    : QWidget(parent),
      theImage(500, 500, QImage::Format_RGB32)
{
    clearImage();
}

void PaintArea::clearImage()
{
    clickPoints.clear();
    theImage.fill(qRgb(255, 255, 255));
    update();
}

void PaintArea::buildPCA()//QPainter* painter)
{
    QPainter* painter = new QPainter(&theImage);

    // Очищаем старый центр
    painter->setPen(QPen(QBrush(Qt::white), 10));
    painter->drawEllipse(means[0], means[1], 10, 10);

    // Ковариационная матрица + центр облака
    computeCovarianceMatrix();

    // Вычисляем собственные значения и векторы
    computeEigen();

    // Отмечаем центр облака
    painter->setPen(QPen(QBrush(Qt::magenta), 5));
    painter->drawEllipse(means[0], means[1], 10, 10);

    // Получаем главную компоненту и перпендикуляр к ней



    QLineF PCA(getPCA(clickPoints));
    QLineF ortoLine(getNormalLine(PCA));

    // Отобржаем полученные прямые


    painter->setPen(QPen(QBrush(Qt::darkBlue), 5));
    painter->drawLine(PCA);

//    painter->setPen(QPen(QBrush(Qt::blue), 5));
//    painter->drawLine(ortoLine);

    computeCovarianceMatrix();

    update();
}

// Метод главных компонент (возвращает главную компоненту)
QLineF PaintArea::getPCA(vector<QPointF> points)
{
    // k - наклон, l - длина (половина)
    // x, y - соответсвующие проекции l
    double k, l, dx, dy;

    k = (eigenValues[0] - covarianceMatrix[0][0]) / covarianceMatrix[1][0];
    l = eigenValues[0] / 2;

    dy = l * sqrt( 1 / (1 + k * k) );
    dx = dy * k;

    QPointF start (means[0] - dx, means[1] - dy);
    QPointF finish(means[0] + dx, means[1] + dy);

    QLineF PCALine(start, finish);
    //QLineF PCALine(means[0], means[1], 250, 250);
    return PCALine;
}

// Пернедикулярная к прямой
QLineF PaintArea::getNormalLine(QLineF line)
{
    QLineF normalLine = line.normalVector();
    return normalLine;
}

void PaintArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), theImage);
}

void PaintArea::resizeEvent(QResizeEvent *event)
{
    if (width() > theImage.width() || height() > theImage.height())
    {
        int newWidth = qMax(width() + 128, theImage.width());
        int newHeight = qMax(height() + 128, theImage.height());
        resizeImage(&theImage, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void PaintArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void PaintArea::mousePressEvent(QMouseEvent *e)
{
    QPointF newClickPoint = e->pos();
    clickPoints.push_back(newClickPoint);

    // Отображаем точки
    QPainter painter(&theImage);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::red);
    foreach (QPointF point, clickPoints)
    {
        painter.drawPoint(point);
        painter.drawEllipse(point, 5, 5);
    }

    update();

    //buildPCA(&painter);
}

void PaintArea::computeCovarianceMatrix()
{
    means[0] = 0, means[1] = 0;

    for (int i = 0; i < clickPoints.size(); i++)
        means[0] += clickPoints[i].x(),
        means[1] += clickPoints[i].y();

    means[0] /= clickPoints.size(),
    means[1] /= clickPoints.size();

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            covarianceMatrix[i][j] = 0.0;
            for (int k = 0; k < clickPoints.size(); k++)
            {
                double coord_i = (i == 0) ? clickPoints[k].x() : clickPoints[k].y();
                double coord_j = (j == 0) ? clickPoints[k].x() : clickPoints[k].y();

                covarianceMatrix[i][j] += (means[i] - coord_i) * (means[j] - coord_j);
            }
            covarianceMatrix[i][j] /= clickPoints.size() - 1;
        }
    }
}

void PaintArea::computeEigen()
{
    eigenValues[0] = 0, eigenValues[1] = 0;

    double a = covarianceMatrix[0][0];
    double b = covarianceMatrix[1][0]; // = covarianceMatrix[0][1];
    double d = covarianceMatrix[1][1];

    eigenValues[0] = ( (a + d) + sqrt( pow((a - d), 2) + (4 * pow(b, 2) ) ) ) / 2;
    eigenValues[1] = ( (a + d) - sqrt( pow((a - d), 2) + (4 * pow(b, 2) ) ) ) / 2;
}
