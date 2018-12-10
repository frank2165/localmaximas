/*
Functions for interacting with the GDAL C++ library
*/

#include "localmaximas.h"



#ifdef __cplusplus
extern "C" {
#endif


centroids get_coordinates(SEXP sxpHandle){

    Rprintf("get_coordinates\n");
    
    centroids points;
    ANNpointArray centres;
	SEXP sxpGt, sxpXSize, sxpYSize;
	int XSize = 0, YSize = 0, numPts = 0;
	double res_x = 0.0, res_y = 0.0;
	double origin_x = 0.0, origin_y = 0.0;


	// Get XSize and YSize for GDALReadOnlyDataset
	sxpXSize = RGDAL_GetRasterXSize(sxpHandle);
	sxpYSize = RGDAL_GetRasterYSize(sxpHandle);
	XSize = Rf_asInteger(sxpXSize);
	YSize = Rf_asInteger(sxpYSize);
	numPts = XSize * YSize;

	Rprintf("XSize: %i, YSize: %i \n", XSize, YSize);
	Rprintf("numPts: %i \n", numPts);
	

	// Get coordinate information GDALReadOnlyDataset
	sxpGt = RGDAL_GetGeoTransform(sxpHandle);


	// Set coordinate information
	origin_x = REAL(sxpGt)[0];
	origin_y = REAL(sxpGt)[3];
	res_x = REAL(sxpGt)[1];
	res_y = REAL(sxpGt)[5];
	
	Rprintf("res_x: %g, res_y: %g \n", res_x, res_y);


	// Set origin coordinates to be the centroid of the first pixel
	origin_x += res_x / 2;
	origin_y += res_y / 2;
	
	Rprintf("origin_x: %g, origin_y: %g \n", origin_x, origin_y);


	// Allocate storage for coordinates
	centres = annAllocPts(numPts, 2);
	Rprintf("Memory allocated for ANNpointArray: %p\n", centres);


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
	
	Rprintf("Assigning members to struct: %p\n", &points);
	points.centres = centres;
	points.numPts = numPts;

	return points;
}


std::vector<double> get_heights(SEXP sxpHandle){

    Rprintf("get_heights\n");
    
	SEXP sxpRasterPtr, sxpHeights;
	SEXP sxpXSize, sxpYSize;
	int XSize, YSize, numPts;
	std::vector<double> heights;


	// Get XSize and YSize
	sxpXSize = RGDAL_GetRasterXSize(sxpHandle);
	sxpYSize = RGDAL_GetRasterYSize(sxpHandle);
	XSize = Rf_asInteger(sxpXSize);
	YSize = Rf_asInteger(sxpYSize);
	numPts = XSize * YSize;
	
	Rprintf("XSize: %i, YSize: %i \n", XSize, YSize);
	

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
	Rprintf("RGDAL_GetRasterBand\n");
	sxpRasterPtr = Rf_protect(RGDAL_GetRasterBand(sxpHandle, Rf_ScalarInteger(1)));
	Rf_PrintValue(sxpRasterPtr);

	Rcpp::S4 sxpGDALRasterBand("GDALRasterBand");
	Rf_PrintValue(sxpGDALRasterBand);

	sxpGDALRasterBand.slot("handle") = sxpRasterPtr;
	Rf_PrintValue(sxpGDALRasterBand);

	Rprintf("RGDAL_GetRasterData\n");
	sxpHeights = Rf_protect(RGDAL_GetRasterData(sxpGDALRasterBand, sxpDimReg, sxpDimOut, sxpInterleave));
	Rf_PrintValue(sxpHeights);

	// Store heights in output std::vector<double>
	heights.resize(numPts);
	double* pH = REAL(sxpHeights); // Recommended for long vectors
	for (int i = 0; i < numPts; i++){
		heights[i] = pH[i];
	}


	Rf_unprotect(5);
	return heights;
}


std::vector<int> index_missing(SEXP sxpHandle, std::vector<double> &heights){

	SEXP sxpRasterPtr, sxpNoDataValue;
	std::vector<int> idxMissing;
	double ndv;


	/*
	Get the "no value" value from the file, GDAL's GetBandNoDataValue function
	returns a double, so it is assumed that RGDAL_GetBandNoDataValue returns a
	REALSXP.
	*/
	Rprintf("RGDAL_GetRasterBand\n");
	sxpRasterPtr = Rf_protect(RGDAL_GetRasterBand(sxpHandle, Rf_ScalarInteger(1)));
	Rcpp::S4 sxpGDALRasterBand("GDALRasterBand");
	sxpGDALRasterBand.slot("handle") = sxpRasterPtr;


	sxpNoDataValue = Rf_protect(RGDAL_GetBandNoDataValue(sxpGDALRasterBand));
	ndv = Rf_asReal(sxpNoDataValue);
	Rprintf("ndv = %g", ndv);

	// Search heights for all occurrances of ndv.
	idxMissing = find_all(heights, ndv);


	Rf_unprotect(2);
	return idxMissing;
}


//std::vector<std::vector<int>> frNN_search(ANNpointArray points){}

#ifdef __cplusplus
}
#endif