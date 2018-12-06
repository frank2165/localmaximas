


#include <Rcpp.h>
#include <algorithm>
#include "assert.h"
using namespace Rcpp;


// [[Rcpp::plugins("cpp11")]]


//LogicalVector maxima_search_cpp(List, NumericVector);
bool check_maxima_cpp(const int, IntegerVector, NumericVector);



bool check_maxima_cpp(const int point, IntegerVector neighbours,
                      NumericVector Z) {
    
    // neighbours should be an index of Z
    assert(max(neighbours) < Z.length());
    
    bool localmaxima;
    IntegerVector::iterator it;

    
    // Iterate through the list of neighbours and check whether the point is a maxima within its
    // neighbourhood. If a point is a maxima, then all other points in its neighbourhood cannot be
    // a maxima. Note however that points may reside in multiple neighbourhoods and so these points
    // cannot simply be discarded.
    it = std::find_if(neighbours.begin(), neighbours.end(), [&Z, &point](const int &i){
        bool strictLT = Z[point] < Z[i];
        bool isEQ = Z[point] == Z[i];
        bool idLT = i < point;
        return strictLT || (isEQ && idLT); 
    });
    
    localmaxima = (it == neighbours.end());
    return localmaxima;
}
