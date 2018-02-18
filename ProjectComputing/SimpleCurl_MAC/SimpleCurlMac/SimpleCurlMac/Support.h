#ifndef Support_h
#define Support_h
#include "DataRetrieval.h"
#include "Earnings.h"

inline void StockDataYahoo(Stock *Obj,map<string,string> &BbgMap,map<string,TimeSeries>& DataMap)
{
    std::map<string,string>::iterator itr;
    for(itr=BbgMap.begin();itr!=BbgMap.end();itr++)
    {
        
        Obj->SetTicker(itr->first);
        Obj->SetAnnDate(itr->second);
        if(Obj->GetData()==0)
        {
            DataMap[itr->first] = TimeSeries(Obj->GetDates(),Obj->GetTimeSeries(),Obj->GetAbReturns());
        }
    }
}

#endif /* Support_h */
