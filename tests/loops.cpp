#include <Rcpp.h>
#include <iterator>
using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]



// [[Rcpp::export]]
NumericVector do_the_loop_iterator(NumericVector x, NumericVector y) {
    
    int i = 0;
    NumericVector z(x.size());
    NumericVector::iterator it = z.begin();
    
    for(it = z.begin(); it != z.end(); it++){
        i = std::distance(z.begin(), it);
        *it = x[i] + y[i];
    }
    
    
    return z;
}



// [[Rcpp::export]]
NumericVector do_the_loop_range(NumericVector x, NumericVector y) {
    
    NumericVector z(x.size());
    
    for(auto &it : z){
        auto i = &it - &z[0];
        it = x[i] + y[i];
    }
    
    
    return z;
}




/*** R
x <- rnorm(10)
y <- rnorm(10, 20)

x + y
do_the_loop_iterator(x, y)
do_the_loop_range(x, y)
*/
