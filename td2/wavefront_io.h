#ifndef WAVEFRONT_OI_H
#define WAVEFRONT_OI_H

using namespace std;

extern pair<int, int> wf_read_from_wavefront(vector<vector<double>> &vertices, vector<vector<int>> &faces, const char *file_name_in);
extern pair<int, int> wf_write_to_wavefront(vector<vector<double>> &vertices, vector<vector<int>> &faces, const char *file_name);

#endif // WAVEFRONT_OI_H
