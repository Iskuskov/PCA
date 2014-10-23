#ifndef PRINCOMP_H
#define PRINCOMP_H

#include <vector>

class QPointF;
class QLineF;

using namespace std;

namespace pca {

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

    // ���������� ����������� ��������
    void computeEigenVectors(vector< vector<double> > covarianceMatrix,
                             vector<double> eigenValues,
                             vector< vector<double> > & eigenVectors);

    // ���������������� ���������� ����������� ������
    void computePCAData(vector<QPointF> points,
                        vector<double> & means,
                        vector< vector<double> > & covarianceMatrix,
                        vector<double> & eigenValues,
                        vector< vector<double> > & eigenVectors);

    // ���������� ����������� �����
    QPointF computeMeanPoint(vector<QPointF> points);

    // ���������� ������� ��������� (������ � ������)
    vector<QLineF> computePCA(vector<QPointF> points);

    // ����������� - ��������� (2D)
    static int dimension;
};

} // namespace

#endif // PRINCOMP_H
