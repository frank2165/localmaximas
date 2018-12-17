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


// Structures
struct RasterData{
	int XSize = 0;
	int YSize = 0;
	double origin_x = 0.0;
	double origin_y = 0.0;
	double res_x = 0.0;
	double res_y = 0.0;
	Rcpp::NumericVector z;
};



// Utility functions
void rebase_index(Rcpp::IntegerVector &x);
bool check_maxima(const int, Rcpp::IntegerVector&, Rcpp::NumericVector&);
Rcpp::NumericMatrix subset_matrix_rows(Rcpp::NumericMatrix&, Rcpp::LogicalVector&);
Rcpp::NumericMatrix subset_matrix_rows(Rcpp::NumericMatrix&, Rcpp::IntegerVector&);


// Functionality
RasterData ReadDataset(SEXP sxpHandle);
Rcpp::NumericMatrix SetCoordinates(RasterData&);
void RemoveMissingZ(RasterData&, Rcpp::NumericMatrix);
Rcpp::IntegerVector SearchNeighbours(Rcpp::NumericMatrix xy, Rcpp::NumericVector z, double eps);


// Workflow
Rcpp::List FindLocalMaxima(Rcpp::List handles, double radius);