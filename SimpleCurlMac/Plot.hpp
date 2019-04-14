#ifndef Plot_h
#define Plot_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Calculation.hpp"

/* Plot the graph.*/
void plotResults(double* xData, double* yData, double *yData2, double *yData3,int dataSize);

/* Store all data from three groups for plotResults use.*/
void plot_data_manager(vector<Matrix2> &final_Matrix3, double nIntervals, int x0, double* xData, double* yBeat, double* yMeet, double* yMiss);


#endif
