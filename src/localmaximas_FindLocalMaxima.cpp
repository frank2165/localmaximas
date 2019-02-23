
#include <iostream>
#include "localmaximas.h"


//[[Rcpp::export]]
arma::Mat<double> FindLocalMaxima(SEXP sxpHandle, double radius){


	// Read data from file handle
	RasterData data = ReadDataset(sxpHandle);


	// Make the coordinates
	arma::Mat<double> coords = SetCoordinates(data);


	// Remove the missing values
	arma::Col<unsigned int> idxFinite = arma::find_finite(data.z);
	coords = coords.rows(idxFinite);
	data.z = data.z.rows(idxFinite);


	// frNN search for local maxima
	arma::Col<unsigned int> idxMaxima = SearchNeighbours(coords, data.z, radius);


	// Output
	coords = coords.rows(idxMaxima);
	data.z = data.z.rows(idxMaxima);
	coords.insert_cols(coords.n_cols, data.z);

	return coords;
}

