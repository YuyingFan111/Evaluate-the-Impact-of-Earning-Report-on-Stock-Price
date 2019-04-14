#include "Calculation.hpp"

using namespace std;

vector<double> AAR(Matrix2 m)
{
    int row=m.size();
    int col=m[0].size();
    vector<double> aar;
    for (int i=0; i<col; i++)
    {
        double initial=0.0;
        for (int j=0; j<row; j++)
        {
            initial+=m[j][i];
        }
        initial=initial/row;
        aar.push_back(initial);
    }
    
    return aar;
}

vector<double> CAAR(vector<double> v)
{
    int size=v.size();
    vector<double> caar;
    double initial=0.0;
    for (int j=0; j<size; j++)
    {
        initial+=v[j];
        caar.push_back(initial);
    }
    return caar;
}

/* Present AAR and CAAR for three groups in a matrix.*/
void populate_final_AAR_CAAR(vector<double> &AAR_avg, vector<double> &CAAR_avg, vector<Matrix2> &final_Matrix3) {
    Matrix2 temp_AAR_CAAR;
    temp_AAR_CAAR.push_back(AAR_avg);
    temp_AAR_CAAR.push_back(CAAR_avg);
    final_Matrix3.push_back(temp_AAR_CAAR);
}
