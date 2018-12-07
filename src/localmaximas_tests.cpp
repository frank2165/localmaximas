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
SEXP test_get_coordinates(SEXP sxpHandle){

	Rprintf("Starting test_get_coordinates\n");

	SEXP sxpPts;
	int rows, cols;
	ANNpointArray pts = get_coordinates(sxpHandle);

	Rprintf("Got coordinates\n");

	rows = sizeof(pts) / sizeof(pts[0]);
	cols = sizeof(pts[0]) / sizeof(pts[0][0]);

	Rprintf("rows: %i, cols: %i \n", rows, cols);

	sxpPts = Rf_protect(Rf_allocVector(REALSXP, rows * cols));

	for (int j = 0; j < cols; j++){
		for (int i = 0; i < rows; i++){
			REAL(sxpPts)[i + rows*j] = pts[j][i]; //column major order?
		}
	}

	Rprintf("Assigned points to SEXP\n");

	Rf_unprotect(1);
	return sxpPts;
}

// [[Rcpp::export]]
SEXP test_get_heights(SEXP sxpHandle){

	Rprintf("Starting test_get_heights\n");

	SEXP sxpHeights;
	std::vector<double> heights = get_heights(sxpHandle);
	sxpHeights = Rcpp::wrap(heights);

	return sxpHeights;
}

// [[Rcpp::export]]
SEXP test_index_missing(SEXP sxpHandle, SEXP sxpHeights){

	Rprintf("Starting test_index_missing\n");

	SEXP sxpIndex;
	Rcpp::NumericVector rcppHeights = Rcpp::wrap(sxpHeights);
	std::vector<double> heights = Rcpp::as<std::vector<double>>(rcppHeights);
	std::vector<int> idx;

	Rprintf("Running index_missing\n");

	idx = index_missing(sxpHandle, heights);
	sxpIndex = Rcpp::wrap(idx);

	return sxpIndex;
}

//Rcpp::List test_frNN_search(Rcpp::NumericMatrix points){}

/*
#ifdef __cplusplus
}
#endif
*/
