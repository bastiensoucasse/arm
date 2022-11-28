#ifndef VECTORHASHERMAP_H
#define  VECTORHASHERMAP_H

//https://stackoverflow.com/questions/41604789/use-vectors-as-key-in-unordered-map?answertab=oldest#tab-top
//https://ideone.com/i674OP

#include <boost/functional/hash.hpp> /* hash_combine */

using namespace std;


#include <unordered_map>

template <typename T>

struct vectorHasher{
  size_t operator()(vector<T> const &in) const
  {
    using boost::hash_value;
    using boost::hash_combine;
    // Start with a hash value of 0
    size_t seed = 0;
    T value;
    for (size_t i=0; i< in.size(); i++)
      {
	value = static_cast<T>(in[i]);
	hash_combine(seed, hash_value(value));
      }
    return seed;
  }
};

typedef unordered_map <vector<size_t>, int, vectorHasher<size_t> > vectorHasherMap;


#endif /* VECTORHASHERMAP_H*/
