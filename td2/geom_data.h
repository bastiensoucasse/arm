#ifndef GEOM_DATA_H
#define GEOM_DATA_H

#include <vector>

using namespace std;

extern int geom_read_prism_data(vector<vector<double>> &polygon, vector<double> &offset, const char *file_name_in);
extern int geom_read_pyramid_data(vector<vector<double>> &polygon, vector<double> &offset, const char *file_name_in);

#endif // GEOM_DATA_H
