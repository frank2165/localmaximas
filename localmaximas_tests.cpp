/*

This file wraps the various localmaximas package internal functions so that their output
may be checked in R.

*/

#include "localmaximas.h"
#include "localmaximas_tests.h"

/*
#ifdef __cplusplus
extern "C"{
#endif
*/

// [[Rcpp::export]]
Rcpp::NumericMatrix test_get_coordinates(Rcpp::S4 handle){

	SEXP sxpPts;
	Rcpp::NumericMatrix rcppPts;
	int rows, cols;
	SEXP sxpHandle = Rcpp::wrap(handle);
	ANNpointArray pts = get_coordinates(sxpHandle);


	rows = sizeof(pts) / sizeof(pts[0]);
	cols = sizeof(pts[0]) / sizeof(pts[0][0]);
	sxpPts = Rf_protect(Rf_allocVector(REALSXP, rows * cols));

	for (int j = 0; j < cols; j++){
		for (int i = 0; i < rows; i++){
			REAL(sxpPts)[i + rows*j] = pts[i][j];
		}
	}

	rcppPts = Rcpp::as<Rcpp::NumericMatrix>(sxpPts);

	Rf_unprotect(1);
	return rcppPts;
}

// [[Rcpp::export]]
Rcpp::NumericVector test_get_heights(Rcpp::S4 handle){

	SEXP sxpHeights;
	Rcpp::NumericVector rcppHeights;
	
	SEXP sxpHandle = Rcpp::wrap(handle);
	std::vector<double> heights = get_heights(sxpHandle);
	sxpHeights = Rcpp::wrap(heights);

	rcppHeights = Rcpp::as<Rcpp::NumericVector>(sxpHeights);
	return rcppHeights;
}

// [[Rcpp::export]]
Rcpp::IntegerVector test_index_missing(Rcpp::S4 handle, Rcpp::NumericVector rcppHeights){

	SEXP sxpIndex;
	Rcpp::IntegerVector rcppIndex;
	std::vector<double> heights = Rcpp::as<std::vector<double>>(rcppHeights);
	std::vector<int> idx;

	// Wrap handle
	SEXP sxpHandle = Rcpp::wrap(handle);

	idx = index_missing(sxpHandle, heights);
	sxpIndex = Rcpp::wrap(idx);
	rcppIndex = Rcpp::as<Rcpp::IntegerVector>(sxpIndex);

	return rcppIndex;
}

//Rcpp::List test_frNN_search(Rcpp::NumericMatrix points){}

/*
#ifdef __cplusplus
}
#endif
*/