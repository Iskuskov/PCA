#include "princompscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <math.h>
#include "princomp.h"

namespace pca {

PrincompScene::PrincompScene(QObject *parent) :
    QGraphicsScene(parent),
    m_pointDiameter(20),          // 1. Точки
    m_pointPenColor(Qt::black),
    m_pointBrushColor(Qt::red),
    m_centerDiameter(20),         // 2. Центр
    m_centerPenColor(Qt::black),
    m_centerBrushColor(Qt::yellow),
    m_firstPrincipalWidth(5),     // 3. Компоненты
    m_secondPrincipalWidth(5),
    m_firstPrincipalColor(Qt::darkBlue),
    m_secondPrincipalColor(Qt::cyan),
    m_ellipseWidth(5),            // 4. Эллипс
    m_ellipsePenColor(Qt::green),
    m_ellipseBrushColor(Qt::transparent)
{
    // Инициализируем отоборажаемые объекты (центр, главные компоненты, эллипс)
    initializePrincipal();

    connect(this, SIGNAL(changed(QList<QRectF>)), this, SLOT(addPrincomp()));
}

PrincompScene::~PrincompScene()
{
    removeItem(m_center);
    delete m_center;

    removeItem(m_firstPrincipal);
    delete m_firstPrincipal;

    removeItem(m_secondPrincipal);
    delete m_secondPrincipal;

    removeItem(m_ellipse);
    delete m_ellipse;
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
        {
            removeItem(item);
            delete item;
        }
    }
}

void PrincompScene::clearScene()
{
    clear();
    initializePrincipal();
}

void PrincompScene::initializePrincipal()
{
    m_center = addEllipse(QRect());
    m_center->setData(0, centerPoint);

    m_firstPrincipal = addLine(QLine());
    m_firstPrincipal->setData(0, firstPrincipal);

    m_secondPrincipal = addLine(QLine());
    m_secondPrincipal->setData(0, secondPrincipal);

    m_ellipse = addEllipse(QRect());
    m_ellipse->setData(0, ellipsePrincipal);
}

void PrincompScene::placePoint(qreal x, qreal y, itemTypes type)
{
    QPen pen(m_pointPenColor);
    QBrush brush(m_pointBrushColor);

    QGraphicsEllipseItem *point = addEllipse(0, 0, m_pointDiameter, m_pointDiameter, pen, brush);

    point->setPos(x - m_pointDiameter / 2, y - m_pointDiameter / 2); // (x, y);
    point->setData(0, type);

    if (type == simplePoint)
    {
        point->setFlags(point->flags() | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
                        QGraphicsItem::ItemIsFocusable );
        point->setAcceptDrops(true);
        point->setZValue(+10);      // Хак для отображения точек поверх остального
    }
}

void PrincompScene::updateCenter(qreal x, qreal y)
{
    QPen pen(m_centerPenColor);
    QBrush brush(m_centerBrushColor);

    m_center->setRect(0, 0, m_centerDiameter, m_centerDiameter);
    m_center->setPos(x - m_centerDiameter / 2, y - m_centerDiameter / 2);
    m_center->setPen(pen);
    m_center->setBrush(brush);
}

void PrincompScene::updateLine(QLineF line, itemTypes type)
{
    int principalWidth;
    QColor principalColor;
    QGraphicsLineItem *principal;

    if (type == firstPrincipal)
    {
        principalWidth = m_firstPrincipalWidth;
        principalColor = m_firstPrincipalColor;
        principal = m_firstPrincipal;
    }
    else if (type == secondPrincipal)
    {
        principalWidth = m_secondPrincipalWidth;
        principalColor = m_secondPrincipalColor;
        principal = m_secondPrincipal;
    }

    QPen pen(QBrush(principalColor), principalWidth);
    principal->setPen(pen);
    principal->setLine(line);
}

void PrincompScene::updateEllipse(qreal x, qreal y, qreal w, qreal h, qreal angle)
{
    QPen pen(QBrush(m_ellipsePenColor), m_ellipseWidth);
    QBrush brush(m_ellipseBrushColor);

    m_ellipse->setRect(0, 0, w, h);
    m_ellipse->setPos(x, y);
    m_ellipse->setPen(pen);
    m_ellipse->setBrush(brush);

    if (angle != 0)
    {
        // Центр эллипса
        qreal center_x = m_ellipse->rect().center().x();
        qreal center_y = m_ellipse->rect().center().y();

        // Поворачивает элемент на angle градусов вокруг точки (x, y).
        m_ellipse->setTransform(QTransform().translate(center_x, center_y).rotate(angle).translate(-center_x, -center_y));
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
        {
            QGraphicsEllipseItem *point = dynamic_cast<QGraphicsEllipseItem*>(item);
            if (point != NULL)
            {
                // Берем координату центра
                int diameter = point->rect().width();
                clickPoints.push_back(QPointF(item->x() + diameter / 2, item->y() + diameter / 2));
            }
        }
    }

    if (clickPoints.size() == 0)
    {
        clearScene();
        return;
    }

    // Перерасчитываем главные компоненты
    // Вычисляем необходимые данные
    PrinComp prinComp;

    // Вычисляем центральную точку
    QPointF meanPoint = prinComp.computeMeanPoint(clickPoints);

    // 1. Отображаем центр
    updateCenter(meanPoint.x(), meanPoint.y());

    // Вычисляем главные компоненты (первую и вторую)
    vector<QLineF> PCAVectors = prinComp.computePCA(clickPoints);

    QLineF firstPrincipalComponent  = PCAVectors[0];
    QLineF secondPrincipalComponent = PCAVectors[1];

    // 2. Отображаем главные компоненты
    updateLine(firstPrincipalComponent, firstPrincipal);
    updateLine(secondPrincipalComponent, secondPrincipal);

    // 3. Отображаем эллипс
    qreal firstPrincipalLength = firstPrincipalComponent.length();
    qreal secondPrincipalLength = secondPrincipalComponent.length();

    // Полуоси
    double majoraxis = firstPrincipalLength / 2;
    double minoraxis = secondPrincipalLength / 2;

    // Угол поворота
    qreal angle = (firstPrincipalLength > 0)
            ? atan(firstPrincipalComponent.dy() / firstPrincipalComponent.dx()) * (180 / M_PI)
            : 0;

    updateEllipse(meanPoint.x() - majoraxis, meanPoint.y() - minoraxis,
                  firstPrincipalLength, secondPrincipalLength, angle);
}

} // namespace
