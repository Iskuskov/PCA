#ifndef PRINCOMPSCENE_H
#define PRINCOMPSCENE_H

#include <QGraphicsScene>

namespace pca {

enum itemTypes {
    simplePoint,
    centerPoint,
    firstPrincipal,
    secondPrincipal,
    ellipsePrincipal
};

class PrincompScene : public QGraphicsScene
{
    Q_OBJECT
public:
    PrincompScene(QObject *parent = 0);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);

private slots:
    void addPrincomp();

private:
    void placePoint(qreal x, qreal y, itemTypes type);
    void placeLine(QLineF line, itemTypes type);
    void placeEllipse(qreal x, qreal y, qreal w, qreal h, itemTypes type, qreal angle = 0);
};

}

#endif // PRINCOMPSCENE_H
