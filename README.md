# Evaluate-the-Impact-of-Earning-Report-on-Stock-Price
In order to evaluate the impact of earning report on stock price, I did the following things:
• Use liburl to retrieve historical price data from Yahoo Finance: A function retrieves the adjusted close prices for selected Russell 1000 stocks and iShares Russell 100 (IWB) into memory.
• Parse the retrieved prices and store them in STL vector for each stock and IWB.
• Create a set of classes such as class for stock to handle EPS Estimate and price information.
• Use member functions or independent functions for all calculation. Overload a few arithmetic operators for vector/matrix.
• The stocks and their corresponding price information for each group are stored in a STL map, with stock symbol as its keys.
• The AAR and CAAR for 3 groups are presented in a matrix. The row of the matrix is the group#, matrix columns are for AAR and CAAR.
• Use gnuplot to show the CAAR from all 3 groups in one graph.
The program have a menu of 5 options:
o Retrieve historical price data for all stocks.
o Pull information for one stock from one group.
o Show AAR or CAAR for one group.
o Show the Excel graph with CAAR for all 3 groups.
o Exit the program.
