#include "paintarea.h"

#include <math.h>
#include <QtGui>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "princomp.h"

namespace pca {

PaintArea::PaintArea(QWidget *parent)
    : QWidget(parent),
      theImage(500, 500, QImage::Format_RGB32),

      means(PrinComp::dimension),
      covarianceMatrix(PrinComp::dimension, vector<double>(PrinComp::dimension)),
      eigenValues(PrinComp::dimension),
      eigenVectors(PrinComp::dimension, vector<double>(PrinComp::dimension))
{
//    scene->addItem(pixmapItem);
//    set

//    view->setScene(scene);

    clearImage();
}

void PaintArea::clearImage()
{
    clickPoints.clear();
    theImage.fill(qRgb(255, 255, 255));
    update();
}

void PaintArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawImage(QPoint(0, 0), theImage);

    // Перерасчитываем главные компоненты
    if (clickPoints.size() == 0)
        return;

    // Вычисляем необходимые данные
    PrinComp prinComp;
//    prinComp.computeMeans(clickPoints, means);
//    prinComp.computeCovarianceMatrix(clickPoints, means, covarianceMatrix);
//    prinComp.computeEigenValues(covarianceMatrix, eigenValues);
//    prinComp.computeEigenVectors(covarianceMatrix, eigenValues, eigenVectors);
//    prinComp.computePCAData(clickPoints, means, covarianceMatrix, eigenValues, eigenVectors);

    // Вычисляем центральную точку
    QPointF meanPoint = prinComp.computeMeanPoint(clickPoints);

    // Вычисляем главные компоненты (первую и вторую)
    vector<QLineF> PCAVectors = prinComp.computePCA(clickPoints);

    QLineF firstPrincipalComponent  = PCAVectors[0];
    QLineF secondPrincipalComponent = PCAVectors[1];

    // 1. Отображаем точки
    painter.save();
    painter.setBrush(Qt::red);
    painter.setPen(Qt::red);
    foreach (QPointF point, clickPoints)
    {
        painter.drawPoint(point);
        painter.drawEllipse(point, 5, 5);
    }
    painter.restore();

    // 2. Отображаем центр
    painter.setPen(QPen(QBrush(Qt::magenta), 5));
    painter.drawEllipse(meanPoint, 10, 10);

    // 3. Отображаем главные компоненты
    painter.setPen(QPen(QBrush(Qt::darkBlue), 5));
    painter.drawLine(firstPrincipalComponent);

    painter.setPen(QPen(QBrush(Qt::blue), 5));
    painter.drawLine(secondPrincipalComponent);

    // 4. Отображаем эллипс
    painter.save();
    // Translate
    painter.translate(meanPoint);

    // Rotate
    qreal a = atan(firstPrincipalComponent.dy() / firstPrincipalComponent.dx()) * 180 / M_PI;
    painter.rotate(a);

    painter.setPen(QPen(QBrush(Qt::green), 5));
    painter.setBrush(QBrush(Qt::transparent));
    painter.drawEllipse(QPointF(0, 0), firstPrincipalComponent.length() / 2, secondPrincipalComponent.length() / 2);
    painter.restore();
}

void PaintArea::mousePressEvent(QMouseEvent *e)
{
    // Тестовые данные
//    double x[] = {37, 84, 39,  7, 92, 72, 22, 68, 83, 54};
//    double y[] = {42, 77, 74, 61, 91,  2, 10, 93, 70, 36};

//    QPointF point;
//    for (int i = 0; i < 10; ++i) { point.setX(x[i]); point.setY(y[i]); clickPoints.push_back(point); }

    // Добавляем точку
    QPointF newClickPoint = e->pos();
    clickPoints.push_back(newClickPoint);

    update();
}

/* --------------------------------------------------------- */

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

} // namespace
