/*
Functions for interacting with the GDAL C++ library
*/

#include <Rcpp.h>
#include "rgdal.h"



// [[Rcpp::export]]
void Rcpp_local_maximas(Rcpp::List handleList, Rcpp::NumericVector search.radius){
    
    SEXP poDataset;
    double radius = search.radius[0];
    
    
    for(unsigned int i = 0; i < handleList.size(); i++){
        poDataset = handleList[[i]];
        find_local_maxima(poDataset, radius);
    }
    
    

    
    
    return;
}





void find_local_maxima(SEXP handle, double radius){
    
    SEXP gt;
    
    // Get coordinate information
    gt = RGDAL_GetGeoTransform(handle);
    if(getAttrib(gt, "CE_Failure")){
        stop("Could not get attributes from dataset handle!")
    }
    
}