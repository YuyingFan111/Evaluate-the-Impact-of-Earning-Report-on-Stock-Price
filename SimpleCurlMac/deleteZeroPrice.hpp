#ifndef deleteZeroPrice_hpp
#define deleteZeroPrice_hpp

#include <stdio.h>
bool contain(double target, vector<double> v)
{
    for(int i=0; i<v.size();i++)
    {
        if(target == v[i])
        {
            return true;
        }
    }
    
    return false;
}

void SpecialDelete(double target, vector<Series>& vec)
{
    for(vector<Series>::iterator itr=vec.begin();itr!=vec.end();itr++)
    {
        vector<double> temp = (*itr).GetPrice();
        if(contain(target,temp))
        {
            vec.erase(itr);
            itr--;
        }
    }
    
}

#endif /* deleteZeroPrice_hpp */
