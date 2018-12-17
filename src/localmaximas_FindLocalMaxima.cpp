/*
This function processes an R vector of file handles into a list of coordinates of 
local maxima within each file. OpenMP is used in an attempt to minimise the program runtime.
*/


//#include <omp.h>
#include "localmaximas.h"


//[[Rcpp::export]]
Rcpp::List FindLocalMaxima(Rcpp::List handles, double radius){
	
	// Allocate memory for output
	int numPts;
	Rcpp::List maxima(handles.length()); 
	Rcpp::NumericMatrix coords, maximaCoords;
	Rcpp::IntegerVector idxMaxima;
	RasterData data;


	/* Loop over the files:
		1./ Get the raster coordinates,
		2./ Get the CHM heights,
		3./ Remove CHM points with missing data,
		4./ Use fixed-radius NN search to find maxima,
		5./ Store xyz-coordinates in list
	*/
	for (int i = 0; i < handles.length(); i++){		

		// Get data
		data = ReadDataset(handles[i]);
		numPts = data.XSize * data.YSize;

		// Make the coordinates
		coords = SetCoordinates(data);


		// Remove the missing values
		RemoveMissingZ(data, coords);


		// frNN search for local maxima
		idxMaxima = SearchNeighbours(coords, data.z, radius);


		// Output
		Rcpp::NumericMatrix maximaCoords(idxMaxima.length(), 3);
		coords = subset_matrix_rows(coords, idxMaxima);
		data.z = data.z[idxMaxima];
		maximaCoords(Rcpp::_, 0) = coords(Rcpp::_, 0);
		maximaCoords(Rcpp::_, 1) = coords(Rcpp::_, 1);
		maximaCoords(Rcpp::_, 2) = data.z;

		maxima[i] = maximaCoords;
	}
	

	return maxima;
}

