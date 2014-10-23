#ifndef PRINCOMP_H
#define PRINCOMP_H

#include <vector>

class QPointF;
class QLineF;

using namespace std;

namespace pca {

// Класс расчета метода главных компонент для QPointF
class PrinComp
{
public:
    // Расчет среднего
    void computeMeans(vector<QPointF> points,
                      vector<double> & means);

    // Вычисление ковариационной матрицы
    void computeCovarianceMatrix(vector<QPointF> points,
                                 vector<double> means,
                                 vector< vector<double> > & covarianceMatrix);

    // Вычисление собственных значений
    void computeEigenValues(vector<vector<double> > covarianceMatrix,
                            vector<double> & eigenValues);

    // Вычисление собственных векторов
    void computeEigenVectors(vector< vector<double> > covarianceMatrix,
                             vector<double> eigenValues,
                             vector< vector<double> > & eigenVectors);

    // Последовательное вычисление необходимых данных
    void computePCAData(vector<QPointF> points,
                        vector<double> & means,
                        vector< vector<double> > & covarianceMatrix,
                        vector<double> & eigenValues,
                        vector< vector<double> > & eigenVectors);

    // Вычисление центральной точки
    QPointF computeMeanPoint(vector<QPointF> points);

    // Вычисление главных компонент (первой и второй)
    vector<QLineF> computePCA(vector<QPointF> points);

    // Размерность - плоскость (2D)
    static int dimension;
};

} // namespace

#endif // PRINCOMP_H
