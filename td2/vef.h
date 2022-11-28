#ifndef VEF_H
#define VEF_H

using namespace std;

typedef vector<double> raw_vertex;
typedef vector<int> raw_face;

typedef struct vef_vertex
{
    int ind;
    vector<double> coordinates;
    vector<int> edges;
    vector<int> faces;
} vef_vertex;

typedef struct vef_edge
{
    int ind;
    vector<int> vertices;
    vector<pair<int, int>> faces;
} vef_edge;

typedef struct vef_face
{
    int ind;
    vector<int> vertices;
    vector<int> edges;
} vef_face;

typedef struct vef
{
    vector<vef_vertex> verticesEF;
    vector<vef_edge> edgesVF;
    vector<vef_face> facesVE;
} vef;

extern tuple<int, int, int> vef_from_wavefront(struct vef &vef, const char *file_name);
extern int vef_to_vef(struct vef &v_e_f, const char *file_name);

#endif // VEF_H
