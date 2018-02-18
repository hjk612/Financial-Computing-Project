#ifndef gnuplot_h
#define gnuplot_h
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <iostream>


using namespace std;

void plotresults(vector<vector<double>> *matrix)
{
    getchar();
    FILE *gnuplotPipe,*DataFile;
    int prd =60;
    
    const char *DataFileName;
    DataFileName = "CAAR plots";
    gnuplotPipe = popen("/opt/local/bin/gnuplot","w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "plot \"%s\" using 2 title 'Miss' lw 2 with linespoints,\"%s\" using 3 title 'Beat' lw 2 with linespoints,\"%s\" using 4 title 'Meet' lw 2 with linespoints \n", DataFileName,DataFileName,DataFileName);
        fflush(gnuplotPipe);
        DataFile = fopen(DataFileName,"w");
        for(int i=0;i<prd; i++)
            fprintf(DataFile," %d %f %f %f\n",i,(*matrix)[0][i],(*matrix)[1][i], (*matrix)[2][i]);
        fclose(DataFile);
        printf("press enter to continue...");
        getchar();
        remove(DataFileName);
        fprintf(gnuplotPipe,"exit \n");
    }
    else
        printf("gnuplot not found...");
}

void plotResults(vector <double>* TS)
{
    getchar();
    int si_ze = (int)TS->size();
    FILE *gnuplotPipe,*DataFile;
    const char *DataFileName;
    DataFileName = "Plots";
    gnuplotPipe = popen("/opt/local/bin/gnuplot","w"); // Address to GNU Plot
    
    if (gnuplotPipe)
    {
        fprintf(gnuplotPipe, "plot \"%s\" using 2 title 'Time Series' lw 2 with linespoints\n", DataFileName);
        fflush(gnuplotPipe);
        DataFile = fopen(DataFileName,"w");
        
        for(int i=0; i<si_ze; i++)
        {
            fprintf(DataFile," %d %f\n",i,(*TS)[i]); // Derefencing Pointer to Print Plot
        }
        fclose(DataFile);
        printf("press enter to continue...");
        getchar();
        remove(DataFileName);
        fprintf(gnuplotPipe,"exit \n");
    }
    
    else
    {
        printf("GNU Plot not Found");
    }
}
#endif /* gnuplot_h */
