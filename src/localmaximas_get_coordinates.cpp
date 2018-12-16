
#include "localmaximas.h"

//[[Rcpp::export]]
Rcpp::NumericMatrix get_coordinates(SEXP sxpFile) {

	SEXP sxpHandle, sxpGt, sxpXSize, sxpYSize;
	int XSize = 0, YSize = 0;
	double res_x = 0.0, res_y = 0.0;
	double origin_x = 0.0, origin_y = 0.0;


	// Open file connection
	if (Rf_isNull(sxpFile)){
		Rf_error("NULL filename passed to get_coordinates");
	}

	if (!Rf_isString(sxpFile)){
		Rf_error("Variable passed to get_coordinates was not a STRSXP");
	}

	Rprintf("get_coordinates: Opening dataset\n");
	Rf_PrintValue(sxpFile);
	sxpHandle = Rf_protect(RGDAL_OpenDataset(sxpFile, Rf_ScalarLogical(1), Rf_ScalarLogical(1), R_NilValue, R_NilValue));
	Rprintf("File Open\n");


	// Get XSize and YSize for GDALReadOnlyDataset
	sxpXSize = Rf_protect(RGDAL_GetRasterXSize(sxpHandle));
	sxpYSize = Rf_protect(RGDAL_GetRasterYSize(sxpHandle));
	XSize = Rf_asInteger(sxpXSize);
	YSize = Rf_asInteger(sxpYSize);


	// Get coordinate information GDALReadOnlyDataset
	sxpGt = Rf_protect(RGDAL_GetGeoTransform(sxpHandle));


	// Set coordinate information
	origin_x = REAL(sxpGt)[0];
	origin_y = REAL(sxpGt)[3];
	res_x = REAL(sxpGt)[1];
	res_y = REAL(sxpGt)[5];


	// Set origin coordinates to be the centroid of the first pixel
	origin_x += res_x / 2;
	origin_y += res_y / 2;


	// Allocate storage for coordinates
	Rcpp::NumericMatrix centres(XSize*YSize, 2);


	// Assign coordinates to each point
	int pt_idx = 0;
	double linedbl = 0.0;
	double pixeldbl = 0.0;
	for (int line = 0; line < YSize; line++) {
		linedbl = (double)line;
		for (int pixel = 0; pixel < XSize; pixel++) {
			pt_idx = pixel + line * XSize;
			pixeldbl = (double)pixel;

			centres(pt_idx, 0) = origin_x + pixeldbl * res_x;
			centres(pt_idx, 1) = origin_y + linedbl * res_y;
		}
	}

	sxpHandle = RGDAL_CloseDataset(sxpHandle);
	Rf_unprotect(4);
	return centres;
}
