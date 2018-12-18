
#include "localmaximas.h"

arma::Mat<double> SetCoordinates(const RasterData &data) {

	unsigned int numPts = 0;
	double origin_x = data.origin_x, origin_y = data.origin_y;



	// Set origin coordinates to be the centroid of the first pixel
	origin_x += data.res_x / 2;
	origin_y += data.res_y / 2;



	// Allocate storage for coordinates
	numPts = data.XSize * data.YSize;
	arma::Mat<double> centres(numPts, 2);



	// Assign coordinates to each point
	int pt_idx = 0;
	double linedbl = 0.0;
	double pixeldbl = 0.0;
	for (int line = 0; line < data.YSize; line++) {
		linedbl = (double)line;
		for (int pixel = 0; pixel < data.XSize; pixel++) {
			pt_idx = pixel + line * data.XSize;
			pixeldbl = (double)pixel;

			centres(pt_idx, 0) = origin_x + pixeldbl * data.res_x;
			centres(pt_idx, 1) = origin_y + linedbl * data.res_y;
		}
	}


	return centres;
}
