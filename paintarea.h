#ifndef PAINTAREA_H
#define PAINTAREA_H

using namespace std;

#include <QImage>
#include <QWidget>
#include <vector>

class QGraphicsScene;
class QGraphicsPixmapItem;

namespace pca {

class PaintArea : public QWidget
{
    Q_OBJECT

public:
    PaintArea(QWidget *parent = 0);

public slots:
    void clearImage();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void mousePressEvent(QMouseEvent *e);

private:
    void resizeImage(QImage *image, const QSize &newSize);

    QImage theImage;

    vector<QPointF> clickPoints;

    //
    vector<double> means;
    vector< vector<double> > covarianceMatrix;
    vector<double> eigenValues;
    vector< vector<double> > eigenVectors;
};

} // namespace



#endif
