#ifndef WAVEFRONT_OI_H
#define WAVEFRONT_OI_H
using namespace std;

#include <vector>

extern pair <int,int> wf_read_from_wavefront(vector <vector<double>> &vertices,
					     vector <vector<int>> &faces, const char* file_name_in);
extern pair <int,int> wf_write_to_wavefront(vector <vector<double>> &vertices,
						   vector <vector<int>> &faces, const char* file_name);
extern int wf_merge(const char* file_name_in_1, const char* file_name_in_2, const char* file_name_out);
extern int wf_merge_multiple_vertices(const char* file_name_in, const char* file_name_out);
#endif /*  WAVEFRONT_OI_H */
