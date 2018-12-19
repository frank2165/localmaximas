/*
This function processes an R vector of file handles into a list of coordinates of 
local maxima within each file. OpenMP is used in an attempt to minimise the program runtime.
*/

#include <iostream>
#include <omp.h>
#include "localmaximas.h"


static int MAXTHREADS = omp_get_max_threads();


//[[Rcpp::export]]
Rcpp::List FindLocalMaxima(Rcpp::List handles, double radius, int numCores){

	if (numCores > MAXTHREADS){
		numCores = MAXTHREADS;
	}
	
	omp_set_num_threads(numCores);


	
	// Allocate memory for output
	int current_pos = 0;
	int numFiles = handles.length();
	std::vector<RasterData> dataList(numCores);
	std::vector<arma::Mat<double>> maxima(numFiles);
	std::vector<arma::Mat<double>>::iterator it = maxima.begin();
	// Rcpp::List maximaList;

	Rprintf("Files in dataset: %i\n", numFiles);

	while(it != maxima.end()) {
		current_pos = std::distance(maxima.begin(), it); // Does this add any actual safety or could you just update current pos at the end of the while loop?
		if ((current_pos + numCores) > numFiles) {
			numCores = numFiles - current_pos;
		}

		Rprintf( "Reading datasets: %i - %i\n", current_pos, current_pos + numCores-1);

		for (int j = 0; j < numCores; j++) {
			dataList[j] = ReadDataset(handles[current_pos + j]);
		}

#pragma omp parallel
		{

			RasterData data;

#pragma omp for private (data)
			for (int thread = 0; thread < numCores; thread++) {

				int threadID = omp_get_thread_num();

				data = dataList[threadID];

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
					std::cout << "Thread = " << threadID << " writing to maximas" << std::endl;
					std::cout << "Writing to maximas[" << std::distance(maxima.begin(), it) << "]" << std::endl;
					*(it + threadID) = coords;
					std::cout << "Write complete" << std::endl;
				}
			}

		}
		it += numCores;
		//current_pos += numCores;
	}
	

	Rprintf("Converting std::vector to Rcpp::List\n");

	// Explicitly convert the std::vector to Rcpp::List
	// maximaList = Rcpp::as<Rcpp::List>(Rcpp::wrap(maxima));
	Rcpp::List maximaList(maxima.size());
	for (unsigned int i = 0; i < maxima.size(); i++) {
		maximaList[i] = Rcpp::as<Rcpp::NumericMatrix>(Rcpp::wrap(maxima[i]));
	}

	return maximaList;
}

