/* Notes:

+ A quick check in R reveals that the internal storage of a long numeric vector approaches
  8 Bytes of storage per 64-bit value being stored (theoretically it cannot be less than this
  without compression). For this reason the package is using the Rcpp wrappers, rather than 
  switching to std::vector<double>, as this provides a similar storage efficiency with the
  added benefit of the C++ container features.

+ std::find_if did not accept the Rcpp::is_na function, so advantage will be taken of the
  Rcpp logical subsetting.



*/

#define R_NO_REMAP // Recommended


#include <vector>
#include <algorithm>
#include <iterator>

#include <R.h>
#include <Rinternals.h>
#include <RcppArmadillo.h>

#include "rgdal.h"
#include "ANN.h"


// Structures
struct RasterData{
	int XSize = 0;
	int YSize = 0;
	double origin_x = 0.0;
	double origin_y = 0.0;
	double res_x = 0.0;
	double res_y = 0.0;
	arma::Col<double> z; // Replace with RcppArmadillo varieties
};



// Utility functions
bool check_maxima(const unsigned int p, const std::vector<unsigned int> &idxNeighbours, const arma::Col<double> &z);

template<typename T>
std::vector<T> subset_by_index(std::vector<T> &x, std::vector<unsigned int> &idx){
	std::vector<T> result(idx.size());
	std::transform(idx.begin(), idx.end(), result.begin(), [x](size_t pos){
		return x[pos];
	});

	return result;
}


template<typename T>
std::vector<T> subset_by_logical(std::vector<T> &x, std::vector<bool> &lgl){
	std::vector<unsigned int> idx;
	std::vector<bool>::iterator it = lgl.begin();
	while ((it = std::find(it, lgl.end(), true)) != lgl.end()){
		idx.push_back(std::distance(lgl.begin(), it));
		it++;
	}

	return subset_by_index(x, idx);
}


// Functionality
RasterData ReadDataset(SEXP sxpHandle);
arma::Mat<double> SetCoordinates(const RasterData&);
arma::Col<unsigned int> SearchNeighbours(const arma::Mat<double> &xy, const arma::Col<double> &z, const double eps);
std::vector<unsigned int> get_neighbours(unsigned int &i, ANNpointArray &dataPts, ANNpointSet* &kdTree, double &eps2, double &approx);


// Workflow
Rcpp::List FindLocalMaxima(Rcpp::List handles, double radius, int numCores);