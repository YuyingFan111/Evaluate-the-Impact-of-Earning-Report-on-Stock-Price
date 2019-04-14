#ifndef Series_h
#define Series_h

#include <stdio.h>
#include <vector>
#include <string>
#include "Resources.hpp"
#include "Operations.hpp"
#include "TradingDays.hpp"

using namespace std;

class Series
{
protected:
    string ticker;
    string releaseDay;
    string startDay;
    string endDay;
    vector<double> rawPrices;
    vector<double> IWB;
    double EPS_actual;
    double EPS_estimated;
    
    
    
public:
    string GetTicker(){return ticker;}
    string GetReleaseDay(){return releaseDay;}
    string GetStartDay(){return startDay;}
    string GetEndDay(){return endDay;}
    double GetEPS_actual(){return EPS_actual;}
    double GetEPS_estimated(){return EPS_estimated;}
    vector<double> GetPrice(){return rawPrices;}
    vector<double> GetIWB(){return IWB;}
    
    void SetTicker(string ticker_){ticker=ticker_;}
    void SetReleaseDay(string releaseDay_){releaseDay=releaseDay_;}
    void SetStartDay(string startDay_){startDay = startDay_;}
    void SetEndDay(string endDay_){endDay = endDay_;}
    void SetPrice(vector<double>rawPrices_){rawPrices=rawPrices_;}
    void SetIWB()
    {
        map<string,double>::iterator itr;
        for(itr=IWB_Total_map.find(startDay);itr!=IWB_Total_map.find(endDay);itr++)
        {
            IWB.push_back(itr->second);
        }
        IWB.push_back(IWB_Total_map[endDay]);
        
    }

    vector<double> AR()
    {
        vector<double> stock_return;
        vector<double> IWB_return;
        vector<double> abnormal_R;
        for(int i=1;i<rawPrices.size();i++)
        {
            stock_return.push_back((rawPrices[i]-rawPrices[i-1])/rawPrices[i-1]);
            IWB_return.push_back((IWB[i]-IWB[i-1])/IWB[i-1]);
        }
        return stock_return-IWB_return;
    }
    
    
    Series(){};
    Series(string ticker_,string releaseDay_,vector<double> rawPrices_,double EPS_actual_, double EPS_estimated_)
    {
        ticker = ticker_;
        releaseDay = releaseDay_;
        rawPrices = rawPrices_;
        EPS_actual=EPS_actual_;
        EPS_estimated=EPS_estimated_;
    }
    Series(string ticker_, string startDay_, string endDay_, vector<double> rawPrices_)
    {
        ticker = ticker_;
        startDay = startDay_;
        endDay = endDay_;
        rawPrices = rawPrices_;
    }
    Series(string ticker_, string releaseDay_)
    {
        ticker = ticker_;
        releaseDay = releaseDay_;
    }
    void ClearPrice()
    {
        rawPrices.clear();
    }
    
    
    
};

/* Assign value of ticker, release day, starting trading day, ending trading day, price, and corresponding IWB to each stock in all three groups.*/
void populate_group_info(vector<Series> &group) {
    for(int i=0;i<group.size();i++) {
        string temp_ticker = group[i].GetTicker();
        string temp_release = group[i].GetReleaseDay();
        temp_release.substr(0,10);
        group[i].SetStartDay(TradeDays::GetStartTradeDay(temp_release));
        group[i].SetEndDay(TradeDays::GetEndTradeDay(temp_release));
        vector<double> temp_rawp;
        GetStockRawPrice(temp_ticker, temp_rawp, group[i].GetStartDay(), group[i].GetEndDay());
        group[i].SetPrice(temp_rawp);
        group[i].SetIWB();
    }
}

/* Delete stocks that have insufficient information (less than 241 pieces of price data).*/
void delete_invalid_stock(vector<Series> &group) {
    vector<Series>::iterator itr_delete;
    for(itr_delete = group.begin();itr_delete != group.end();itr_delete++) {
        if((*itr_delete).GetPrice().size()<241) {
            group.erase(itr_delete);
        }
    }
}

/* Populate stocks information into maps.*/
void populate_into_map(vector<Series> &group, map<string,vector<double> > &group_map) {
    vector<Series>::iterator itr_mymap;
    for(itr_mymap=group.begin();itr_mymap!=group.end();itr_mymap++) {
        group_map[itr_mymap->GetTicker()]=itr_mymap->GetPrice();
    }
}

/* Populate sample vector from each group given the index of selected stock.*/
void populate_sample(vector<Series> &group, vector<int> &index, vector<Series> &group_sample) {
    for(int i=0;i<group.size();i++) {
        vector<int>::iterator it = find(index.begin(), index.end(), i);
        if(it!=index.end())
            group_sample.push_back(group[i]);
    }
}

/* Calculate AR Matrix.*/
void populate_AR_sample(vector<Series> &group_sample, Matrix2 &AR_group_sample) {
    for(int i=0;i<group_sample.size();i++) {
        AR_group_sample.push_back(group_sample[i].AR());
    }
}

#endif
