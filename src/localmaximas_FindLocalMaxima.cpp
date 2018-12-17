/*
This function processes an R vector of file handles into a list of coordinates of 
local maxima within each file. OpenMP is used in an attempt to minimise the program runtime.
*/


#include <omp.h>
#include "localmaximas.h"


//[[Rcpp::export]]
Rcpp::List FindLocalMaxima(Rcpp::List handles, double radius){
	
	// Allocate memory for output
	int numPts = 0;
	Rcpp::List maxima(handles.length()); 
	Rcpp::NumericMatrix coords, maximaCoords;
	Rcpp::IntegerVector idxMaxima;
	RasterData data;

	int NumThreads = omp_get_max_threads();
	omp_set_num_threads(NumThreads);


	/* Loop over the files:
		1./ Get the raster coordinates,
		2./ Get the CHM heights,
		3./ Remove CHM points with missing data,
		4./ Use fixed-radius NN search to find maxima,
		5./ Store xyz-coordinates in list
	*/
#pragma omp parallel for shared(maxima, handles) private(numPts, coords, maximaCoords, idxMaxima, data) 
	for (int i = 0; i < handles.length(); i++){		

		// Get data
#pragma omp critical
		{
			data = ReadDataset(handles[i]); // I/O is serial, make sure only one thread is reading.
		}
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

