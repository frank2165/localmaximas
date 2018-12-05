/*
Functions for interacting with the GDAL C++ library
*/

#include "localmaximas.h"



#ifdef __cplusplus
#extern "C" {
#endif


/*
void Rcpp_local_maximas(Rcpp::List handleList, Rcpp::NumericVector searchRadius){

double radius = searchRadius[0];


// I imagine that this loop is where the multi-threaded buggery will happen.
for(unsigned int i = 0; i < handleList.size(); i++){
find_local_maxima(handleList[i], radius);
}

return;
}
*/





//void find_local_maxima(SEXP sxpHandle, double radius){}


ANNpointArray get_coordinates(SEXP sxpHandle){

	SEXP sxpGt, sxpXSize, sxpYSize;
	int XSize = 0, YSize = 0, NumPts = 0;
	double res_x = 0.0, res_y = 0.0;
	double origin_x = 0.0, origin_y = 0.0;
	ANNpointArray centres;


	// Get XSize and YSize for GDALReadOnlyDataset
	sxpXSize = RGDAL_GetRasterXSize(sxpHandle);
	sxpYSize = RGDAL_GetRasterYSize(sxpHandle);
	XSize = sxpXSize[0];
	YSize = sxpYSize[0];
	NumPts = XSize * YSize;


	// Get coordinate information GDALReadOnlyDataset
	sxpGt = RGDAL_GetGeoTransform(sxpHandle);
	if (getAttrib(sxpGt, "CE_Failure")){
		stop("Could not get attributes from dataset handle!")
	}


	// Set coordinate information
	origin_x = sxpGt[0];
	origin_y = sxpGt[3];
	res_x = sxpGt[1];
	res_y = sxpGt[4];


	// Set origin coordinates to be the centroid of the first pixel
	origin_x += res_x / 2;
	origin_y += res_y / 2;


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
	XSize = sxpXSize[0];
	YSize = sxpYSize[0];
	numPts = XSize * YSize;

	// Set RGDAL_getRasterData args
	SEXP sxpOffset = PROTECT(allocVector(REALSXP, 2));
	SEXP sxpDimReg = PROTECT(allocVector(REALSXP, 2));
	SEXP sxpDimOut = PROTECT(allocVector(REALSXP, 2));
	SEXP sxpInterleave = PROTECT(allocVector(REALSXP, 2));

	memset(REAL(sxpOffset), 0, 2 * sizeof(double);
	memset(REAL(sxpInterleave), 0, 2 * sizeof(double);

	sxpDimReg[0] = XSize;
	sxpDimReg[1] = YSize;
	sxpDimOut = sxpDimReg;


	// Read raster data
	sxpRaster = RGDAL_getRasterBand(sxpHandle, 1);
	sxpHeights = RGDAL_getRasterData(sxpRaster, sxpDimReg, sxpDimOut, sxpInterleave);


	// Store heights in output std::vector<double>
	heights.resize(numPts);
	for (int i = 0; i < numPts; i++){
		heights[i] = sxpHeights[i];
	}


	UNPROTECT(4);
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
	sxpRaster = RGDAL_getRasterBand(sxpHandle, 1);
	sxpNoDataValue = RGDAL_GetBandNoDataValue(sxpRaster);
	ndv = sxpNoDataValue[0];

	// Search heights for all occurrances of ndv.
	// https://stackoverflow.com/questions/25846235/finding-the-indexes-of-all-occurrences-of-an-element-in-a-vector
	std::vector<double>::iterator it = heights.begin();
	while ((it = std::find_if(it, heights.end(), ndv)) != heights.end()){
		idxMissing.push_back(std::distance(heights.begin()), it);
		it++
	}

	return idxMissing;
}


//std::vector<std::vector<int>> frNN_search(ANNpointArray points){}

#ifdef __cplusplus
}
#endif