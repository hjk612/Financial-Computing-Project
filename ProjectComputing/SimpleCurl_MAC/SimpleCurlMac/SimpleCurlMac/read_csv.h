#ifndef read_csv_h
#define read_csv_h
#include <iostream>
#include <iomanip>
#include <fstream>
#include<map>
#include<cstdlib>
#include<ctime>
#include<sstream>
#include<algorithm>
#include<string>
#include "Earnings.h"
using namespace std;

inline void DateFormat(string &date)
{
    date.insert(4,"-");
    date.insert(7,"-");
}
inline int read_csv(map<string,string> &beat,map<string,string> &meet,map<string,string> &miss,map<string,BloombergData> &Stocks,double &tolerance)
{
    ifstream myFile("/Users/Hatim/Desktop/project_data-3.csv");
    if(myFile.is_open())
    {
        string aLine;
        getline(myFile,aLine);
        while(getline(myFile,aLine))
        {
            string Ticker, BbgEst, ActEps, date;
            double BBG, ACT;
            long Date;
            istringstream ss(aLine);
            getline(ss,Ticker,',');
            getline(ss,BbgEst,',');
            getline(ss,ActEps,',');
            getline(ss,date,'\n');
            BBG = atof(BbgEst.c_str());
            ACT = atof(ActEps.c_str());
            Date = atol(date.c_str());
            string sdate;
            sdate = to_string(Date);
            DateFormat(sdate);
            double temp = ACT/BBG;
            string category;
            if((temp>(1+(tolerance/100))))
            {
                category = "Beat";
                beat[Ticker]=sdate;
            }
            else if((temp<(1-(tolerance/100))))
            {
                category = "Miss";
                miss[Ticker]=sdate;
            }
            else
            {
                category = "Meet";
                meet[Ticker]=sdate;
            }
            
            BloombergData Stock(sdate,category,BBG,ACT);
            Stocks[Ticker] = Stock;
        }
        myFile.close();
    }
    else
    {
        return 1;
    }
    return 0;
}
inline void RandomSampler(map<string,string> BeatAll, map<string,string> MeetAll,map<string,string> MissAll,int&N,
                          vector<string> &Beat,vector<string> &Meet,vector<string> &Miss)
{
    srand(time(NULL));
    for(int i=0;i<N;i++)
    {
        auto it1 = BeatAll.begin();
        auto it2 = MeetAll.begin();
        auto it3 = MissAll.begin();
        advance(it1, rand() % BeatAll.size());
        advance(it2,rand() % MeetAll.size());
        advance(it3,rand() % MissAll.size());
        Beat.push_back(it1->first);
        Meet.push_back(it2->first);
        Miss.push_back(it3->first);
        BeatAll.erase(it1);
        MeetAll.erase(it2);
        MissAll.erase(it3);
    }
}


#endif /* read_csv_h */
