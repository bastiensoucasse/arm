#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

#include "vef.h"
#include "wavefront_io.h"

using namespace std;

int vef_to_vef(struct vef &v_e_f, const char *file_name)
{
    ofstream f;
    f.open(file_name);
    f << "# " << file_name << endl
      << endl;

    /// Retrieve dimension.
    const int d = v_e_f.verticesEF[0].coordinates.size();

    /// Vertices.
    const vector<vef_vertex> vertices = v_e_f.verticesEF;
    for (int v = 0; v < vertices.size(); v++)
    {
        // Write vertice idx & coordinates.
        f << "v" << vertices[v].ind << " < ";
        for (int i = 0; i < d; i++)
            f << vertices[v].coordinates[i] << " ";
        f << "> ";

        /// Write adjacent edges.
        f << "< ";
        const vector<int> vedges = vertices[v].edges;
        for (int e = 0; e < vedges.size(); e++)
            f << "e" << vedges[e] << " ";
        f << "> ";

        /// Write adjacent faces.
        const vector<int> vfaces = vertices[v].faces;
        f << "< ";
        for (int fa = 0; fa < vfaces.size(); fa++)
            f << "f" << vfaces[fa] << " ";
        f << ">\n";
    }
    f << "\n";

    /// Edges.
    const vector<vef_edge> edges = v_e_f.edgesVF;
    for (int e = 0; e < edges.size(); e++)
    {
        /// Write edge idx.
        f << "e" << edges[e].ind;

        /// Write adjacent vertices.
        f << " < ";
        const vector<int> evertices = edges[e].vertices;
        for (int v = 0; v < evertices.size(); v++)
            f << "v" << evertices[v] << " ";
        f << ">";

        /// Write adjacent faces.
        f << " < ";
        const vector<pair<int, int>> efaces = edges[e].faces;
        for (int fa = 0; fa < efaces.size(); fa++)
            f << "(f" << efaces[fa].first << "," << efaces[fa].second << ") ";
        f << ">\n";
    }
    f << "\n";

    /// Faces.
    const vector<vef_face> faces = v_e_f.facesVE;
    for (int fa = 0; fa < faces.size(); fa++)
    {
        /// Write face idx
        f << "f" << faces[fa].ind;

        /// Write adjacent vertices
        f << " < ";
        const vector<int> fvertices = faces[fa].vertices;
        for (int v = 0; v < fvertices.size(); v++)
            f << "v" << fvertices[v] << " ";
        f << ">";

        /// Write adjacent edges
        f << " < ";
        const vector<int> fedges = faces[fa].edges;
        for (int e = 0; e < fedges.size(); e++)
            f << "e" << fedges[e] << " ";
        f << ">\n";
    }

    return 0;
}

/**
 * @brief Initializes a VEF data by adding only OBJ data (vertices: id and coordinates, and faces: id and vertices).
 *
 * @param v_e_f The VEF data to initialize.
 * @param vertices The vector of vertices.
 * @param faces The vector of faces.
 */
static void init_vef(vef &v_e_f, const vector<raw_vertex> &raw_vertices, const vector<raw_face> &raw_faces)
{
    for (int v_id = 1; v_id <= raw_vertices.size(); v_id++)
    {
        const raw_vertex &raw_v = raw_vertices.at(v_id - 1);

        vef_vertex vef_v;
        vef_v.ind = v_id;
        vef_v.coordinates = raw_v;
        // vef_v.edges
        // vef_v.faces
        v_e_f.verticesEF.push_back(vef_v);
    }

    for (int f_id = 1; f_id <= raw_faces.size(); f_id++)
    {
        const raw_face &raw_f = raw_faces.at(f_id - 1);

        vef_face vef_f;
        vef_f.ind = f_id;
        vef_f.vertices = raw_f;
        // vef_f.edges
        v_e_f.facesVE.push_back(vef_f);
    }
}

/**
 * @brief Get the VEF vertex reference from its id.
 *
 * @param v_e_f The VEF data.
 * @param id The vertex id.
 * @return vef_vertex&.
 */
static vef_vertex &getVertex(vef &v_e_f, int id)
{
    for (vef_vertex &v : v_e_f.verticesEF)
        if (v.ind == id)
            return v;

    cerr << "FATAL ERROR: Vertex #" << id << " not found in VEF." << endl;
    exit(EXIT_FAILURE);
}

/**
 * @brief Get the VEF edge pointer from its id.
 *
 * @param v_e_f The VEF data.
 * @param v0_id The first vertex id.
 * @param v1_id The second vertex id.
 * @return vef_edge*.
 */
static vef_edge *getEdge(vef &v_e_f, int v0_id, int v1_id)
{
    /// Automatically sort indices.
    if (v1_id < v0_id)
    {
        int tmp = v0_id;
        v0_id = v1_id;
        v1_id = tmp;
    }

    for (vef_edge &e : v_e_f.edgesVF)
        if (e.vertices.size() == 2 && e.vertices.at(0) == v0_id && e.vertices.at(1) == v1_id)
            return &e;

    return nullptr;
}

tuple<int, int, int> vef_from_wavefront(vef &v_e_f, const char *file_name)
{
    /// Retrieve the vertices and faces.
    vector<raw_vertex> vertices;
    vector<raw_face> faces;
    wf_read_from_wavefront(vertices, faces, file_name);
    init_vef(v_e_f, vertices, faces);

    /// For each face.
    for (vef_face &f : v_e_f.facesVE)
    {
        /// For each vertex id of the current face.
        for (int i = 0; i < f.vertices.size(); i++)
        {
            /// Retrieve the vertex.
            vef_vertex &v = getVertex(v_e_f, f.vertices.at(i));

            /// Add the face id to the vertex faces list.
            v.faces.push_back(f.ind);

            /// Retrieve the following vertex for defining an edge.
            vef_vertex &v_neigh = getVertex(v_e_f, f.vertices.at((i + 1) % f.vertices.size()));

            /// Check if the edge already exists.
            vef_edge *e = getEdge(v_e_f, v.ind, v_neigh.ind);
            if (e == nullptr)
            {
                /// Create a new edge for those vertices.
                vef_edge new_e;
                new_e.ind = v_e_f.edgesVF.size() + 1;

                /// Add to new_e its two vertices.
                new_e.vertices.push_back(min(v.ind, v_neigh.ind));
                new_e.vertices.push_back(max(v.ind, v_neigh.ind));

                /// Add the new_e id to the two vertices concerned edges list.
                v.edges.push_back(new_e.ind);
                v_neigh.edges.push_back(new_e.ind);

                /// Add the new edge to the VEF edges list.
                v_e_f.edgesVF.push_back(new_e);

                /// Update e as this new edge.
                e = getEdge(v_e_f, v.ind, v_neigh.ind);
            }

            /// Add the edge id to the face edges list.
            f.edges.push_back(e->ind);

            /// Add the face id to the edge faces list, with its direction.
            if (f.vertices[i] == e->vertices[0] && f.vertices[(i + 1) % f.vertices.size()] == e->vertices[1])
                e->faces.push_back(make_pair(f.ind, 1));
            if (f.vertices[(i + 1) % f.vertices.size()] == e->vertices[0] && f.vertices[i] == e->vertices[1])
                e->faces.push_back(make_pair(f.ind, -1));
        }
    }

    return make_tuple(v_e_f.verticesEF.size(), v_e_f.edgesVF.size(), v_e_f.facesVE.size());
}
