#include <Rcpp.h>
#include <vector>
#include <omp.h>


static int MAXTHREADS = omp_get_max_threads();


// [[Rcpp::export]]
void recursion_counter(Rcpp::NumericVector x) {
    omp_set_num_threads(x.size());
    // if(x.size() < MAXTHREADS){
    //     
    // } else {
    //     omp_set_num_threads(MAXTHREADS);
    // }
    
#pragma omp parallel
{
    int count = 0;
    const int threadID = omp_get_thread_num();
    double num = 1.0;
    
    while(num < x[threadID]){
        num += num;
        count += 1;
    }
    
    Rprintf("Count = %i, Final Num = %g \n", count, num);
}
  
  return;
}



/*** R
recursion_counter(500)
recursion_counter(c(5, 10, 30, 60, 200, 500))
*/
