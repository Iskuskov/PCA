#ifndef PRINCOMP_H
#define PRINCOMP_H

#include <vector>

using namespace std;

class PrinComp
{
public:
    PrinComp();

    // Ковариационная матрица
    void compute_covariance_matrix(vector< vector<double> > & d, vector< vector<double> > & dst);

    //Собственные векторы



    void outer_product(vector<double> row, vector<double> col, vector< vector<double> >& dst);

    //computes row[i] - val for all i;
    void subtract(vector<double> row, double val, vector<double>& dst);

    //computes m[i][j] + m2[i][j]
    void add(vector< vector<double> > m, vector< vector<double> > m2, vector< vector<double> >& dst);

    double mean(std::vector<double> &data);

    void scale(vector< vector<double> > & d, double alpha);


    void print(vector< vector<double> > & data);
};

#endif // PRINCOMP_H
