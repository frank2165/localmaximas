/*
Functions for interacting with the GDAL C++ library
*/

#include "localmaximas.h"



//[[Rcpp::export]]
Rcpp::NumericVector get_heights(SEXP sxpFile){

	SEXP sxpHandle, sxpRasterPtr, sxpHeights;
	SEXP sxpXSize, sxpYSize;
	int XSize, YSize;
	Rcpp::NumericVector heights;


	// Open file connection
	if (Rf_isNull(sxpFile)){
		Rf_error("NULL filename passed to get_heights");
	}

	if (!Rf_isString(sxpFile)){
		Rf_error("Variable passed to get_heights was not a STRSXP");
	}

	Rprintf("get_heights: Opening dataset\n");
	Rf_PrintValue(sxpFile);
	sxpHandle = Rf_protect(RGDAL_OpenDataset(sxpFile, Rf_ScalarLogical(1), Rf_ScalarLogical(1), R_NilValue, R_NilValue));
	Rprintf("File Open\n");


	// Get XSize and YSize
	sxpXSize = Rf_protect(RGDAL_GetRasterXSize(sxpHandle));
	sxpYSize = Rf_protect(RGDAL_GetRasterYSize(sxpHandle));
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


	// Convert REALSXP to Rcpp::NumericVector, strip dimension attribute (if it exists)
	heights = Rcpp::wrap(sxpHeights);
	if (heights.hasAttribute("dim")){
		heights.attr("dim") = R_NilValue;
	}

	sxpHandle = RGDAL_CloseDataset(sxpHandle);
	Rf_unprotect(8);
	return sxpHeights;
}


//std::vector<std::vector<int>> frNN_search(ANNpointArray points){}
