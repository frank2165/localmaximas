
#define R_NO_REMAP // Recommended


#include <vector>
#include <iterator>
#include <algorithm>

#include <R.h>
#include <Rinternals.h>
#include <Rcpp.h>

#include "rgdal.h"
#include "ANN.h"



std::vector<int> find_all(std::vector<double> x, double val);


#ifdef __cplusplus
extern "C" {
#endif

ANNpointArray get_coordinates(SEXP sxpHandle);
std::vector<double> get_heights(SEXP sxpHandle);
std::vector<int> index_missing(SEXP sxpHandle, std::vector<double> &heights);


#ifdef __cplusplus
}
#endif