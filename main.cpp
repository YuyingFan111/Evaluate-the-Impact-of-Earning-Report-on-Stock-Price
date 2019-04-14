#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include "curl/curl.h"
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include "Calculation.hpp"

#include "GetStockPrice.hpp"
#include "Series.hpp"
#include "Sampler.hpp"
#include "TradingDays.hpp"
#include "Resources.hpp"
#include "Plot.hpp"
#include "deleteZeroPrice.hpp"

using namespace std;


int main() {
    char answer;
    char follow_answer;
    bool continue_menu = true;
    
    cout<<"**********************************************************\n";
    cout<<"1 - Retrieve historical data.\n";
    cout<<"2 - Pull information for one stock.\n";
    cout<<"3 - Show AAR and CAAR.\n";
    cout<<"4 - Plot the results.\n";
    cout<<"5 - Exit the program.\n";
    cout<<"Enter your choice and press return: ";
    
    while(continue_menu != false) {
        cin >> answer;
        switch (answer) {
            case '1':
            {
                cout<<"1 - Retrieve historical data.\n";
                
                /* Retrieve IWB total price and trading days from Yahoo Finance, save into 2 vectors.*/
                GetStockRawPrice("IWB",IWB_Total_price,"2016-01-01T16:00:00","2018-06-30T16:00:00");
                GetDates("IWB", IWB_Total_date, "2016-01-01T16:00:00", "2018-06-30T16:00:00");
                
                /* Save IWB price and trading days information into map.*/
                for (int i=0;i<IWB_Total_price.size();i++) {
                    IWB_Total_map[IWB_Total_date[i]]=IWB_Total_price[i];
                }
                
                /* Retrieve EPS information from csv file.*/
                ifstream readFile;
                /* Please put all files under data folder into your own debug folder first!*/
                readFile.open("RIY_Modified_Deleted.csv");
                if (!readFile) {
                    cout << "Fail to open file" << '\n';
                }
                
                /* Create a vector of series to store ticker and releaseDay information for all stocks.*/
                vector<Series> allStocks;
                vector<string> actual_;
                vector<string> est_;
                string line;
                getline(readFile,line);
                while(getline(readFile,line)) {
                    string ticker = line.substr(0,line.find(","));
                    string temp = line.substr(line.find(",")+1,line.size());//actual
                    actual_.push_back(temp.substr(0,temp.find(",")));
                    string temp1 = temp.substr(temp.find(",")+1,temp.size());//est
                    est_.push_back(temp1.substr(0,temp1.find(",")));
                    string releaseDay = temp1.substr(temp1.find(",")+1,temp1.size());
                    double temp_actual = atof(temp.c_str());
                    double temp_est = atof(temp1.c_str());
                    vector<double> temp_EPS;
                    temp_EPS.push_back(temp_actual);
                    temp_EPS.push_back(temp_est);
                    
                    Series temp_Series(ticker, releaseDay,temp_EPS,temp_actual,temp_est);
                    allStocks.push_back(temp_Series);
                }
                
                
                /* Divide all stocks into 3 groups by its EPS performance.*/
                vector<Series> group_Below;
                vector<Series> group_Mid;
                vector<Series> group_Above;
                
                for (int i = 0; i<allStocks.size();i++) {
                    /* Evaluate performance of each stock by calculate how actual eps beats estimate.*/
                    double performance = (allStocks[i].GetPrice()[0]-allStocks[i].GetPrice()[1])/abs(allStocks[i].GetPrice()[1]); //take abs for denominators
                    if (performance>=0.05)
                        group_Above.push_back(allStocks[i]);
                    else if (performance<0.05 && performance>=-0.05)
                        group_Mid.push_back(allStocks[i]);
                    else if (performance<-0.05)
                        group_Below.push_back(allStocks[i]);
                    else
                        cout << allStocks[i].GetTicker() << " has data issue" << endl;
                }
                
                /* The "price" vector was used to store EPS for group dividing. Now we delete the EPS data to avoid mistake or confusion.*/
                for(int i=0;i<group_Above.size();i++) {
                    group_Above[i].ClearPrice();
                }
                for(int i=0;i<group_Mid.size();i++) {
                    group_Mid[i].ClearPrice();
                }
                for(int i=0;i<group_Below.size();i++) {
                    group_Below[i].ClearPrice();
                }
                
                /* Assign value of ticker, releast day, starting trading day, ending trading day, price, and corresponding IWB to each stock in all three groups.*/
                populate_group_info(group_Above);
                populate_group_info(group_Mid);
                populate_group_info(group_Below);
                
                
                /* Delete stocks that have insufficient information (less than 241 pieces of price data).*/
                delete_invalid_stock(group_Above);
                delete_invalid_stock(group_Mid);
                delete_invalid_stock(group_Below);
                
                /* Delete stocks with zero prices.*/
                SpecialDelete(0, group_Above);
                SpecialDelete(0, group_Mid);
                SpecialDelete(0, group_Below);
                
                
                /* Save all groups into one map for menu use.*/
                vector<Series>All_Stock_Info;
                for(int i=0;i<group_Above.size();i++)
                    All_Stock_Info.push_back(group_Above[i]);
                for(int i=0;i<group_Mid.size();i++)
                    All_Stock_Info.push_back(group_Mid[i]);
                for(int i=0;i<group_Below.size();i++)
                    All_Stock_Info.push_back(group_Below[i]);
                
                map<string,Series>mymap_menu;
                vector<Series>::iterator itr_map_menu;
                for(itr_map_menu=All_Stock_Info.begin();itr_map_menu!=All_Stock_Info.end();itr_map_menu++) {
                    mymap_menu[(*itr_map_menu).GetTicker()]=*itr_map_menu;
                }
                
                map<string,vector<double> > mymap_Above;
                map<string,vector<double> > mymap_Mid;
                map<string,vector<double> > mymap_Below;
                
                /* Populate stocks information into maps.*/
                populate_into_map(group_Above, mymap_Above);
                populate_into_map(group_Mid, mymap_Mid);
                populate_into_map(group_Below, mymap_Below);
                
                
                Matrix2 CAAR_Above_matrix;
                Matrix2 CAAR_Mid_matrix;
                Matrix2 CAAR_Below_matrix;
                Matrix2 AAR_Above_matrix;
                Matrix2 AAR_Mid_matrix;
                Matrix2 AAR_Below_matrix;
                
                
                /* Bootstraping for 5 times.*/
                for (int repeatCount=0; repeatCount<5; repeatCount++){
                    
                    vector<Series> group_Above_100;
                    vector<Series> group_Mid_100;
                    vector<Series> group_Below_100;
                    
                    /* Randomly select 100 stocks for each group.*/
                    vector<int> index_Above = Sampler::GenerateIndice(100, (int)group_Above.size());
                    vector<int> index_Mid = Sampler::GenerateIndice(100, (int)group_Mid.size());
                    vector<int> index_Below = Sampler::GenerateIndice(100, (int)group_Below.size());
                    
                    /* Populate sample vector from each group given the index of selected stock.*/
                    populate_sample(group_Above, index_Above, group_Above_100);
                    populate_sample(group_Mid, index_Mid, group_Mid_100);
                    populate_sample(group_Below, index_Below, group_Below_100);
                    
                    Matrix2 AR_group_Below_100;
                    Matrix2 AR_group_Mid_100;
                    Matrix2 AR_group_Above_100;
                    
                    /* Calculate AR Matrix for each sample.*/
                    populate_AR_sample(group_Above_100, AR_group_Above_100);
                    populate_AR_sample(group_Mid_100, AR_group_Mid_100);
                    populate_AR_sample(group_Below_100, AR_group_Below_100);
                    
                    vector<double> AAR_Above;
                    vector<double> AAR_Mid;
                    vector<double> AAR_Below;
                    
                    /* Calculate AAR vector for each group.*/
                    AAR_Above=AAR(AR_group_Above_100);
                    AAR_Mid=AAR(AR_group_Mid_100);
                    AAR_Below=AAR(AR_group_Below_100);
                    
                    /* Integrate all AAR vectors generated from bootstrapping into Matrix for further calculation.*/
                    AAR_Above_matrix.push_back(AAR_Above);
                    AAR_Mid_matrix.push_back(AAR_Mid);
                    AAR_Below_matrix.push_back(AAR_Below);
                    
                    vector<double> CAAR_Above;
                    vector<double> CAAR_Mid;
                    vector<double> CAAR_Below;
                    
                    /* Calculate CAAR vector for each group.*/
                    CAAR_Above=CAAR(AAR_Above);
                    CAAR_Mid=CAAR(AAR_Mid);
                    CAAR_Below=CAAR(AAR_Below);
                    
                    /* Integrate all CAAR vectors generated from bootstrapping into Matrix for further calculation.*/
                    CAAR_Above_matrix.push_back(CAAR_Above);
                    CAAR_Mid_matrix.push_back(CAAR_Mid);
                    CAAR_Below_matrix.push_back(CAAR_Below);
                    
                }
                
                vector<double> CAAR_Above_avg;
                vector<double> CAAR_Mid_avg;
                vector<double> CAAR_Below_avg;
                vector<double> AAR_Above_avg;
                vector<double> AAR_Mid_avg;
                vector<double> AAR_Below_avg;
                
                /* Calculate the overall AAR after bootstraping.*/
                AAR_Above_avg = AAR(AAR_Above_matrix);
                AAR_Mid_avg = AAR(AAR_Mid_matrix);
                AAR_Below_avg = AAR(AAR_Below_matrix);
                
                /* Calculate the overall CAAR after bootstraping.*/
                CAAR_Above_avg = AAR(CAAR_Above_matrix);
                CAAR_Mid_avg = AAR(CAAR_Mid_matrix);
                CAAR_Below_avg = AAR(CAAR_Below_matrix);
                
                
                vector<Matrix2> final_AAR_CAAR;
                
                /* Present AAR and CAAR for three groups in a matrix.*/
                populate_final_AAR_CAAR(AAR_Above_avg, CAAR_Above_avg, final_AAR_CAAR);
                populate_final_AAR_CAAR(AAR_Mid_avg, CAAR_Mid_avg, final_AAR_CAAR);
                populate_final_AAR_CAAR(AAR_Below_avg, CAAR_Below_avg, final_AAR_CAAR);
                
                
                int nIntervals = 239;
                double x0 = 0.0;
                double* xData = (double*) malloc((nIntervals+1)*sizeof(double));
                double* yBeat = (double*) malloc((nIntervals+1)*sizeof(double));
                double* yMeet = (double*) malloc((nIntervals+1)*sizeof(double));
                double* yMiss = (double*) malloc((nIntervals+1)*sizeof(double));
                xData[0] = -119.0;
                
                /* Store all data from three groups for plotResults use.*/
                plot_data_manager(final_AAR_CAAR, nIntervals, x0, xData, yBeat, yMeet, yMiss);
                
                cout<<"**********************************************************\n";
                cout<<"Successfully retrieved all stock information!\n";
                
                
                
                /* Menu. */
                cout<<"Enter your choice and press return: ";
                while(continue_menu != false) {
                    cin >> follow_answer;
                    switch(follow_answer) {
                        case '1':
                        {
                            cout<<"Already retrieved historical data, please select other options.\n";
                        }
                            break;
                        case '2':
                        {
                            cout<<"2 - Pull information for one stock.\n";
                            string StockSymbol;
                            cout<<"Please enter stock symbol: ";
                            cin>>StockSymbol;
                            
                            //map<string,Series> itr_menu;
                            auto itr_menu = mymap_menu.find(StockSymbol);
                            
                            if (itr_menu != mymap_menu.end())
                            {
                                cout<<"Stock Ticker: "<<itr_menu->second.GetTicker()<<endl;
                                cout<<"Actual_EPS: "<<itr_menu->second.GetEPS_actual()<<endl;
                                cout<<"Estimated_EPS: "<<itr_menu->second.GetEPS_estimated()<<endl;
                                cout<<"Get EPS Release Day: "<<itr_menu->second.GetReleaseDay()<<endl;
                                cout<<"Get the Start Day for Calculation: "<<itr_menu->second.GetStartDay()<<endl;
                                cout<<"Get the End Day for Calculation: "<<itr_menu->second.GetEndDay()<<endl;
                                cout<<"Get the 241 Days' Prices for the Selected Stock: "<<endl;
                                
                                for(int i=0;i<itr_menu->second.GetPrice().size();i++)
                                    cout<<itr_menu->second.GetPrice()[i]<<"  ";
                                cout<<endl;
                            }
                            else
                            {
                                cout<<"Invalid input...."<<endl;
                            }
                        }
                            break;
                            
                        case '3':
                        {
                            cout<<"3 - Show AAR and CAAR.\n";
                            
                            char groupNum;
                            
                            cout<<"1 for Beat Group"<<endl;
                            cout<<"2 for Meet Group"<<endl;
                            cout<<"3 for Miss Group"<<endl;
                            cout<<"Please input the group number: ";
                            cin>>groupNum;
                            
                            if(groupNum=='1')
                            {
                                cout<<"The ARR for Beat Group is: "<<endl;
                                for(int i=0;i<AAR_Above_avg.size();i++)
                                    cout<<AAR_Above_avg[i]<<"  ";
                                cout<<endl;
                                
                                cout<<"The CARR for Beat Group is: "<<endl;
                                for(int i=0;i<CAAR_Above_avg.size();i++)
                                    cout<<CAAR_Above_avg[i]<<"  ";
                                cout<<endl<<endl;
                            }
                            
                            if(groupNum=='2')
                            {
                                cout<<"The ARR for Meet Group is: "<<endl;
                                for(int i=0;i<AAR_Mid_avg.size();i++)
                                    cout<<AAR_Mid_avg[i]<<"  ";
                                cout<<endl;
                                
                                cout<<"The CARR for Meet Group is: "<<endl;
                                for(int i=0;i<CAAR_Mid_avg.size();i++)
                                    cout<<CAAR_Mid_avg[i]<<"  ";
                                cout<<endl<<endl;
                            }
                            
                            if(groupNum=='3')
                            {
                                cout<<"The ARR for Miss Group is: "<<endl;
                                for(int i=0;i<AAR_Below_avg.size();i++)
                                    cout<<AAR_Below_avg[i]<<"  ";
                                cout<<endl;
                                
                                cout<<"The ARR for Miss Group is: "<<endl;
                                for(int i=0;i<CAAR_Below_avg.size();i++)
                                    cout<<CAAR_Below_avg[i]<<"  ";
                                cout<<endl<<endl;
                            }
                        }
                            break;
                        case '4':
                        {
                            cout<<"4 - Plot the results.\n";
                            plotResults(xData, yBeat, yMeet, yMiss, nIntervals);
                        }
                            break;
                        case '5':
                            cout<<"5 - Exit the program.\n";
                            /* Avoid memory leaking.*/
                            free(xData);
                            free(yBeat);
                            free(yMeet);
                            free(yMiss);
                            xData = NULL;
                            yBeat = NULL;
                            yMeet = NULL;
                            yMiss = NULL;
                            continue_menu = false;
                            exit(1);
                            
                        default:
                            cout<<"Invalid input!\n";
                            break;
                    }
                    cout<<"Enter your choice and press return: ";
                }
                
            }
                break;
            case '2':
                cout<<"Must retrieve data first!\n";
                cout<<"Enter your choice and press return: ";
                break;
                
            case '3':
                cout<<"Must retrieve data first!\n";
                cout<<"Enter your choice and press return: ";
                break;
            case '4':
                cout<<"Must retrieve data first!\n";
                cout<<"Enter your choice and press return: ";
                break;
            case '5':
                cout<<"5 - Exit the program.\n";
                
                continue_menu = false;
                exit(1);
            default:
                cout<<"Invalid Input!\n";
                cout<<"Enter your choice and press return: ";
                break;
        }
    }
    return 0;
}
