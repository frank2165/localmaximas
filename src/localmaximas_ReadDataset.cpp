
#include "localmaximas.h"



RasterData ReadDataset(SEXP sxpHandle){

	RasterData data; // Need to make sure that memory is allocated on the heap (maybe you need a class?)
	SEXP sxpXSize, sxpYSize, sxpDimReg, sxpDimOut, sxpInterleave, sxpGt, sxpRasterPtr, sxpHeights;
	Rcpp::S4 RGDAL_GDALRasterBand("GDALRasterBand");



	// Get XSize and YSize
	sxpXSize = Rf_protect(RGDAL_GetRasterXSize(sxpHandle));
	sxpYSize = Rf_protect(RGDAL_GetRasterYSize(sxpHandle));
	data.XSize = Rf_asInteger(sxpXSize);
	data.YSize = Rf_asInteger(sxpYSize);



	// Set RGDAL_GetRasterData args
	sxpDimReg = Rf_protect(Rf_allocVector(INTSXP, 4));
	sxpDimOut = Rf_protect(Rf_allocVector(INTSXP, 2));
	sxpInterleave = Rf_protect(Rf_allocVector(INTSXP, 2));

	memset(INTEGER(sxpInterleave), 0, 2 * sizeof(int));

	INTEGER(sxpDimReg)[0] = 0;
	INTEGER(sxpDimReg)[1] = 0;
	INTEGER(sxpDimReg)[3] = data.XSize;
	INTEGER(sxpDimReg)[2] = data.YSize;
	INTEGER(sxpDimOut)[1] = data.XSize;
	INTEGER(sxpDimOut)[0] = data.YSize;


	// Read raster data
	sxpRasterPtr = Rf_protect(RGDAL_GetRasterBand(sxpHandle, Rf_ScalarInteger(1)));
	RGDAL_GDALRasterBand.slot("handle") = sxpRasterPtr;
	sxpHeights = Rf_protect(RGDAL_GetRasterData(RGDAL_GDALRasterBand, sxpDimReg, sxpDimOut, sxpInterleave));



	// Get coordinate information GDALReadOnlyDataset
	sxpGt = Rf_protect(RGDAL_GetGeoTransform(sxpHandle));


	// Set coordinate information
	data.origin_x = REAL(sxpGt)[0];
	data.origin_y = REAL(sxpGt)[3];
	data.res_x = REAL(sxpGt)[1];
	data.res_y = REAL(sxpGt)[5];



	// Convert REALSXP to Rcpp::NumericVector, strip dimension attribute (if it exists)
	data.z = Rcpp::wrap(sxpHeights);
	if (data.z.hasAttribute("dim")){
		data.z.attr("dim") = R_NilValue;
	}


	// Return
	sxpHandle = RGDAL_CloseDataset(sxpHandle);
	Rf_unprotect(8);
	return data;
}