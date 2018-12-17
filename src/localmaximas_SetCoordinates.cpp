
#include "localmaximas.h"

Rcpp::NumericMatrix SetCoordinates(RasterData &data) {

	int numPts = 0;
	double origin_x = 0.0, origin_y = 0.0;



	// Set origin coordinates to be the centroid of the first pixel
	origin_x += (data->res_x) / 2;
	origin_y += (data->res_y) / 2;



	// Allocate storage for coordinates
	numPts = (data->XSize) * (data->YSize);
	Rcpp::NumericMatrix centres(numPts, 2);



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
