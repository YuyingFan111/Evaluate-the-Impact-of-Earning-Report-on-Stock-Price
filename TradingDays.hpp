#ifndef TradingDays_hpp
#define TradingDays_hpp
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;
class TradeDays
{
public:
    
    static string forward(string theDate)
    {
        ifstream readFile_;
        readFile_.open("Calendar.csv");
        if (!readFile_)
        {
            cout << "Fail to open Calendar.csv" << endl;
        }
        vector<string> allDays;
        string line_;
        while(getline(readFile_,line_))
        {
            allDays.push_back(line_.substr(0,10));
        }
        vector<string>::iterator itr = find(allDays.begin(), allDays.end(), theDate);
        string temp = *(itr+1);
        return temp;
    }
    
    //Given the release day, return start day(-120 trading days) and end day (+120 trading days)
    static string GetStartTradeDay(string rlzDay)
    {
        rlzDay = rlzDay.substr(0,10);
        ifstream readFile;
        readFile.open("TradeDays.csv");
        if (!readFile)
        {
            cout << "Fail to open TradeDays.csv" << endl;
        }
        vector<string> allTradeDays;
        string line;
        while(getline(readFile,line))
        {
            allTradeDays.push_back(line.substr(0,10));
        }
        vector<string>::iterator it = find(allTradeDays.begin(), allTradeDays.end(), rlzDay);
//        int count = 0;
        while(it == allTradeDays.end()) // potential improvement
        {
            rlzDay = forward(rlzDay);
//            count++;
//            cout << count << "*"<< endl;
            it = find(allTradeDays.begin(), allTradeDays.end(), rlzDay);
        }
        string startDate_sec = *(it-120) ;
        return startDate_sec;
    }

    static string GetEndTradeDay(string rlzDay)
    {
        rlzDay = rlzDay.substr(0,10);
        ifstream readFile;
        readFile.open("TradeDays.csv");
        if (!readFile)
        {
            cout << "Fail to open TradeDays.csv" << endl;
        }
        vector<string> allTradeDays;
        string line;
        while(getline(readFile,line))
        {
            allTradeDays.push_back(line.substr(0,10));
        }
        vector<string>::iterator it_ = find(allTradeDays.begin(), allTradeDays.end(), rlzDay);
        while(it_ == allTradeDays.end()) // potential improvement
        {
            rlzDay = forward(rlzDay);
            it_ = find(allTradeDays.begin(), allTradeDays.end(), rlzDay);
        }
        string endDate_sec = *(it_+120);
        return endDate_sec;
    }


};




#endif /* TradingDays_hpp */
