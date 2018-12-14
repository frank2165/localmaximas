/* Notes:

+ A quick check in R reveals that the internal storage of a long numeric vector approaches
  8 Bytes of storage per 64-bit value being stored (theoretically it cannot be less than this
  without compression). For this reason the package is using the Rcpp wrappers, rather than 
  switching to std::vector<double>, as this provides a similar storage efficiency with the
  added benefit of the C++ container features.

+ std::find_if did not accept the Rcpp::is_na function, so advantage will be taken of the
  Rcpp logical subsetting.



*/




#define R_NO_REMAP // Recommended


#include <algorithm>
#include <iterator>
#include <assert.h>

#include <R.h>
#include <Rinternals.h>
#include <Rcpp.h>

#include "rgdal.h"
#include "ANN.h"


// Utility functions
void rebase_index(Rcpp::IntegerVector x);
bool check_maxima(const int, Rcpp::IntegerVector, Rcpp::NumericVector);
Rcpp::NumericMatrix subset_matrix_rows(Rcpp::NumericMatrix, Rcpp::LogicalVector);
Rcpp::NumericMatrix subset_matrix_rows(Rcpp::NumericMatrix, Rcpp::IntegerVector);


// Functionality
Rcpp::NumericMatrix get_coordinates(SEXP sxpFile);
Rcpp::NumericVector get_heights(SEXP sxpFile);
Rcpp::IntegerVector frNN_search(Rcpp::NumericMatrix xy, Rcpp::NumericVector z, double eps);


// Workflow
Rcpp::List find_local_maxima(Rcpp::CharacterVector files, double search_radius);