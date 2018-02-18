#ifndef Operator_h
#define Operator_h

vector<double> operator~(const vector<vector<double>> &MyMap)
{
    vector<double> Average;
    for(int i=0;i<MyMap[1].size();i++)
    {
        double temp=0.0;
        for(int j=0;j<MyMap.size();j++)
            temp+=MyMap[j][i];
        Average.push_back(temp/MyMap.size());
    }
    return Average;
}


#endif /* Operator_h */
