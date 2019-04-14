#include "Operations.hpp"
using namespace std;

/* Operator overloading for vector calculation.*/
vector<double> operator-(const vector<double>& v, const vector<double>& w)
{
    
    int s=v.size();
    vector<double> result(s);
    for ( int i=0; i<s; i++) result[i]=v[i]-w[i];
    
    return result;
}
