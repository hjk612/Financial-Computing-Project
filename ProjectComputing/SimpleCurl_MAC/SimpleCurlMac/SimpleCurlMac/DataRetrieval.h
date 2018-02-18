#ifndef DataRetrieval_h
#define DataRetrieval_h
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
#include "curl/curl.h"
#include "data.h"
#include "read_csv.h"

class Data
{
protected:
    inline static vector<string> DatesR;
    inline static vector<double> ReturnsReference;
    string Ticker;
    string Start;
    string End;
    string sCookies,sCrumb;
    CURL *handle;
    CURLcode result;
    FILE *fp;
    const char outfilename[FILENAME_MAX] = "Output.txt";
    bool flag = false;
    vector<double> Close;
public:
    Data(string Ticker_):Ticker(Ticker_){curl_global_init(CURL_GLOBAL_ALL);handle = curl_easy_init();}
    virtual int GetData()=0;
    virtual ~Data(){curl_easy_cleanup(handle);curl_global_cleanup();}
};

class Stock:public Data
{
protected:
    string AnnDate;
    vector<string> Dates;
    vector<double> Returns;
    vector<double> AbReturns;
public:
    Stock():Data("Null"),AnnDate("Null"){Returns.reserve(60);AbReturns.reserve(60);Close.reserve(61);Dates.reserve(61);}
    Stock(string AnnDate_):Data(NULL){AnnDate=AnnDate_;Returns.reserve(60);AbReturns.reserve(60);Close.reserve(61);Dates.reserve(61);}
    Stock(string AnnDate_,string Ticker_):Data(Ticker_),AnnDate(AnnDate_){Returns.reserve(60);AbReturns.reserve(60);Close.reserve(61);Dates.reserve(61);}
    void SetTicker(string Name){Ticker = Name;Close.clear();Returns.clear();AbReturns.clear();Dates.clear();}
    void SetAnnDate(string AnnDate_){AnnDate=AnnDate_;}
    void CalculateReturns();
    vector<double> GetTimeSeries(){return Close;}
    vector<string> GetDates(){return Dates;}
    vector<double> GetReturns(){return Returns;}
    vector<double> GetAbReturns(){return AbReturns;}
    int GetData();
    virtual ~ Stock(){}
};
class Reference: public Data
{
public:
    Reference(string Ticker_):Data(Ticker_){}
    virtual ~Reference() {}
    void SetTicker(string Name){Ticker = Name;Close.clear();ReturnsReference.clear();DatesR.clear();}
    vector<string> DatesRef(){return DatesR;}
    vector<double> GetClose(){return Close;}
    int GetData();
    vector<double> GetReturns(){return ReturnsReference;}
    void CalculateReturns();
};
#endif /* DataRetrieval_h */

