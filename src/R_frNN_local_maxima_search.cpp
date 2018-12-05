//----------------------------------------------------------------------
//                   Fixed Radius Nearest Neighbors
// File:                        frNN.cpp
//----------------------------------------------------------------------
// Copyright (c) 2015 Michael Hahsler. All Rights Reserved.
//
// This software is provided under the provisions of the
// GNU General Public License (GPL) Version 3
// (see: http://www.gnu.org/licenses/gpl-3.0.en.html)
//
// Modified by Marc Fletcher for use in HQPlantations LiDAR Tree 
// Identification efforts (2018-04-12). Not intended for distribution.
//
// Function frNN_int was modified, and subsequently renamed, to return an 
// index of points that are local maxima in a canopy height map. 
// The function was originally used in an unmodified form using R, however 
// memory constraints imposed by checking a large number of points has 
// required a "lower-level" approach.


// Note: does not return self-matches unless selfmatches = TRUE

#include <Rcpp.h>
#include "ANN/ANN.h"
#include "R_regionQuery.h"

using namespace Rcpp;

// [[Rcpp::plugins("cpp11")]]


bool check_maxima_cpp(const int, IntegerVector, NumericVector);



// [[Rcpp::export]]
IntegerVector R_maxima_search(NumericMatrix xy, NumericVector z, double eps) {

	// formerly arguments to frNN
	int bucketSize = 10;	// default to frNN
	int splitRule = 5;		// suggested split rule
	double approx = 0.0;	// default to frNN

  // kd-tree uses squared distances
  double eps2 = eps*eps;

  // copy xy
  int nrow = xy.nrow();
  int ncol = xy.ncol();
  ANNpointArray dataPts = annAllocPts(nrow, ncol);
  for(int i = 0; i < nrow; i++){
    for(int j = 0; j < ncol; j++){
      (dataPts[i])[j] = xy(i, j);
    }
  }
 

  // create kd-tree (1) or linear search structure (2)
  ANNpointSet* kdTree = NULL;

  kdTree = new ANNkd_tree(dataPts, nrow, ncol, bucketSize, (ANNsplitRule)  splitRule);

  bool isMaxima = false;
  IntegerVector maxima;
  LogicalVector pointTested(nrow, false);

  for (int p=0; p<nrow; p++) {
    if (!(p % 100)) Rcpp::checkUserInterrupt();

	if (!pointTested[p]){
		nn N = regionQueryDist(p, dataPts, kdTree, eps2, approx);

		IntegerVector ids = IntegerVector(N.first.begin(), N.first.end());

		// remove self matches
		LogicalVector take = ids != p;
		ids = ids[take];

		isMaxima = check_maxima_cpp(p, ids, z);
		if (isMaxima){
			maxima.push_back(p);
			for (auto& it : ids){
				pointTested[it] = true;
			}
		}
		pointTested[p] = true;
	}
  }

  // Conversion back to 1-based indexing
  for (auto& it : maxima){
	  it += 1;
  }


  // cleanup
  delete kdTree;
  annDeallocPts(dataPts);
  annClose();

  return maxima;
}
