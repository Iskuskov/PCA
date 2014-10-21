#ifndef PRINCOMP_H
#define PRINCOMP_H

#include <vector>
#include <QPointF>
#include <QLineF>

using namespace std;

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

    // Вычисление первой главной компоненты (PC1)
    QLineF computePCA1(vector<QPointF> points,
                       vector<double> means,
                       vector< vector<double> > covarianceMatrix,
                       vector<double> eigenValues);

    static int dimension;
};

#endif // PRINCOMP_H
