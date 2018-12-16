/*
This function processes an R vector of file handles into a list of coordinates of 
local maxima within each file. OpenMP is used in an attempt to minimise the program runtime.
*/


//#include <omp.h>
#include "localmaximas.h"


//[[Rcpp::export]]
Rcpp::List find_local_maxima(Rcpp::CharacterVector files, double search_radius){

	// Allocate memory for output
	Rcpp::List maxima(files.length()); 
	//Rcpp:::CharacterVector::iterator f = files.begin();


	/* Loop over the files:
		1./ Get the raster coordinates,
		2./ Get the CHM heights,
		3./ Remove CHM points with missing data,
		4./ Use fixed-radius NN search to find maxima,
		5./ Store xyz-coordinates in list
	*/
	SEXP fileName = Rf_protect(Rf_allocVector(STRSXP, 1));

	for (int i = 0; i < files.length(); i++){
		fileName = Rf_mkString(files[i]);
		Rf_PrintValue(fileName);
		
		Rcpp::NumericMatrix xy = get_coordinates(fileName);
		Rcpp::NumericVector z = get_heights(fileName);

		Rcpp::LogicalVector notMissing = !Rcpp::is_na(z); //almost guaranteed to be one missing value
		xy = subset_matrix_rows(xy, notMissing);
		z = z[notMissing];

		Rcpp::IntegerVector idxMaxima = frNN_search(xy, z, search_radius);
		Rcpp::NumericMatrix maxima_coords(idxMaxima.length(), 3);

		xy = subset_matrix_rows(xy, idxMaxima);
		z = z[idxMaxima];
		maxima_coords(Rcpp::_, 0) = xy(Rcpp::_, 0);
		maxima_coords(Rcpp::_, 1) = xy(Rcpp::_, 1);
		maxima_coords(Rcpp::_, 2) = z;

		maxima[i] = maxima_coords;
	}

	maxima.names() = files;
	
	Rf_unprotect(1);
	return maxima;
}


Rcpp::NumericMatrix subset_matrix_rows(Rcpp::NumericMatrix M, Rcpp::LogicalVector keep){
	Rcpp::NumericMatrix out(Rcpp::sum(keep), M.ncol());
	Rcpp::NumericVector col(M.nrow());

	for (int i = 0; i < out.ncol(); i++){
		col = M(Rcpp::_, i);
		col = col[keep];
		out(Rcpp::_, i) = col;
	}

	return out;
}


Rcpp::NumericMatrix subset_matrix_rows(Rcpp::NumericMatrix M, Rcpp::IntegerVector idx){
	Rcpp::NumericMatrix out(idx.length(), M.ncol());
	Rcpp::NumericVector col(M.nrow());

	for (int i = 0; i < out.ncol(); i++){
		col = M(Rcpp::_, i);
		col = col[idx];
		out(Rcpp::_, i) = col;
	}

	return out;
}

