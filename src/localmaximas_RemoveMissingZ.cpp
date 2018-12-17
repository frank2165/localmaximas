

#include "localmaximas.h"



void RemoveMissingZ(RasterData &data, Rcpp::NumericMatrix &xy){

	Rcpp::LogicalVector notMissing = !Rcpp::is_na(data.z); //almost guaranteed to be one missing value

	xy = subset_matrix_rows(xy, notMissing); //Rcpp examples indicate that no change in the semantics are needed.
	data.z = data.z[notMissing];


	return;
}
