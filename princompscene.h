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
    ~PrincompScene();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);

public slots:
    void clearScene();

private slots:
    void addPrincomp();

private:
    void initializePrincipal();

    void placePoint(qreal x, qreal y, itemTypes type);

    void updateCenter(qreal x, qreal y);
    void updateLine(QLineF line, itemTypes type);
    void updateEllipse(qreal x, qreal y, qreal w, qreal h, qreal angle = 0);

    QGraphicsEllipseItem* m_center;
    QGraphicsLineItem *m_firstPrincipal;
    QGraphicsLineItem *m_secondPrincipal;
    QGraphicsEllipseItem* m_ellipse;

    // Настройки отображения

    // 1. Точки
    int m_pointDiameter;
    QColor m_pointPenColor;
    QColor m_pointBrushColor;

    // 2. Центр
    int m_centerDiameter;

    QColor m_centerPenColor;
    QColor m_centerBrushColor;

    // 3. Компоненты
    int m_firstPrincipalWidth;
    int m_secondPrincipalWidth;

    QColor m_firstPrincipalColor;
    QColor m_secondPrincipalColor;

    // 4. Эллипс
    int m_ellipseWidth;
    QColor m_ellipsePenColor;
    QColor m_ellipseBrushColor;
};

}

#endif // PRINCOMPSCENE_H
