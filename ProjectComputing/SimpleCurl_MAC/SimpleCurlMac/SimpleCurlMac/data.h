#ifndef data_h
#define data_h
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
#include<string>
#include "DataRetrieval.h"

using namespace std;
struct MemoryStruct {
    char *memory;
    size_t size;
};
inline size_t write_data(void *ptr, int size, int nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}
inline void *myrealloc(void *ptr, size_t size)
{
    /* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}
inline size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;
    
    mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

inline string getTimeinSeconds(string Time)
{
    std::tm t = {0};
    std::istringstream ssTime(Time);
    char time[100];
    memset(time, 0, 100);
    if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
    {
        std::put_time(&t, "%c %Z");
        std::mktime(&t);
        sprintf (time, "%ld", mktime(&t));
        return string(time);
    }
    else
    {
        cout << "Parse failed\n";
        return "";
    }
}


#endif /* data_h */
