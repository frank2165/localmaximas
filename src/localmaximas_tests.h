
#include "localmaximas.h"


#ifdef __cplusplus
extern "C" {
#endif


Rcpp::NumericMatrix test_get_coordinates(SEXP handle);
Rcpp::NumericVector test_get_heights(SEXP handle);
Rcpp::IntegerVector test_index_missing(SEXP handle, Rcpp::NumericVector heights);



#ifdef __cplusplus
}
#endif