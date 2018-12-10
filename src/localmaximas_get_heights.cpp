/*
Functions for interacting with the GDAL C++ library
*/

#include "localmaximas.h"



//[[Rcpp::export]]
Rcpp::NumericVector get_heights(SEXP sxpHandle){
    
	SEXP sxpRasterPtr, sxpHeights;
	SEXP sxpXSize, sxpYSize;
	int XSize, YSize;
	Rcpp::NumericVector heights;


	// Get XSize and YSize
	sxpXSize = RGDAL_GetRasterXSize(sxpHandle);
	sxpYSize = RGDAL_GetRasterYSize(sxpHandle);
	XSize = Rf_asInteger(sxpXSize);
	YSize = Rf_asInteger(sxpYSize);
	

	// Set RGDAL_GetRasterData args
	SEXP sxpDimReg = Rf_protect(Rf_allocVector(INTSXP, 4));
	SEXP sxpDimOut = Rf_protect(Rf_allocVector(INTSXP, 2));
	SEXP sxpInterleave = Rf_protect(Rf_allocVector(INTSXP, 2));

	memset(INTEGER(sxpInterleave), 0, 2 * sizeof(int));

	INTEGER(sxpDimReg)[0] = 0;
	INTEGER(sxpDimReg)[1] = 0;
	INTEGER(sxpDimReg)[3] = XSize;
	INTEGER(sxpDimReg)[2] = YSize;
	INTEGER(sxpDimOut)[1] = XSize;
	INTEGER(sxpDimOut)[0] = YSize;


	// Read raster data
	sxpRasterPtr = Rf_protect(RGDAL_GetRasterBand(sxpHandle, Rf_ScalarInteger(1)));
	Rcpp::S4 sxpGDALRasterBand("GDALRasterBand");
	sxpGDALRasterBand.slot("handle") = sxpRasterPtr;
	sxpHeights = Rf_protect(RGDAL_GetRasterData(sxpGDALRasterBand, sxpDimReg, sxpDimOut, sxpInterleave));


	// Convert REALSXP to Rcpp::NumericVector
	heights = Rcpp::wrap(sxpHeights);

	Rf_unprotect(5);
	return sxpHeights;
}


//std::vector<std::vector<int>> frNN_search(ANNpointArray points){}
