/*
This function processes an R vector of file handles into a list of coordinates of 
local maxima within each file. OpenMP is used in an attempt to minimise the program runtime.
*/


#include <omp.h>
#include "localmaximas.h"


static int MAXTHREADS = omp_get_max_threads();


//[[Rcpp::export]]
Rcpp::List FindLocalMaxima(Rcpp::List handles, double radius, int numCores){

	// Allocate memory for output
	Rcpp::List maxima(handles.length());


	if (numCores > MAXTHREADS){
		numCores = MAXTHREADS;
	}
	
	omp_set_num_threads(numCores);


	/* Loop over the files:
		1./ Get the raster coordinates,
		2./ Get the CHM heights,
		3./ Remove CHM points with missing data,
		4./ Use fixed-radius NN search to find maxima,
		5./ Store xyz-coordinates in list
		*/
#pragma omp parallel
	{

#pragma omp for
		for (int i = 0; i < handles.length(); i++){
			RasterData data;

			// Get data
#pragma omp critical
			{
				data = ReadDataset(handles[i]); // I/O is serial, make sure only one thread is reading. (THREAD-SAFETY!)
			}

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

#pragma omp critical 
			{
				maxima[i] = coords; // R & Rcpp API is not thread-safe.
			}
		}

	}
	

	return maxima;
}

