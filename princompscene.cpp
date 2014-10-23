#include "princompscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <math.h>
#include "princomp.h"

namespace pca {

PrincompScene::PrincompScene(QObject *parent) :
    QGraphicsScene(parent)
{
    connect(this, SIGNAL(changed(QList<QRectF>)), this, SLOT(addPrincomp()));
}

void PrincompScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        QGraphicsItem * item = itemAt(mouseEvent->scenePos());

        if ((!item) || (item && item->data(0) != simplePoint))
            placePoint(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), simplePoint);
        else if (item && item->data(0) == simplePoint)
            QGraphicsScene::mousePressEvent(mouseEvent);
    }

    if (mouseEvent->button() == Qt::RightButton)
    {
        QGraphicsItem * item = itemAt(mouseEvent->scenePos());
        if (item && item->data(0) == simplePoint)
            removeItem(item);
    }
}

void PrincompScene::placePoint(qreal x, qreal y, itemTypes type)
{
    int diameter = 20;

    QPen pen(Qt::black);
    QBrush brush(Qt::red);
    if (type == centerPoint)
        brush = Qt::yellow;

    QGraphicsEllipseItem *point = addEllipse(0, 0, diameter, diameter, pen, brush);
    point->setPos(x, y);//(x - diameter / 2, y - diameter / 2);
    point->setData(0, type);

    if (type == simplePoint)
    {
        point->setFlags(point->flags() | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
                        QGraphicsItem::ItemIsFocusable );
        point->setAcceptDrops(true);
        point->setZValue(+10);      // Хак для отображения точек поверх остального

        addPrincomp();
    }
}

void PrincompScene::placeLine(QLineF line, itemTypes type)
{
    int width = 5;
    QPen pen(QBrush(Qt::darkBlue), width);
    if (type == secondPrincipal)
        pen.setBrush(QBrush(Qt::cyan));

    QGraphicsLineItem *principal = addLine(line, pen);
    principal->setData(0, type);
}

void PrincompScene::placeEllipse(qreal x, qreal y, qreal w, qreal h, itemTypes type, qreal angle)
{
    int width = 5;
    QPen pen(QBrush(Qt::green), width);
    QBrush brush(Qt::transparent);

    QGraphicsEllipseItem *ellipse = addEllipse(0, 0, w, h, pen, brush);
    ellipse->setPos(x, y);
    ellipse->setData(0, type);

    if (angle != 0)
    {
        // Центр эллипса
        qreal center_x = ellipse->rect().center().x();
        qreal center_y = ellipse->rect().center().y();

        // Поворачивает элемент на angle градусов вокруг точки (x, y).
        ellipse->setTransform(QTransform().translate(center_x, center_y).rotate(angle).translate(-center_x, -center_y));
    }
}


void PrincompScene::addPrincomp()
{

    // Выделяем вектор точек
    vector<QPointF> clickPoints;
    foreach(QGraphicsItem *item, items())
    {
        QVariant itemType = item->data(0);
        if (itemType == simplePoint)
            clickPoints.push_back(item->scenePos());
        else
            removeItem(item);
    }

    // Перерасчитываем главные компоненты
    if (clickPoints.size() == 0)
        return;

    // Вычисляем необходимые данные
    PrinComp prinComp;

    // Вычисляем центральную точку
    QPointF meanPoint = prinComp.computeMeanPoint(clickPoints);

    // 1. Отображаем центр
    placePoint(meanPoint.x(), meanPoint.y(), centerPoint);

    // Вычисляем главные компоненты (первую и вторую)
    vector<QLineF> PCAVectors = prinComp.computePCA(clickPoints);

    QLineF firstPrincipalComponent  = PCAVectors[0];
    QLineF secondPrincipalComponent = PCAVectors[1];

    if (clickPoints.size() < 2)
        return;

    // 2. Отображаем главные компоненты
    placeLine(firstPrincipalComponent, firstPrincipal);
    placeLine(secondPrincipalComponent, secondPrincipal);

    // 3. Отображаем эллипс
    // Полуоси
    double majoraxis = firstPrincipalComponent.length() / 2;
    double minoraxis = secondPrincipalComponent.length() / 2;

    // Угол поворота
    qreal angle = atan(firstPrincipalComponent.dy() / firstPrincipalComponent.dx()) * (180 / M_PI);

    placeEllipse(meanPoint.x() - majoraxis, meanPoint.y() - minoraxis,
                 firstPrincipalComponent.length(), secondPrincipalComponent.length(),
                 firstPrincipal, angle);

}

} // namespace
