#ifndef TPOINT3DMAP_H
#define TPOINT3DMAP_H
// https://stackoverflow.com/questions/16792751/hashmap-for-2d3d-coordinates-i-e-vector-of-doubles

#include <unordered_map>

using namespace std;

class TPoint3D{
public:
    TPoint3D(double x, double y, double z) : x(x), y(y), z(z){};

    double x, y, z;
};

struct hashFunc{
    size_t operator()(const TPoint3D &k) const{
    size_t h1 = std::hash<double>()(k.x);
    size_t h2 = std::hash<double>()(k.y);
    size_t h3 = std::hash<double>()(k.z);
    return (h1 ^ (h2 << 1)) ^ h3;
    }
};

struct equalsFunc{
  bool operator()( const TPoint3D& lhs, const TPoint3D& rhs ) const{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
  }
};

typedef unordered_map<TPoint3D, vector<int> , hashFunc, equalsFunc> TPoint3DMap;

#endif /* TPOINT3DMAP_H */ 
