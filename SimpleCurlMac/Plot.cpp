#include "Plot.hpp"
void plotResults(double* xData, double* yData, double* yData2, double* yData3, int dataSize)
{
    FILE *gnuplotPipe,*tempDataFile,*tempDataFile2,*tempDataFile3;
    const char *tempDataFileName,*tempDataFileName2,*tempDataFileName3;
    double x,y,x2,y2,x3,y3;
    int i;
    tempDataFileName = "beat";
    tempDataFileName2 = "meet";
    tempDataFileName3 = "miss";
    gnuplotPipe = popen("/opt/local/bin/gnuplot","w");
    
    if (gnuplotPipe)
    {
        fprintf(gnuplotPipe,"plot [-150:150] \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n",tempDataFileName,tempDataFileName2,tempDataFileName3);
        fprintf(gnuplotPipe,"set colorbox vertical origin screen 0.9, 0.2, 0 size screen 0.05, 0.6, 0 front  noinvert bdefault\n");
        fflush(gnuplotPipe);
        tempDataFile = fopen(tempDataFileName,"w");
        tempDataFile2 = fopen(tempDataFileName2,"w");
        tempDataFile3 = fopen(tempDataFileName3,"w");
        for (i=0; i <= dataSize; i++)
        {
            x = xData[i];
            y = yData[i];
            fprintf(tempDataFile,"%lf %lf\n",x,y);
        }
        
        for (i=0; i <= dataSize; i++)
        {
            x2 = xData[i];
            y2 = yData2[i];
            fprintf(tempDataFile2,"%lf %lf\n",x2,y2);
        }
        
        for (i=0; i <= dataSize; i++)
        {
            x3 = xData[i];
            y3 = yData3[i];
            fprintf(tempDataFile3,"%lf %lf\n",x3,y3);
        }
        
        
        fclose(tempDataFile);
        fclose(tempDataFile2);
        fclose(tempDataFile3);
        
        printf("press enter to continue...");
        getchar();
        fprintf(gnuplotPipe,"exit \n");
    }
    else
    {
        printf("gnuplot not found...");
    }
}

/* Store all data from three groups for plotResults use.*/
void plot_data_manager(vector<Matrix2> &final_Matrix3, double nIntervals, int x0, double* xData, double* yBeat, double* yMeet, double* yMiss) {
    for (int i = 0; i < nIntervals; i++) {
        x0 = xData[i];
        xData[i+1] = x0 + 1;
    }
    for (int i = 0; i <= nIntervals; i++) {
        yBeat[i]=final_Matrix3[0][1][i];
    }
    for (int i = 0; i <= nIntervals; i++) {
        yMeet[i]=final_Matrix3[1][1][i];
    }
    for (int i = 0; i <= nIntervals; i++) {
        yMiss[i]=final_Matrix3[2][1][i];
    }
}
