#include "princomp.h"

#include <math.h>
#include <QDebug>

int PrinComp::dimension = 2;

// ���������� �������
void PrinComp::computeMeans(vector<QPointF> points,
                            vector<double> & means)
{
    // �������������
    means[0] = 0;
    means[1] = 0;

    for (unsigned i = 0; i < points.size(); i++)
        means[0] += points[i].x(),
        means[1] += points[i].y();

    means[0] /= points.size(),
    means[1] /= points.size();
}

// ���������� �������������� �������
void PrinComp::computeCovarianceMatrix(vector<QPointF> points,
                                       vector<double> means,
                                       vector< vector<double> > & covarianceMatrix)
{
//    // ���������� �������
//    vector<double> means(DIMENSION);
//    computeMeans(points, means);

    // ���������� �������������� �������
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

// ���������� ����������� ��������
void PrinComp::computeEigenValues(vector< vector<double> > covarianceMatrix,
                                  vector<double> & eigenValues)
{
    // �������������
    eigenValues[0] = 0;
    eigenValues[1] = 0;

//    // ���������� �������������� �������
//    vector< vector<double> > covarianceMatrix(DIMENSION, vector<double>(DIMENSION));
//    computeCovarianceMatrix(points, covarianceMatrix);

    // ���������� ����������� ��������
    double a = covarianceMatrix[0][0];
    double b = covarianceMatrix[1][0]; // covarianceMatrix[1][0] == covarianceMatrix[0][1];
    double d = covarianceMatrix[1][1];

    eigenValues[0] = ( (a + d) + sqrt( pow((a - d), 2) + (4 * pow(b, 2) ) ) ) / 2;
    eigenValues[1] = ( (a + d) - sqrt( pow((a - d), 2) + (4 * pow(b, 2) ) ) ) / 2;
}

// ���������� ������ ������� ���������� (PC1)
// ���������� ������� ������� �� ������
QLineF PrinComp::computePCA1(vector<QPointF> points,
                             vector<double> means,
                             vector< vector<double> > covarianceMatrix,
                             vector<double> eigenValues)
{
//    // ���������� �������
//    vector<double> means(DIMENSION);
//    computeMeans(points, means);

//    // ���������� �������������� �������
//    vector< vector<double> > covarianceMatrix(DIMENSION, vector<double>(DIMENSION));;
//    computeCovarianceMatrix(points, covarianceMatrix);

//    // ���������� ����������� ��������
//    vector<double> eigenValues(DIMENSION);
//    computeEigenValues(points, eigenValues);


    double slope;           // ������ ������
    double half_length;     // �������� ����� ������ (�� ������)

    double proj_x, proj_y;  // �������� ������


    slope = (eigenValues[0] - covarianceMatrix[0][0]) / covarianceMatrix[1][0];
    half_length = eigenValues[0] / 2;

    proj_y = half_length * sqrt( 1 / (1 + slope * slope) );
    proj_x = proj_y * slope;

    QPointF start (means[0] - proj_x, means[1] - proj_y);
    QPointF finish(means[0] + proj_x, means[1] + proj_y);

    QLineF firstPrincipalComponent(start, finish);
    return firstPrincipalComponent;
}

//// ���������� ������ ������� ���������� (PC2)
//QLineF PrinComp::getNormalLine(QLineF line)
//{
//    QLineF normalLine = line.normalVector();
//    return normalLine;
//}
