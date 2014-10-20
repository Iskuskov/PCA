#include "princomp.h"

#include <iostream>
#include <QDebug>

PrinComp::PrinComp()
{
}

void PrinComp::outer_product(vector<double> row, vector<double> col, vector< vector<double> >& dst)
{
    qDebug() << "outer_product" << row.size() << col.size()
             << dst.size() << dst.at(0).size() << dst.at(1).size();

//    print(dst);

    for(unsigned i = 0; i < row.size(); i++)
    {
        for(unsigned j = 0; j < col.size(); j++)
        {
//            qDebug() << i << j << dst[i][j];// = row[i] * col[j];
//            dst.at(i).push_back(row[i] * col[j]);

            dst[i][j] = row[i] * col[j];
//            qDebug() << i << j << row[i] << col[j] << dst[i][j];
        }
    }

    print(dst);
}

//computes row[i] - val for all i;
void PrinComp::subtract(vector<double> row, double val, vector<double>& dst)
{
    for(unsigned i = 0; i < row.size(); i++)
    {
        dst[i] = row[i] - val;
    }
}

//computes m[i][j] + m2[i][j]
void PrinComp::add(vector< vector<double> > m, vector< vector<double> > m2, vector< vector<double> >& dst)
{
    qDebug() << "add" << m.size() << m[0].size();

    for(unsigned i = 0; i < m.size(); i++)
    {
        for(unsigned j = 0; j < m[i].size(); j++)
        {
            dst[i][j] = m[i][j] + m2[i][j];
            qDebug() << i << j << dst[i][j];
        }
    }
}

double PrinComp::mean(std::vector<double> &data)
{
    double mean = 0.0;

    for(unsigned i = 0; (i < data.size()); i++)
    {
        mean += data[i];
    }

    mean /= data.size();
    return mean;
}

void PrinComp::scale(vector< vector<double> > & d, double alpha)
{
    for(unsigned i = 0; i < d.size(); i++)
    {
        for(unsigned j = 0; j < d[i].size(); j++)
        {
            d[i][j] *= alpha;
        }
    }
}

void PrinComp::compute_covariance_matrix(vector< vector<double> > & d, vector< vector<double> > & dst)
{
    for(unsigned i = 0; i < d.size(); i++)
    {
        double y_bar = mean(d[i]);
        vector<double> d_d_bar(d[i].size());
        subtract(d[i], y_bar, d_d_bar);

        // @TODO:
        // Выделение памяти под внутреннее измерение (разобраться)

       vector< vector<double> > t(d_d_bar.size());
       for (int i = 0; i < t.size(); ++i)
            t.at(i).resize(d_d_bar.size());

        outer_product(d_d_bar, d_d_bar, t);
        add(dst, t, dst);
    }
    scale( dst, (1.0 / (d.size() - 1)) );
}

void PrinComp::print(vector< vector<double> > & data)
{
    for (int i = 0; i < data.size(); ++i)
    {
        for (int j = 0; j < data.at(i).size(); ++j)
            qDebug() << data.at(i).at(j);

        qDebug() << "\n";
    }
}
