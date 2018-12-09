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

    Rprintf("test_get_coordinates\n");
    
	SEXP sxpPts;
    centroids pts = get_coordinates(sxpHandle);
    
    Rprintf("Assigning rows and pA\n");
    
	int rows = pts.numPts;
	ANNpointArray pA = pts.centres;
	
	Rprintf("rows: %i \n", rows);
	
	sxpPts = Rf_protect(Rf_allocMatrix(REALSXP, rows, 2));
	
	Rprintf("sxpPts allocated\n");


	for (int j = 0; j < 2; j++){
		for (int i = 0; i < rows; i++){
			REAL(sxpPts)[i + rows*j] = pA[i][j];
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