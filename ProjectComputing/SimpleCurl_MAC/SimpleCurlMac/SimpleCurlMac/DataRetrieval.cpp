#include "DataRetrieval.h"
#include "data.h"
#include<iostream>
using namespace std;


int Reference::GetData()
{
    Start = getTimeinSeconds("2017-02-01");
    End = getTimeinSeconds("2017-11-25");
    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;
    /* if everything's all right with the easy handle... */
    if (handle)
    {
        if (sCookies.length() == 0 || sCrumb.length() == 0)
        {
            fp = fopen(outfilename, "w");
            curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AAPL/history");
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
            curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
            curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
            
            curl_easy_setopt(handle, CURLOPT_ENCODING, "");
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
            result = curl_easy_perform(handle);
            fclose(fp);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occurred, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
            
            /* perform, then store the expected code in 'result'*/
            result = curl_easy_perform(handle);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occured, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            
            char cKey[] = "CrumbStore\":{\"crumb\":\"";
            char *ptr1 = strstr(data.memory, cKey);
            char *ptr2 = ptr1 + strlen(cKey);
            char *ptr3 = strstr(ptr2, "\"}");
            if ( ptr3 != NULL )
                *ptr3 = NULL;
            
            sCrumb = ptr2;
            
            fp = fopen("cookies.txt", "r");
            char cCookies[100];
            if (fp) {
                while (fscanf(fp, "%s", cCookies) != EOF);
                fclose(fp);
            }
            else
                cerr << "cookies.txt does not exists" << endl;
            
            sCookies = cCookies;
            free(data.memory);
            data.memory = NULL;
            data.size= 0;
        }
        string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
        string symbol = Ticker;
        string urlB = "?period1=";
        string urlC = "&period2=";
        string urlD = "&interval=1d&events=history&crumb=";
        string url = urlA + symbol + urlB + Start + urlC + End + urlD + sCrumb;
        const char * cURL = url.c_str();
        if(flag==false){
            const char * cookies = sCookies.c_str();
            curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);flag=true;}
        curl_easy_setopt(handle, CURLOPT_URL, cURL);
        /* Check for errors */
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
        result = curl_easy_perform(handle);
        /* Check for errors */
        if (result != CURLE_OK)
        {
            /* if errors have occurred, tell us what is wrong with 'result'*/
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }
    
    istringstream sData;
    sData.str(data.memory);
    string line;
    string date, price;
    double Price;
    for(;getline(sData,line);)
    {
        getline(sData,line,',');
        date = line;
        getline(sData,line,',');
        getline(sData,line,',');
        getline(sData,line,',');
        getline(sData,line,',');
        getline(sData,line,',');
        price = line;
        Price = atof(price.c_str());
        if(Price!=0.0)
        {
            DatesR.push_back(date);
            Close.push_back(Price);
        }
    }
    free(data.memory);
    data.size= 0;
    return 0;
}

int Stock::GetData()
{
    vector<string>::iterator it;
    it=find(DatesR.begin(),DatesR.end(),AnnDate);
    long pos = distance(DatesR.begin(), it);
    Start = getTimeinSeconds(DatesR[pos-31]);
    End = getTimeinSeconds(DatesR[pos+30]);
    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;
    /* if everything's all right with the easy handle... */
    if (handle)
    {
        if (sCookies.length() == 0 || sCrumb.length() == 0)
        {
            fp = fopen(outfilename, "w");
            curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AAPL/history");
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
            curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
            curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
            
            curl_easy_setopt(handle, CURLOPT_ENCODING, "");
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
            result = curl_easy_perform(handle);
            fclose(fp);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occurred, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
            
            /* perform, then store the expected code in 'result'*/
            result = curl_easy_perform(handle);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occured, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            
            char cKey[] = "CrumbStore\":{\"crumb\":\"";
            char *ptr1 = strstr(data.memory, cKey);
            char *ptr2 = ptr1 + strlen(cKey);
            char *ptr3 = strstr(ptr2, "\"}");
            if ( ptr3 != NULL )
                *ptr3 = NULL;
            
            sCrumb = ptr2;
            
            fp = fopen("cookies.txt", "r");
            char cCookies[100];
            if (fp) {
                while (fscanf(fp, "%s", cCookies) != EOF);
                fclose(fp);
            }
            else
                cerr << "cookies.txt does not exists" << endl;
            
            sCookies = cCookies;
            free(data.memory);
            data.memory = NULL;
            data.size= 0;
        }
        string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
        string symbol = Ticker;
        string urlB = "?period1=";
        string urlC = "&period2=";
        string urlD = "&interval=1d&events=history&crumb=";
        string url = urlA + symbol + urlB + Start + urlC + End + urlD + sCrumb;
        const char * cURL = url.c_str();
        if(flag==false){
            const char * cookies = sCookies.c_str();
            curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);flag=true;}
        curl_easy_setopt(handle, CURLOPT_URL, cURL);
        /* Check for errors */
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
        result = curl_easy_perform(handle);
        /* Check for errors */
        if (result != CURLE_OK)
        {
            /* if errors have occurred, tell us what is wrong with 'result'*/
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }
    
    istringstream sData;
    sData.str(data.memory);
    string line;
    string date, price;
    double Price;
    getline(sData, line);
    for(;getline(sData,line);)
    {
        getline(sData,line,',');
        date = line;
        getline(sData,line,',');
        getline(sData,line,',');
        getline(sData,line,',');
        getline(sData,line,',');
        getline(sData,line,',');
        price = line;
        Price = atof(price.c_str());
        if(Price!=0.0)
        {
            Dates.push_back(date);
            Close.push_back(Price);
        }
    }
    free(data.memory);
    data.size= 0;
    CalculateReturns();
    for(int i=0;i<Returns.size();i++)
    {
        AbReturns.push_back(Returns.at(i)-ReturnsReference.at(pos-30+i));
        
    }
    return 0;
}
void Reference::CalculateReturns()
{
    for(int i=1;i<Close.size();i++)
    {
        ReturnsReference.push_back((Close[i]-Close[i-1])/Close[i-1]);
    }
}
void Stock::CalculateReturns()
{
    for(int i=1;i<Close.size();i++)
    {
        Returns.push_back((Close[i]-Close[i-1])/Close[i-1]);
    }
}

