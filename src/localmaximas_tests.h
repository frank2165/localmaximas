
#include "localmaximas.h"


/* Apparently RcppExport is a macro for extern "C"
#ifdef __cplusplus
extern "C" {
#endif  
*/


Rcpp::NumericMatrix test_get_coordinates(Rcpp::S4 handle);
Rcpp::NumericVector test_get_heights(Rcpp::S4 handle);
Rcpp::IntegerVector test_index_missing(Rcpp:: S4 handle, Rcpp::NumericVector sxpHeights);


/*
#ifdef __cplusplus
}
#endif
*/