
#define R_NO_REMAP // Recommended


#include <iterator>

#include <R.h>
#include <Rinternals.h>
#include <Rcpp.h>

#include "rgdal.h"
#include "ANN.h"


struct centroids {
    ANNpointArray centres;
    int numPts;
};



centroids get_coordinates(SEXP sxpHandle);
Rcpp::NumericVector get_heights(SEXP sxpHandle);
//Rcpp::IntegerVector find_na(Rcpp::NumericVector x);
