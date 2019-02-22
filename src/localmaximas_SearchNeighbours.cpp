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
#include <omp.h>

#include "localmaximas.h"
#include "R_regionQuery.h"



// [[Rcpp::export]]
arma::Col<unsigned int> SearchNeighbours(arma::Mat<double> &xy, arma::Col<double> &z, double eps) {

	// ThreadID
	int threadID = omp_get_thread_num();

	// formerly arguments to frNN
	int bucketSize = 10;	// default to frNN
	int splitRule = 5;		// suggested split rule
	double approx = 0.0;	// default to frNN

  // kd-tree uses squared distances
  double eps2 = eps*eps;

  // copy xy
  unsigned int nrow = xy.n_rows;
  unsigned int ncol = xy.n_cols;

	  //Rprintf("Thread %i: SearchNeighbours: nrow = %i, ncol = %i\n", threadID, nrow, ncol);

	  ANNpointArray dataPts = annAllocPts(nrow, ncol);
	  for (unsigned int j = 0; j < ncol; j++) {
		  for (unsigned int i = 0; i < nrow; i++) {
			  dataPts[i][j] = xy(i, j);
		  }
	  }


	// create kd-tree
	ANNpointSet* kdTree = new ANNkd_tree(dataPts, nrow, ncol, bucketSize, (ANNsplitRule)splitRule);
	//Rprintf("Thread %i: SearchNeighbours: &kdTree = %p\n", threadID, &kdTree);


	// initialise search
	  bool isMaxima = false;
	  std::vector<bool> pointTested(nrow, false);
	  std::vector<int> maxima;
	  std::vector<int> ids;
	  std::vector<bool>::iterator pt = pointTested.begin();

	  for (pt = pointTested.begin(); pt != pointTested.end(); pt++) {

		  if (!(*pt)) {

			  int idx = std::distance(pointTested.begin(), pt);

			  ids = get_neighbours(idx, dataPts, kdTree, eps2, approx);

			  
			  // Check whether the point is a maximum in its neighbourhood
			  isMaxima = check_maxima(idx, ids, z);

			  //Rprintf("Thread %i: point %i: isMaxima: %i\n", threadID, idx, isMaxima);

			  if (isMaxima) {
				  //Rprintf("Thread %i: point %i: push_back &maxima %p\n", threadID, idx, &maxima);
				  maxima.push_back(idx);

				  // If the point is a maximum then mark all other points (in the neighbourhood) as having 
				  // been tested, they cannot be a maximum in their respective neighbourhoods.
				  for (auto& it : ids) {
					  pointTested[it] = true;
				  }
			  }

			  *pt = true;
		  }
	  }

	  //std::cout << "Found maxima: success!" << std::endl;
	  //Rprintf("Thread %i: Finished Finding Maxima\n", threadID);

	  // cleanup
	  delete kdTree;
	  annDeallocPts(dataPts);
	  //annClose(); // fix a minor memory leak (see ANN.h)


	  //std::cout << "deallocated kdTree and points: success!" << std::endl;
	  // Convert std::vector<int> to arma::Col<unsigned int>
	  arma::Col<unsigned int> ARMA_maxima(maxima.size());
	  for (unsigned int i = 0; i < ARMA_maxima.n_rows; i++) {
		  ARMA_maxima(i) = maxima[i];
	  }
	  return ARMA_maxima;
}




std::vector<int> get_neighbours(int &i, ANNpointArray &dataPts, ANNpointSet* kdTree, double &eps2, double &approx){

	// Find the neighbours to the current point.
	std::vector<int> idxNeighbours = regionQuery(i, dataPts, kdTree, eps2, approx);

	//Rprintf("ThreadID %i: point %i: %i Neighbours\n", omp_get_thread_num(), i, idxNeighbours.size());
	
	std::vector<int> ids(idxNeighbours.begin(), idxNeighbours.end());


	// remove self matches
	std::vector<bool> take(ids.size(), true);
	std::transform(ids.begin(), ids.end(), take.begin(),
		[i](int pos) { return pos != i; });

	/*
	#pragma omp critical
	{
	Rprintf("Thread %i: point %i: (%g, %g, %g)\n", threadID, i, dataPts[i][0], dataPts[i][1], z[i]);
	Rprintf("Neighbours: \n");
	for (unsigned int k = 0; k < ids.size(); k++){
	Rprintf("\t ids[%i] = %i, take[%i] = %s, z[%i] = %g\n", k, ids[k], k, (take[k] ? "true" : "false"), ids[k], z[ids[k]]);
	}
	Rprintf("\n");
	}

	*/

	ids = subset_by_logical(ids, take);


	return idxNeighbours;
}
