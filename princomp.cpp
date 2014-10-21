#include "princomp.h"

#include <math.h>

#include <QLineF>
#include <QPointF>

#include <QDebug>

int PrinComp::dimension = 2;

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
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            covarianceMatrix[i][j] = 0.0;
            for (unsigned k = 0; k < points.size(); k++)
            {
                double coord_i = (i == 0) ? points[k].x() : points[k].y();
                double coord_j = (j == 0) ? points[k].x() : points[k].y();

                covarianceMatrix[i][j] += (means[i] - coord_i) * (means[j] - coord_j);
            }
            covarianceMatrix[i][j] /= points.size() - 1;
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

    // Вычисление собственных значений
    double a = covarianceMatrix[0][0];
    double b = covarianceMatrix[1][0]; // covarianceMatrix[1][0] == covarianceMatrix[0][1];
    double d = covarianceMatrix[1][1];

    eigenValues[0] = ( (a + d) + sqrt( pow((a - d), 2) + (4 * pow(b, 2) ) ) ) / 2;
    eigenValues[1] = ( (a + d) - sqrt( pow((a - d), 2) + (4 * pow(b, 2) ) ) ) / 2;
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

// Вычисление первой главной компоненты (PC1)
// Возвращает нормаль вектора из центра
QLineF PrinComp::computePCA1(vector<double> means,
                             vector< vector<double> > covarianceMatrix,
                             vector<double> eigenValues)
{
//    // Вычисление средних
//    vector<double> means(DIMENSION);
//    computeMeans(points, means);

//    // Вычисление ковариационной матрицы
//    vector< vector<double> > covarianceMatrix(DIMENSION, vector<double>(DIMENSION));
//    computeCovarianceMatrix(points, covarianceMatrix);

//    // Вычисление собственных значений
//    vector<double> eigenValues(DIMENSION);
//    computeEigenValues(points, eigenValues);



    double majoraxis;       // PCA1
    double minoraxis;       // PCA2

    double slope;           // Наклон прямой
    double proj_x, proj_y;  // Проекции прямой

    double k = 2;           // scale factor
    majoraxis = k*sqrtf(eigenValues[0]);
    minoraxis = k*sqrtf(eigenValues[1]);

    slope = (eigenValues[0] - covarianceMatrix[0][0]) / covarianceMatrix[1][0];
    proj_x = majoraxis * sqrt( 1 / (1 + slope * slope) );
    proj_y = proj_y * slope;




    // Вычисление собственных значений
    double a = covarianceMatrix[0][0];
    double b = covarianceMatrix[1][0]; // covarianceMatrix[1][0] == covarianceMatrix[0][1];
    double d = covarianceMatrix[1][1];

    proj_x = b;
    proj_y = eigenValues[0] - a;

    QPointF start (means[0] - proj_x, means[1] - proj_y);
    QPointF finish(means[0] + proj_x, means[1] + proj_y);

    QLineF firstPrincipalComponent(start, finish);
    return firstPrincipalComponent;
}

QLineF PrinComp::computePCA1(vector<QPointF> points,
                   vector<double> & means,
                   vector< vector<double> > & covarianceMatrix,
                   vector<double> & eigenValues)
{
    // Вычисление средних
    computeMeans(points, means);

    // Вычисление ковариационной матрицы
    computeCovarianceMatrix(points, means, covarianceMatrix);

    // Вычисление собственных значений
    computeEigenValues(covarianceMatrix, eigenValues);

    QLineF firstPrincipalComponent = computePCA1(means, covarianceMatrix, eigenValues);
    return firstPrincipalComponent;
}

QLineF PrinComp::computePCA1(vector<QPointF> points)
{
    vector<double> means(dimension);
    vector< vector<double> > covarianceMatrix(dimension, vector<double>(dimension));
    vector<double> eigenValues(dimension);

    // Вычисление средних
    computeMeans(points, means);

    // Вычисление ковариационной матрицы
    computeCovarianceMatrix(points, means, covarianceMatrix);

    // Вычисление собственных значений
    computeEigenValues(covarianceMatrix, eigenValues);

    QLineF firstPrincipalComponent = computePCA1(means, covarianceMatrix, eigenValues);
    return firstPrincipalComponent;
}


//// Вычисление второй главной компоненты (PC2)
//QLineF PrinComp::getNormalLine(QLineF line)
//{
//    QLineF normalLine = line.normalVector();
//    return normalLine;
//}
