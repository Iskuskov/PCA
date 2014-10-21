#ifndef PRINCOMP_H
#define PRINCOMP_H

#include <vector>
#include <QPointF>
#include <QLineF>

using namespace std;

// ����� ������� ������ ������� ��������� ��� QPointF
class PrinComp
{
public:
    // ������ ��������
    void computeMeans(vector<QPointF> points,
                      vector<double> & means);

    // ���������� �������������� �������
    void computeCovarianceMatrix(vector<QPointF> points,
                                 vector<double> means,
                                 vector< vector<double> > & covarianceMatrix);

    // ���������� ����������� ��������
    void computeEigenValues(vector<vector<double> > covarianceMatrix,
                            vector<double> & eigenValues);

    // ���������� ������ ������� ���������� (PC1)
    QLineF computePCA1(vector<QPointF> points,
                       vector<double> means,
                       vector< vector<double> > covarianceMatrix,
                       vector<double> eigenValues);

    static int dimension;
};

#endif // PRINCOMP_H
