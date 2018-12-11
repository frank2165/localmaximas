
#include "localmaximas.h"

//[[Rcpp::export]]
Rcpp::NumericMatrix get_coordinates(SEXP sxpHandle) {

	SEXP sxpGt, sxpXSize, sxpYSize;
	int XSize = 0, YSize = 0;
	double res_x = 0.0, res_y = 0.0;
	double origin_x = 0.0, origin_y = 0.0;


	// Get XSize and YSize for GDALReadOnlyDataset
	sxpXSize = RGDAL_GetRasterXSize(sxpHandle);
	sxpYSize = RGDAL_GetRasterYSize(sxpHandle);
	XSize = Rf_asInteger(sxpXSize);
	YSize = Rf_asInteger(sxpYSize);


	// Get coordinate information GDALReadOnlyDataset
	sxpGt = RGDAL_GetGeoTransform(sxpHandle);


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

	return centres;
}
