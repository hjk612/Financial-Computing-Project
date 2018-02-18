#include <stdio.h>
#include <string> 
#include <iostream>
#include <sstream>  
#include <vector>
#include <locale>
#include <iomanip>
#include<map>
#include<vector>
#include<stack>
#include<thread>
#include<unistd.h>
#include<chrono>
#include "curl/curl.h"
#include "data.h"
#include "read_csv.h"
#include "DataRetrieval.h"
#include "Support.h"
#include "Operator.h"
#include "gnuplot.h"
#include "Earnings.h"
using namespace std;

void DisplayMenu(int &num);
int main()
{
    int N;
    double tolerance;
    int Option;
    map<string,BloombergData> AllStocks;
    map<string,string> BeatAll;
    map<string,string> MeetAll;
    map<string,string> MissAll;
    vector<vector<double>> AllAARBeat;
    vector<vector<double>> AllAARMeet;
    vector<vector<double>> AllAARMiss;
    map<string,TimeSeries> MapTS;
    Stock *BeatStock = new Stock;
    Stock *MeetStock = new Stock;
    Stock *MissStock = new Stock;
    bool FlagData=false,FlagAAR = false;
    string Ref;
    cout<<"Enter the benchmark stock: ";
    cin>>Ref;cout<<endl;
    Reference * Benchmark = new Reference(Ref);
    if(Benchmark->GetData()==0)
    {cout<<"Benchmark data downloaded from Yahoo"<<endl;Benchmark->CalculateReturns();}
    vector<double> CloseRef = Benchmark->GetClose();
    vector<string> DatesRef = Benchmark->DatesRef();
    vector<double> ReturnsRef =  Benchmark->GetReturns();
    cout<<"\nEnter number of stocks for each category: ";
    cin>>N;cout<<endl;
    cout<<"Enter the tolerance level for categorisation: ";
    cin>>tolerance;cout<<endl;
    if(read_csv(BeatAll,MeetAll,MissAll,AllStocks,tolerance)==0)
        cout<<"Bloomberg data loaded successfully"<<endl;
    else
        cout<<"Could not read Bloomberg data"<<endl;
    auto start = chrono::steady_clock::now();
    std::thread beat (StockDataYahoo,(BeatStock),std::ref(BeatAll),std::ref(MapTS));
    std::thread meet (StockDataYahoo,(MeetStock),std::ref(MeetAll),std::ref(MapTS));
    std::thread miss (StockDataYahoo,(MissStock),std::ref(MissAll),std::ref(MapTS));
    beat.join();
    meet.join();
    miss.join();
    cout<<"\nData fetched"<<endl;
    auto end = chrono::steady_clock::now();
    cout<<"\nElapsed time in seconds:  "<<chrono::duration_cast<chrono::seconds>(end-start).count()<<" sec"<<endl;
    FlagData = true;
    int NSampling;
    cout<<"\nEnter the number of samplings you want to do: ";
    cin>>NSampling;cout<<endl;
    for(int i=0;i<NSampling;i++)
    {
        vector<vector<double>> BeatAbReturns;
        vector<vector<double>> MeetAbReturns;
        vector<vector<double>> MissAbReturns;
        BeatAbReturns.reserve(N);
        MeetAbReturns.reserve(N);
        MissAbReturns.reserve(N);
        vector<double> AARBeat;
        vector<double> AARMeet;
        vector<double> AARMiss;
        vector<string> Beat;
        vector<string> Meet;
        vector<string> Miss;
        RandomSampler(BeatAll, MeetAll, MissAll, N, Beat, Meet, Miss);
        std::map<string,TimeSeries>::iterator itr;
        for(int j=0;j<Beat.size();j++)
        {
            itr = MapTS.find(Beat[j]);
            BeatAbReturns.push_back((itr->second).GetAbReturns());
            itr = MapTS.find(Meet[j]);
            MeetAbReturns.push_back((itr->second).GetAbReturns());
            itr=MapTS.find(Miss[j]);
            MissAbReturns.push_back((itr->second).GetAbReturns());
        }
        AARBeat = ~BeatAbReturns;
        AARMeet = ~MeetAbReturns;
        AARMiss = ~MissAbReturns;
        AllAARBeat.push_back(AARBeat);
        AllAARMeet.push_back(AARMeet);
        AllAARMiss.push_back(AARMiss);
    }
    vector<double> FinalAARBeat = ~AllAARBeat;
    vector<double> FinalAARMeet = ~AllAARMeet;
    vector<double> FinalAARMiss = ~AllAARMiss;
    vector<vector<double>> Mat;
    Mat.push_back(FinalAARMiss);
    Mat.push_back(FinalAARBeat);
    Mat.push_back(FinalAARMeet);
    DisplayMenu(Option);

switchStart:
    switch(Option)
    {
        case 1:
        {
            if(FlagData==true)
            {
                string Tickr;
                cout<<"Enter the ticker of stock to be plotted: ";
                cin>>Tickr;
                std::map<string,BloombergData>::iterator iter;
                iter = AllStocks.find(Tickr);
                if(iter==AllStocks.end())
                {
                    cout<<"\nNo data from bloomberg for Data Retreival\n"<<endl;
                    goto switchStart;
                }
                std::map<string,TimeSeries>::iterator it;
                it = MapTS.find(Tickr);
                vector<string> DatesTickr;
                vector<double> CloseTickr;
                vector<double> ReturnsTickr;
                vector<double> AbReturnsTickr;
                DatesTickr.clear();
                CloseTickr.clear();
                if(it!=MapTS.end())
                {
                    cout<<"\nName: "<<Tickr<<endl;
                    cout<<"Category: "<<(iter->second).GetCategory()<<endl;
                    cout<<"Announcement Date: "<<(iter->second).GetAnnDate()<<endl;
                    cout<<"Bloomberg Estimate: "<<(iter->second).GetBbgEst()<<endl;
                    cout<<"Actual Earnings: "<<(iter->second).GetAPS()<<"\n"<<endl;
                    DatesTickr = (it->second).GetDates();
                    CloseTickr = (it->second).GetClose();
                    AbReturnsTickr = (it->second).GetAbReturns();
                    for(int i=0;i<DatesTickr.size();i++)
                        cout<<DatesTickr[i]<<" "<<CloseTickr[i]<<endl;
                    for(int i=0;i<AbReturnsTickr.size();i++)
                        cout<<AbReturnsTickr[i]<<endl;
                    plotResults(&CloseTickr);
                }
                DisplayMenu(Option);
                goto switchStart;
            }
            else
            {
                Option=0;
                goto switchStart;
            }

        }
       case 2:
        {
            if(FlagData==true)
            {
                plotresults(&Mat);
                FlagAAR=true;
                DisplayMenu(Option);
                goto switchStart;
            }
            else
            {
                cout<<"No data available"<<endl;
                cout<<"Fetch data first....."<<endl;
                Option=0;
                goto switchStart;
            }
        }
            
        case 3:
        {
            if((FlagData==true) && (FlagAAR==true))
            {
                vector<vector<double>> Matrix;
                vector<double> CAARBeat;
                vector<double> CAARMeet;
                vector<double> CAARMiss;
                double sum=0.0;
                for(int i =0;i<FinalAARBeat.size();i++)
                {
                    sum+=FinalAARBeat[i];
                    CAARBeat.push_back(sum);
                }
                sum=0.0;
                for(int i =0;i<FinalAARMeet.size();i++)
                {
                    sum+=FinalAARMeet[i];
                    CAARMeet.push_back(sum);
                }
                sum=0.0;
                for(int i =0;i<FinalAARMiss.size();i++)
                {
                    sum+=FinalAARMiss[i];
                    CAARMiss.push_back(sum);
                }
                Matrix.push_back(CAARMiss);
                Matrix.push_back(CAARBeat);
                Matrix.push_back(CAARMeet);
                plotresults(&Matrix);
                DisplayMenu(Option);
                goto switchStart;
            }
            else if(FlagData==false){
                Option=0;goto switchStart;}
            else
            {
                cout<<"AAR not calculated.....\n";
                Option=2;
                goto switchStart;
            }
        }
        case 4:
        {
            Option = 4;
            break;
        }
}
    delete BeatStock;
    delete MeetStock;
    delete MissStock;
    delete Benchmark;
    BeatStock = NULL;
    MeetStock = NULL;
    MissStock = NULL;
    Benchmark = NULL;
    return 0;
}
void DisplayMenu(int &num)
{
    cout<<"\n-------------Computing Project-------------"<<endl;
    cout<<"1. Plot time series"<<endl;
    cout<<"2. Plot AAR"<<endl;
    cout<<"3. Plot CAAR"<<endl;
    cout<<"4. Exit"<<endl;
    cin>>num;
}
