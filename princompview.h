#ifndef PRINCOMPVIEW_H
#define PRINCOMPVIEW_H

#include <QGraphicsView>
#include <map>

//class PrincompEllipseItem;
#include "princompellipseitem.h"

namespace pca {

class PrincompView : public QGraphicsView
{
    Q_OBJECT

public:
    PrincompView();

public slots:
    void mousePressEvent(QMouseEvent * e);
    void mouseDoubleClickEvent(QMouseEvent * e);
    void changePosition(PrincompEllipseItem* item);

protected:
//    void paintEvent(QPaintEvent *event);

private:
    std::map<int, QPointF> m_clickPoints;
    static int m_clickPointsSize;

    void paintPCA();
};

} // namespace

#endif // PRINCOMPVIEW_H
