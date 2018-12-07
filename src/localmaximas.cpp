/*
Functions for interacting with the GDAL C++ library
*/

#include "localmaximas.h"



#ifdef __cplusplus
extern "C" {
#endif


ANNpointArray get_coordinates(SEXP sxpHandle){

	SEXP sxpGt, sxpXSize, sxpYSize;
	int XSize = 0, YSize = 0, NumPts = 0;
	double res_x = 0.0, res_y = 0.0;
	double origin_x = 0.0, origin_y = 0.0;
	ANNpointArray centres;


	// Get XSize and YSize for GDALReadOnlyDataset
	sxpXSize = RGDAL_GetRasterXSize(sxpHandle);
	sxpYSize = RGDAL_GetRasterYSize(sxpHandle);
	XSize = Rf_asInteger(sxpXSize);
	YSize = Rf_asInteger(sxpYSize);
	NumPts = XSize * YSize;


	Rprintf("XSize: %i, YSize: %i\n", XSize, YSize);


	// Get coordinate information GDALReadOnlyDataset
	sxpGt = RGDAL_GetGeoTransform(sxpHandle);


	// Set coordinate information
	origin_x = REAL(sxpGt)[0];
	origin_y = REAL(sxpGt)[3];
	res_x = REAL(sxpGt)[1];
	res_y = REAL(sxpGt)[4];


	Rprintf("res_x: %g, res_y: %g\n", res_x, res_y);



	// Set origin coordinates to be the centroid of the first pixel
	origin_x += res_x / 2;
	origin_y += res_y / 2;

	Rprintf("origin_x_x: %g, origin_y: %g\n", origin_x, origin_y);


	// Allocate storage for coordinates
	centres = annAllocPts(NumPts, 2);


	// Assign coordinates to each point
	int pt_idx = 0;
	double linedbl = 0.0;
	double pixeldbl = 0.0;
	for (int line = 0; line < YSize; line++){
		linedbl = (double)line;
		for (int pixel = 0; pixel < XSize; pixel++){
			pt_idx = pixel + line * XSize;
			pixeldbl = (double)pixel;

			centres[pt_idx][0] = origin_x + pixeldbl * res_x;
			centres[pt_idx][1] = origin_y + linedbl * res_y;
		}
	}


	Rprintf("Assigned coordinates to each point\n");

	return centres;
}


std::vector<double> get_heights(SEXP sxpHandle){

	SEXP sxpRaster, sxpHeights;
	SEXP sxpXSize, sxpYSize;
	int XSize, YSize, numPts;
	std::vector<double> heights;


	// Get XSize and YSize
	sxpXSize = RGDAL_GetRasterXSize(sxpHandle);
	sxpYSize = RGDAL_GetRasterYSize(sxpHandle);
	XSize = Rf_asInteger(sxpXSize);
	YSize = Rf_asInteger(sxpYSize);
	numPts = XSize * YSize;

	Rprintf("XSize: %i, YSize: %i\n", XSize, YSize);
	

	// Set RGDAL_GetRasterData args
	SEXP sxpOffset = Rf_protect(Rf_allocVector(REALSXP, 2));
	SEXP sxpDimReg = Rf_protect(Rf_allocVector(INTSXP, 2));
	SEXP sxpDimOut = Rf_protect(Rf_allocVector(INTSXP, 2));
	SEXP sxpInterleave = Rf_protect(Rf_allocVector(REALSXP, 2));

	memset(REAL(sxpOffset), 0, 2 * sizeof(double));
	memset(REAL(sxpInterleave), 0, 2 * sizeof(double));

	INTEGER(sxpDimReg)[0] = XSize;
	INTEGER(sxpDimReg)[1] = YSize;
	sxpDimOut = sxpDimReg;


	Rprintf("Got Raster Data args\n");


	// Read raster data
	sxpRaster  = RGDAL_GetRasterBand(sxpHandle, Rf_ScalarInteger(0));
	sxpHeights = RGDAL_GetRasterData(sxpRaster, sxpDimReg, sxpDimOut, sxpInterleave);


	Rprintf("Got heights from Raster Band\n");


	// Store heights in output std::vector<double>
	heights.resize(numPts);
	double* pH = REAL(sxpHeights); // Recommended for long vectors
	for (int i = 0; i < numPts; i++){
		heights[i] = pH[i];
	}


	Rf_unprotect(4);
	return heights;
}


std::vector<int> index_missing(SEXP sxpHandle, std::vector<double> &heights){

	SEXP sxpRaster, sxpNoDataValue;
	std::vector<int> idxMissing;
	double ndv;


	/*
	Get the "no value" value from the file, GDAL's GetBandNoDataValue function
	returns a double, so it is assumed that RGDAL_GetBandNoDataValue returns a
	REALSXP.
	*/
	sxpRaster = RGDAL_GetRasterBand(sxpHandle, Rf_ScalarInteger(1));
	sxpNoDataValue = RGDAL_GetBandNoDataValue(sxpRaster);
	ndv = Rf_asReal(sxpNoDataValue);

	// Search heights for all occurrances of ndv.
	idxMissing = find_all(heights, ndv);

	return idxMissing;
}

/*
std::vector<std::vector<int>> frNN_search(ANNpointArray points, int numPts){

	// Create kd-tree
	ANNkdtree kdTree = new ANNkd_tree(points, numPts, 2); // Always going to be 2d

}

*/

#ifdef __cplusplus
}
#endif