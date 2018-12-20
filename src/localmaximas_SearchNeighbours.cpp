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

#include <iostream>

#include "localmaximas.h"
#include "R_regionQuery.h"



// [[Rcpp::export]]
arma::Col<unsigned int> SearchNeighbours(const arma::Mat<double> &xy, const arma::Col<double> &z, const double eps) {

	// formerly arguments to frNN
	int bucketSize = 10;	// default to frNN
	int splitRule = 5;		// suggested split rule
	double approx = 0.0;	// default to frNN

  // kd-tree uses squared distances
  double eps2 = eps*eps;

  // copy xy
  unsigned int nrow = xy.n_rows;
  unsigned int ncol = xy.n_cols;
  ANNpointArray dataPts = annAllocPts(nrow, ncol);
  for (unsigned int j = 0; j < ncol; j++){
	for(unsigned int i = 0; i < nrow; i++){
      dataPts[i][j] = xy(i, j);
    }
  }
 

  // create kd-tree
  ANNpointSet* kdTree = new ANNkd_tree(dataPts, nrow, ncol, bucketSize, (ANNsplitRule)splitRule); // Allocating to heap


  // initialise search
  bool isMaxima = false;
  std::vector<bool> pointTested(nrow, false);
  std::vector<unsigned int> maxima;

  for (unsigned int p = 0; p < nrow; p++) {

	if (!pointTested[p]){
		// Find the neighbours to the current point.
		std::vector<int> idxNeighbours = regionQuery(p, dataPts, kdTree, eps2, approx);
		std::vector<unsigned int> ids(idxNeighbours.begin(), idxNeighbours.end());


		// remove self matches
		std::vector<bool> take(ids.size(), true);
		std::transform(ids.begin(), ids.end(), take.begin(),
			[p](size_t pos){ return pos == p; });

		ids = subset_by_logical(ids, take);


		// Check whether the point is a maximum in its neighbourhood
		isMaxima = check_maxima(p, ids, z);
		if (isMaxima){
			maxima.push_back(p);

			// If the point is a maximum then mark all other points (in the neighbourhood) as having 
			// been tested, they cannot be a maximum in their respective neighbourhoods.
			for (auto& it : ids){
				pointTested[it] = true;
			}
		}
		pointTested[p] = true;
	}
  }


  // cleanup
  delete kdTree;
  annDeallocPts(dataPts);
  //annClose(); // fix a minor memory leak (see ANN.h)


  // Convert std::vector<int> to arma::Col<unsigned int>
  arma::Col<unsigned int> ARMA_maxima(maxima.size());
  for (unsigned int i = 0; i < ARMA_maxima.n_rows; i++){
	  ARMA_maxima(i) = maxima[i];
  }


  return ARMA_maxima;
}
