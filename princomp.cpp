#include "princomp.h"

#include <math.h>

#include <QLineF>
#include <QPointF>
#include <QDebug>

namespace pca {

int PrinComp::m_dimension = 2;

// Вычисление средних
void PrinComp::computeMeans(vector<QPointF> points,
                            vector<double> & means)
{
    // Инициализация
    means[0] = 0;
    means[1] = 0;

    for (unsigned i = 0; i < points.size(); i++)
        means[0] += points[i].x(),
        means[1] += points[i].y();

    means[0] /= points.size(),
    means[1] /= points.size();
}

// Вычисление ковариационной матрицы
void PrinComp::computeCovarianceMatrix(vector<QPointF> points,
                                       vector<double> means,
                                       vector< vector<double> > & covarianceMatrix)
{
//    // Вычисление средних
//    vector<double> means(DIMENSION);
//    computeMeans(points, means);

    // Вычисление ковариационной матрицы
    for (int i = 0; i < m_dimension; i++)
    {
        for (int j = i; j < m_dimension; j++)
        {
            covarianceMatrix[i][j] = 0.0;
            for (unsigned k = 0; k < points.size(); k++)
            {
                double coord_i = (i == 0) ? points[k].x() : points[k].y();
                double coord_j = (j == 0) ? points[k].x() : points[k].y();

                covarianceMatrix[i][j] += (means[i] - coord_i) * (means[j] - coord_j);
            }
            covarianceMatrix[i][j] /= points.size() - 1;

            // В силу симметричности ковариационной матрицы
            if (i != j)
                covarianceMatrix[j][i] = covarianceMatrix[i][j];
        }
    }

    return;
}

// Вычисление собственных значений
void PrinComp::computeEigenValues(vector< vector<double> > covarianceMatrix,
                                  vector<double> & eigenValues)
{
    // Инициализация
    eigenValues[0] = 0;
    eigenValues[1] = 0;

//    // Вычисление ковариационной матрицы
//    vector< vector<double> > covarianceMatrix(DIMENSION, vector<double>(DIMENSION));
//    computeCovarianceMatrix(points, covarianceMatrix);

    // Вычисление собственных значений:
    // Решаем характеристическое уранение (квадратное)
    double a = covarianceMatrix[0][0];
    double b = covarianceMatrix[1][0]; // covarianceMatrix[1][0] == covarianceMatrix[0][1];
    double d = covarianceMatrix[1][1];

    // Корень из дискриминанта (после упрощения)
    double discriminantSqrt = sqrt( pow((a - d), 2) + (4 * pow(b, 2) ) );

    eigenValues[0] = ( (a + d) + discriminantSqrt ) / 2;
    eigenValues[1] = ( (a + d) - discriminantSqrt ) / 2;
}

// Вычисление собственных векторов
void PrinComp::computeEigenVectors(vector< vector<double> > covarianceMatrix,
                                   vector<double> eigenValues,
                                   vector< vector<double> > & eigenVectors)
{
    // Собственные векторы
    double aplus  = covarianceMatrix[0][0] + covarianceMatrix[0][1] - eigenValues[1];
    double bplus  = covarianceMatrix[1][1] + covarianceMatrix[0][1] - eigenValues[1];
    double aminus = covarianceMatrix[0][0] + covarianceMatrix[0][1] - eigenValues[0];
    double bminus = covarianceMatrix[1][1] + covarianceMatrix[0][1] - eigenValues[0];

    // Нормализация
    double denomPlus = sqrtf(aplus*aplus + bplus*bplus);
    double denomMinus= sqrtf(aminus*aminus + bminus*bminus);

    eigenVectors[0][0] = aplus / denomPlus;
    eigenVectors[0][1] = bplus / denomPlus;
    eigenVectors[1][0] = aminus / denomMinus;
    eigenVectors[1][1] = bminus / denomMinus;
}

// Последовательное вычисление необходимых данных
void PrinComp::computePCAData(vector<QPointF> points,
                              vector<double> & means,
                              vector< vector<double> > & covarianceMatrix,
                              vector<double> & eigenValues,
                              vector< vector<double> > & eigenVectors)
{
    // Вычисление средних
    computeMeans(points, means);

    // Вычисление ковариационной матрицы
    computeCovarianceMatrix(points, means, covarianceMatrix);

    // Вычисление собственных значений
    computeEigenValues(covarianceMatrix, eigenValues);

    // Вычисление собственных векторов
    computeEigenVectors(covarianceMatrix, eigenValues, eigenVectors);
}

/* ----------------------------------------------------- */

// Вычисление центральной точки
QPointF PrinComp::computeMeanPoint(vector<QPointF> points)
{
    // Вычисление средних
    vector<double> means(m_dimension);
    computeMeans(points, means);

    // Центральная точка
    QPointF meanPoint(means[0], means[1]);
    return meanPoint;
}

// Вычисление главных компонент (первой и второй)
vector<QLineF> PrinComp::computePCA(vector<QPointF> points)
{
    // Вычисляем необходимые данные
    vector<double> means(m_dimension);
    vector< vector<double> > covarianceMatrix(m_dimension, vector<double>(m_dimension));
    vector<double> eigenValues(m_dimension);
    vector< vector<double> > eigenVectors(m_dimension, vector<double>(m_dimension));

    computePCAData(points, means, covarianceMatrix, eigenValues, eigenVectors);

    // Вычисляем главные компоненты (первую и вторую)
    QLineF firstPrincipalComponent;
    QLineF secondPrincipalComponent;

    // Полуоси (первой и второй главных компонент)
    double k = 2; // scale factor
    double majoraxis = k*sqrtf(eigenValues[0]);
    double minoraxis = k*sqrtf(eigenValues[1]);

    // Первая главная компонента
    QPointF start (means[0] - eigenVectors[0][0] * majoraxis, means[1] - eigenVectors[0][1] * majoraxis);
    QPointF finish(means[0] + eigenVectors[0][0] * majoraxis, means[1] + eigenVectors[0][1] * majoraxis);
    firstPrincipalComponent.setPoints(start, finish);

    // Вторая главная компонента
    QPointF secondStart (means[0] - eigenVectors[1][0] * minoraxis, means[1] - eigenVectors[1][1] * minoraxis);
    QPointF secondFinish(means[0] + eigenVectors[1][0] * minoraxis, means[1] + eigenVectors[1][1] * minoraxis);
    secondPrincipalComponent.setPoints(secondStart, secondFinish);

    vector<QLineF> PCAVectors;
    PCAVectors.push_back(firstPrincipalComponent);
    PCAVectors.push_back(secondPrincipalComponent);

    return PCAVectors;
}

} // namespace
