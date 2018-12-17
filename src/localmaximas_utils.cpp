/*
> https://stackoverflow.com/questions/25846235/finding-the-indexes-of-all-occurrences-of-an-element-in-a-vector
*/

#include "localmaximas.h"


/* Use x[is_na(x)];
Rcpp::IntegerVector find_na(Rcpp::NumericVector x){
    Rcpp::IntegerVector index;
    Rcpp::NumericVector::iterator it = x.begin();
    
    while ((it = std::find_if(it, x.end(), Rcpp::is_na)) != x.end()){
        index.push_back(std::distance(x.begin(), it)); // C indexing!
        it++;
    }
    return index;
}
*/

void rebase_index(Rcpp::IntegerVector &x){
	for (auto& it : x){
		it += 1;
	}
	return;
}



bool check_maxima(const int point, Rcpp::IntegerVector &neighbours, Rcpp::NumericVector &Z) {

	// neighbours should be an index of Z
	assert(max(neighbours) < Z.length());

	bool localmaximum;
	Rcpp::IntegerVector::iterator it;


	// Iterate through the list of neighbours and check whether the point is a maxima within its
	// neighbourhood. If a point is a maxima, then all other points in its neighbourhood cannot be
	// a maxima. Note however that points may reside in multiple neighbourhoods and so these points
	// cannot simply be discarded.
	it = std::find_if(neighbours.begin(), neighbours.end(), [&Z, &point](const int &i){
		bool strictLT = Z[point] < Z[i];
		bool isEQ = Z[point] == Z[i];
		bool idLT = i < point;
		return strictLT || (isEQ && idLT);
	});

	localmaximum = (it == neighbours.end());
	return localmaximum;
}




Rcpp::NumericMatrix subset_matrix_rows(Rcpp::NumericMatrix &M, Rcpp::LogicalVector &keep){
	Rcpp::NumericMatrix out(Rcpp::sum(keep), M.ncol());
	Rcpp::NumericVector col(M.nrow());

	for (int i = 0; i < out.ncol(); i++){
		col = M(Rcpp::_, i);
		col = col[keep];
		out(Rcpp::_, i) = col;
	}

	return out;
}


Rcpp::NumericMatrix subset_matrix_rows(Rcpp::NumericMatrix &M, Rcpp::IntegerVector &idx){
	Rcpp::NumericMatrix out(idx.length(), M.ncol());
	Rcpp::NumericVector col(M.nrow());

	for (int i = 0; i < out.ncol(); i++){
		col = M(Rcpp::_, i);
		col = col[idx];
		out(Rcpp::_, i) = col;
	}

	return out;
}
