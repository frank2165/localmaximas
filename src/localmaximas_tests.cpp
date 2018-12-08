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

	SEXP sxpPts;
	int rows, cols;
	ANNpointArray pts = get_coordinates(sxpHandle);


	rows = sizeof(pts) / sizeof(pts[0]);
	cols = sizeof(pts[0]) / sizeof(pts[0][0]);
	sxpPts = Rf_protect(Rf_allocMatrix(REALSXP, rows, cols));

	for (int j = 0; j < cols; j++){
		for (int i = 0; i < rows; i++){
			REAL(sxpPts)[i + rows*j] = pts[i][j];
		}
	}

	Rf_unprotect(1);
	return sxpPts;
}


// [[Rcpp::export]]
SEXP test_get_heights(SEXP sxpHandle){

	SEXP sxpHeights;
    
	std::vector<double> heights = get_heights(sxpHandle);
	sxpHeights = Rcpp::wrap(heights);

	return sxpHeights;
}


// [[Rcpp::export]]
SEXP test_index_missing(SEXP sxpHandle, SEXP sxpHeights){

    // Rf_xlen_t is a typedef for ptrdiff_t
    
	SEXP sxpIndex;
    R_xlen_t numPts = Rf_length(sxpHeights);
	std::vector<double> heights(numPts);
	std::vector<int> idx;
	
	for(int i = 0; i < numPts; i++){
	    heights[i] = REAL(sxpHeights)[i];
	}

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