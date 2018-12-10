/*
// https://stackoverflow.com/questions/25846235/finding-the-indexes-of-all-occurrences-of-an-element-in-a-vector
*/

#include "localmaximas.h"


/* Use x[is_na(x)];
Rcpp::IntegerVector find_na(Rcpp::NumericVector x){
    Rcpp::IntegerVector index;
    Rcpp::NumericVector::iterator it = x.begin();
    
    while ((it = std::find_if(it, x.end(), Rcpp::is_na)) != x.end()){
        index.push_back(std::distance(x.begin(), it)); // C indexing!
        it++;
    }
    return index;
}
*/
