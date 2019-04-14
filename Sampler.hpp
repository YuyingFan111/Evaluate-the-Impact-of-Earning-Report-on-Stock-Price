
#ifndef Sampler_hpp
#define Sampler_hpp
#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;


class Sampler
{
public:
    //this function generates n unrepeated random integers in the interval [0,m)
    static vector<int> GenerateIndice(int n, int m)
    {
        vector<int> temp;
        if (n>m)
        {
            cout << "Invalid input, n must be smaller than m!\n";
            exit(1);
        }
        srand((unsigned)time(NULL));
        while(temp.size()<n)
        {
            int temp_rand = rand()%m;
            vector<int>::iterator it = find(temp.begin(), temp.end(), temp_rand);
            if(it == temp.end())
            {
                temp.push_back(temp_rand);
                //                cout << temp_rand << endl;
            }
            else
                continue;
        }
        return temp;
    }
};




#endif /* Sampler_hpp */
