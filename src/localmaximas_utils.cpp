/*
> https://stackoverflow.com/questions/25846235/finding-the-indexes-of-all-occurrences-of-an-element-in-a-vector
*/

#include "localmaximas.h"



bool check_maxima(int point, std::vector<int> &neighbours, arma::Col<double> &Z) {

	bool localmaximum;
	auto it = neighbours.begin();


	// Iterate through the list of neighbours and check whether the point is a maxima within its
	// neighbourhood. If a point is a maxima, then all other points in its neighbourhood cannot be
	// a maxima. Note however that points may reside in multiple neighbourhoods and so these points
	// cannot simply be discarded.
	it = std::find_if(it, neighbours.end(), [&Z, &point](const int &i){
		bool strictLT = Z[point] < Z[i];
		bool isEQ = Z[point] == Z[i];
		bool idLT = i < point;
		return strictLT || (isEQ && idLT);
	});

	localmaximum = (it == neighbours.end());
	return localmaximum;
}
