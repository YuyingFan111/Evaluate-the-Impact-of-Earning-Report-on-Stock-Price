#ifndef Calculation_hpp
#define Calculation_hpp

#include <vector>
using namespace std;

typedef vector<vector<double>> Matrix2;

vector<double> AAR(Matrix2 m);

vector<double> CAAR(vector<double> v);

/* Present AAR and CAAR for three groups in a matrix.*/
void populate_final_AAR_CAAR(vector<double> &AAR_avg, vector<double> &CAAR_avg, vector<Matrix2> &final_Matrix3);

#endif /* Calculation_hpp */
