/*
// https://stackoverflow.com/questions/25846235/finding-the-indexes-of-all-occurrences-of-an-element-in-a-vector
*/


#include <vector>
#include <algorithm>
#include <iterator>


std::vector<int> find_all(std::vector<double> x, double val){
    std::vector<int> index;
    std::vector<double>::iterator it = x.begin();
    
    while ((it = std::find(it, x.end(), val)) != x.end()){
        index.push_back(std::distance(x.begin(), it)); // C indexing!
        it++;
    }
    return index;
}
