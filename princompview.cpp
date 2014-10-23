#include "princompellipseitem.h"
#include "princompview.h"
#include <QMouseEvent>
#include <QDebug>
#include "math.h"
#include "princomp.h"

namespace pca {

int PrincompView::m_clickPointsSize = 0;

PrincompView::PrincompView()
{
    setRenderHint(QPainter::Antialiasing, true);
}

void PrincompView::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
//        QPointF pt = mapToScene(e->pos());
//        new PcaPoint(0, 0, 10, 10, scene(), pt);
    }
    QGraphicsView::mousePressEvent(e);

}

void PrincompView::mouseDoubleClickEvent(QMouseEvent * e)
{
    QPointF pt = mapToScene(e->pos());

    // Смещаем в центр
    int diameter = 10;
    pt.setX(pt.x() - diameter / 2);
    pt.setY(pt.y() - diameter / 2);

    // Добавляем в список
    ++m_clickPointsSize;
    PrincompEllipseItem* princompEllipseItem =
            new PrincompEllipseItem(0, 0, diameter, diameter, scene(), pt, m_clickPointsSize);

    connect(princompEllipseItem, SIGNAL(positionChanged(PrincompEllipseItem*)),
            this, SLOT(changePosition(PrincompEllipseItem*)) );

    m_clickPoints.insert(std::pair<int, QPointF>(m_clickPointsSize, pt));
    QGraphicsView::mouseDoubleClickEvent(e);

    paintPCA();
}

void PrincompView::changePosition(PrincompEllipseItem* item)
{
    qDebug() << "pos change" << m_clickPoints[item->getId()].x() << m_clickPoints[item->getId()].y() << item->pos();
    m_clickPoints[item->getId()].setX(item->x());
    m_clickPoints[item->getId()].setY(item->y());

    paintPCA();
}

void PrincompView::paintPCA()
{
    // Перерасчитываем главные компоненты
    if (m_clickPoints.size() == 0)
        return;

    // Вычисляем необходимые данные
    PrinComp prinComp;

    // Выделяем вектор точек
    vector<QPointF> clickPoints;
    for (map<int, QPointF>::iterator it = m_clickPoints.begin(); it != m_clickPoints.end(); ++it) {
        clickPoints.push_back(it->second);
    }

    // Вычисляем центральную точку
    QPointF meanPoint = prinComp.computeMeanPoint(clickPoints);

    // Вычисляем главные компоненты (первую и вторую)
    vector<QLineF> PCAVectors = prinComp.computePCA(clickPoints);

    QLineF firstPrincipalComponent  = PCAVectors[0];
    QLineF secondPrincipalComponent = PCAVectors[1];

//    scene()->clear();

//    // 1. Отображаем точки
//    foreach (QPointF point, clickPoints) {
//        scene()->addEllipse(point.x(), point.y(), 5, 5, QPen(Qt::red), QBrush(Qt::red));
//    }

    // 2. Отображаем центр
    //painter.setPen(QPen(QBrush(Qt::magenta), 5));
    //painter.drawEllipse(meanPoint, 10, 10);

    qDebug() << "scene" << scene()->items();

    scene()->addEllipse(meanPoint.x(), meanPoint.y(), 10, 10);
//scene()->itemAt()
    //scene()->removeItem();

    // 3. Отображаем главные компоненты
//    painter.setPen(QPen(QBrush(Qt::darkBlue), 5));
//    painter.drawLine(firstPrincipalComponent);

//    painter.setPen(QPen(QBrush(Qt::blue), 5));
//    painter.drawLine(secondPrincipalComponent);

//    // 4. Отображаем эллипс
//    painter.save();
//    // Translate
//    painter.translate(meanPoint);

//    // Rotate
//    qreal a = atan(firstPrincipalComponent.dy() / firstPrincipalComponent.dx()) * 180 / M_PI;
//    painter.rotate(a);

//    painter.setPen(QPen(QBrush(Qt::green), 5));
//    painter.setBrush(QBrush(Qt::transparent));
//    painter.drawEllipse(QPointF(0, 0), firstPrincipalComponent.length() / 2, secondPrincipalComponent.length() / 2);
//    painter.restore();
}

} // namespace
