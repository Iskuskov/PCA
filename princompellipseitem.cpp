#include "princompellipseitem.h"

#include <QGraphicsScene>
#include <QDebug>

namespace pca {

PrincompEllipseItem::PrincompEllipseItem(double left, double right,
                                         double w, double h,
                                         QGraphicsScene *scene, QPointF &pt,
                                         int _id) :
    QGraphicsEllipseItem(left, right, w, h),
    m_id(_id)
{

    setFlags(QGraphicsItem::ItemIsMovable   | QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemClipsToShape);

    scene->addItem(this);

    QPen pen(Qt::red);
    pen.setWidth(5);
    setPen(pen);
    setPos(pt.x(), pt.y());

    setAcceptDrops(true);
}

PrincompEllipseItem::~PrincompEllipseItem()
{

}

QVariant PrincompEllipseItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged)
    {
        // Изменяем значения точки (возможно нужно отодвинуть)
        setPos(pos().x(), pos().y());

        // Сигнал - для фиксирования изменений в m_clickPoints
        emit positionChanged(this);
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

} // namespace
