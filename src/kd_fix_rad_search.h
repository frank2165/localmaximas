//----------------------------------------------------------------------
// File:			kd_fix_rad_search.h
// Programmer:		Sunil Arya and David Mount
// Description:		Standard kd-tree fixed-radius kNN search
// Last modified:	??/??/?? (Version 1.1)
//----------------------------------------------------------------------
// Copyright (c) 1997-2005 University of Maryland and Sunil Arya and
// David Mount.  All Rights Reserved.
// 
// This software and related documentation is part of the Approximate
// Nearest Neighbor Library (ANN).  This software is provided under
// the provisions of the Lesser GNU Public License (LGPL).  See the
// file ../ReadMe.txt for further information.
// 
// The University of Maryland (U.M.) and the authors make no
// representations about the suitability or fitness of this software for
// any purpose.  It is provided "as is" without express or implied
// warranty.
//----------------------------------------------------------------------
// History:
//	Revision 1.1  ??/??/??
//		Initial release
//----------------------------------------------------------------------

#ifndef ANN_kd_fix_rad_search_H
#define ANN_kd_fix_rad_search_H

#include <omp.h>
#include "ANNx.h"
#include "kd_tree.h"					// kd-tree declarations
#include "kd_util.h"					// kd-tree utilities
#include "pr_queue_k.h"					// k-element priority queue

#include "ANNperf.h"				// performance evaluation

//----------------------------------------------------------------------
//	Global variables
//		These are active for the life of each call to
//		annRangeSearch().  They are set to save the number of
//		variables that need to be passed among the various search
//		procedures.
//----------------------------------------------------------------------

//extern ANNpoint			ANNkdFRQ;			// query point (static copy)

//----------------------------------------------------------------------
//		To keep argument lists short, a number of global variables
//		are maintained which are common to all the recursive calls.
//		These are given below.
//----------------------------------------------------------------------

extern int				ANNkdFRDim;				// dimension of space
extern ANNpoint		ANNkdFRQ;				// query point
extern ANNdist			ANNkdFRSqRad;			// squared radius search bound
extern double			ANNkdFRMaxErr;			// max tolerable squared error
extern ANNpointArray	ANNkdFRPts;				// the points
extern ANNmin_k*		ANNkdFRPointMK;			// set of k closest points

extern std::vector<int> closest;			  // MFH: set of all closest points
extern std::vector<double> dists;			  // MFH: set of all closest points

extern int				ANNkdFRPtsVisited;		// total points visited
extern int				ANNkdFRPtsInRange;		// number of points in the range

#pragma omp threadprivate(ANNkdFRDim, ANNkdFRQ, ANNkdFRSqRad, ANNkdFRMaxErr, ANNkdFRPts, ANNkdFRPointMK, closest, dists, ANNkdFRPtsVisited, ANNkdFRPtsInRange)

#endif
