#ifndef Earnings_h
#define Earnings_h
#include<string>
#include<vector>
using namespace std;

class BloombergData
{
protected:
    string AnnDate,category;
    double BbgEst, ActEPS;
public:
    BloombergData(){}
    BloombergData(string AnnDate_,string category_,double BbgEst_,double ActEPS_):AnnDate(AnnDate_),category(category_),BbgEst(BbgEst_),ActEPS(ActEPS_){}
    string GetCategory(){return category;}
    string GetAnnDate(){return AnnDate;}
    double GetBbgEst(){return BbgEst;}
    double GetAPS(){return ActEPS;}
};
class TimeSeries
{
protected:
    vector<string> Dates;
    vector<double> Close;
    vector<double> AbReturns;
public:
    TimeSeries(){}
    TimeSeries(vector<string> Dates_,vector<double> Close_,vector<double>AbReturns_):Dates(Dates_),Close(Close_),AbReturns(AbReturns_){}
    vector<string> GetDates(){return Dates;}
    vector<double> GetClose(){return Close;}
    vector<double> GetAbReturns(){return AbReturns;}
};

#endif /* Earnings_h */
