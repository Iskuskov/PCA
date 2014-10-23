#ifndef PRINCOMPELLIPSEITEM_H
#define PRINCOMPELLIPSEITEM_H

#include <QObject>
#include <QGraphicsEllipseItem>

namespace pca {

class PrincompEllipseItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    PrincompEllipseItem(double left, double right, double w, double h,
                        QGraphicsScene *scene, QPointF &pt, int _id = 0);

    ~PrincompEllipseItem();

    void setId(int _id) { m_id = _id; }
    int  getId()        { return m_id; }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void positionChanged(PrincompEllipseItem* cell);

private:
    int m_id;
};

} // namespace

#endif // PRINCOMPELLIPSEITEM_H
