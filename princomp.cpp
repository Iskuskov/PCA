#include "princomp.h"

#include <math.h>
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

// Вычисление первой главной компоненты (PC1)
// Возвращает нормаль вектора из центра
QLineF PrinComp::computePCA1(vector<QPointF> points,
                             vector<double> means,
                             vector< vector<double> > covarianceMatrix,
                             vector<double> eigenValues)
{
//    // Вычисление средних
//    vector<double> means(DIMENSION);
//    computeMeans(points, means);

//    // Вычисление ковариационной матрицы
//    vector< vector<double> > covarianceMatrix(DIMENSION, vector<double>(DIMENSION));;
//    computeCovarianceMatrix(points, covarianceMatrix);

//    // Вычисление собственных значений
//    vector<double> eigenValues(DIMENSION);
//    computeEigenValues(points, eigenValues);


    double slope;           // Наклон прямой
    double half_length;     // Половина длины прямой (от центра)

    double proj_x, proj_y;  // Проекции прямой


    slope = (eigenValues[0] - covarianceMatrix[0][0]) / covarianceMatrix[1][0];
    half_length = eigenValues[0] / 2;

    proj_y = half_length * sqrt( 1 / (1 + slope * slope) );
    proj_x = proj_y * slope;

    QPointF start (means[0] - proj_x, means[1] - proj_y);
    QPointF finish(means[0] + proj_x, means[1] + proj_y);

    QLineF firstPrincipalComponent(start, finish);
    return firstPrincipalComponent;
}

//// Вычисление второй главной компоненты (PC2)
//QLineF PrinComp::getNormalLine(QLineF line)
//{
//    QLineF normalLine = line.normalVector();
//    return normalLine;
//}
