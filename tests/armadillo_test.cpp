#include <RcppArmadillo.h>

// [[Rcpp::export]]
arma::Mat<double> subset_matrix(arma::Mat<double> X, arma::Col<double> z){
    
    arma::Col<unsigned int> idx = arma::find_finite(z);
    
    X = X.rows(idx);
    return X;
}


/*** R
X <- matrix(rnorm(16), nrow = 8, ncol = 2)
z <- rnorm(8)
z[c(4, 5, 8)] <- NA

X[!is.na(z), ]
subset_matrix(X, z)
*/