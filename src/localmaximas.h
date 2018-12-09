
#define R_NO_REMAP // Recommended
#ifndef LOCALMAXIMAS_H
#define LOCALMAXIMAS_H


#include <vector>
#include <iterator>
#include <algorithm>

#include <R.h>
#include <Rinternals.h>
#include <Rcpp.h>

#include "rgdal.h"
#include "ANN.h"



std::vector<int> find_all(std::vector<double> x, double val);

struct centroids {
    ANNpointArray centres;
    int numPts;
};


#ifdef __cplusplus
extern "C" {
#endif

centroids get_coordinates(SEXP sxpHandle);
std::vector<double> get_heights(SEXP sxpHandle);
std::vector<int> index_missing(SEXP sxpHandle, std::vector<double> &heights);


#ifdef __cplusplus
}
#endif

#endif