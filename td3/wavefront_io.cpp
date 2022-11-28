#include <fstream>
#include <iostream>

#include <iterator>

#include "tPoint3DMap.h"
#include "vectorHasherMap.h"
#include "wavefront_io.h"

using namespace std;
std::pair<int, int>
wf_read_from_wavefront(vector<vector<double>> &vertices,
                       vector<vector<int>> &faces, const char *file_name_in)
{
    std::ifstream file_in;
    file_in.open(file_name_in);
    int num_v = 0;
    int num_f = 0;
    // http://saurabhg.com/programming/wavefront-obj-reader-building-cgalpolyhedron_3/

    std::string _token;
    if (!file_in.good())
    {
        file_in.clear();
        cout << "wf_read_from_wavefront: file_in problems"
             << "\n";
        exit(1);
    }
    file_in.seekg(0);
    while (!file_in.eof())
    {
        _token = ""; // Reset token.
        file_in >> _token;
        if (_token == "v")
        {
            vector<double> vertex(3);
            file_in >> vertex[0] >> vertex[1] >> vertex[2];
            num_v++;
            vertices.push_back(vertex);
        }
        else if (_token == "f")
        {
            std::string _line;
            getline(file_in, _line);
            std::istringstream _stream(_line);
            std::vector<std::string> _vertices;
            std::copy(std::istream_iterator<std::string>(_stream),
                      std::istream_iterator<std::string>(),
                      std::back_inserter(_vertices));
            vector<int> vertices(_vertices.size());
            for (size_t i = 0; i < _vertices.size(); ++i)
            {
                std::string::size_type _pos = _vertices[i].find('/', 0);
                std::string _indexStr = _vertices[i].substr(0, _pos);
                vertices[i] = stoi(_indexStr);
            }
            num_f++;
            faces.push_back(vertices);
        }
    }
    file_in.close();
    return make_pair(num_v, num_f);
}

std::pair<int, int>
wf_write_to_wavefront(vector<vector<double>> &vertices,
                      vector<vector<int>> &faces, const char *file_name)
{
    ofstream f;
    f.open(file_name);
    f << "# " << file_name << "\n";
    for (size_t i = 0; i < vertices.size(); i++)
    {
        f << "v " << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
    }
    for (size_t i = 0; i < faces.size(); i++)
    {
        f << 'f';
        f << ' ';
        for (size_t j = 0; j < faces[i].size(); j++)
        {
            f << faces[i][j] << ' ';
        }
        f << "\n";
    }
    cout << file_name << " created \n";
    return make_pair(vertices.size(), faces.size());
}

int wf_merge(const char *file_name_in_1, const char *file_name_in_2, const char *file_name_out)
{
    vector<vector<double>> vertices;
    vector<vector<int>> faces;
    ;
    std::pair<int, int> num_vtcs_faces_1, num_vtcs_faces_2;
    ofstream file_out;

    file_out.open(file_name_out);
    if (file_out.is_open())
    {
        num_vtcs_faces_1 = wf_read_from_wavefront(vertices, faces, file_name_in_1);
        num_vtcs_faces_2 = wf_read_from_wavefront(vertices, faces, file_name_in_2);
        for (size_t i = 0; i < vertices.size(); i++)
            file_out << 'v' << ' '
                     << vertices[i][0] << ' '
                     << vertices[i][1] << ' '
                     << vertices[i][2] << "\n";
        for (size_t i = 0; i < (size_t)num_vtcs_faces_1.second; i++)
        {
            vector<int> face(faces[i].size());
            file_out << 'f';
            file_out << ' ';
            for (size_t j = 0; j < faces[i].size(); j++)
            {
                file_out << faces[i][j] << ' ';
            }
            file_out << "\n";
        }
        for (size_t i = num_vtcs_faces_1.second;
             i < (size_t)num_vtcs_faces_1.second + num_vtcs_faces_2.second; i++)
        {
            file_out << 'f';
            file_out << ' ';
            for (size_t j = 0; j < faces[i].size(); j++)
            {
                file_out << faces[i][j] + num_vtcs_faces_1.first << ' ';
            }
            file_out << "\n";
        }
        file_out.close();
    }
    else
    {
        cout << "Unable to open file";
        return 1;
    }
    return 0;
}

int wf_merge_multiple_vertices(const char *file_name_in, const char *file_name_out)
{
    vector<vector<double>> vertices_in, vertices_out;
    vector<vector<int>> faces_in, faces_out;
    std::pair<int, int> num_vtcs_faces_in, num_vtcs_faces_out;

    num_vtcs_faces_in = wf_read_from_wavefront(vertices_in, faces_in, file_name_in);

    TPoint3DMap umap;

    for (size_t i = 0; i < (size_t)num_vtcs_faces_in.first; i++)
    {
        umap[TPoint3D(vertices_in[i][0], vertices_in[i][1], vertices_in[i][2])].push_back(i);
    }
    // cout << umap;;

    TPoint3DMap vmap;
    int new_index = 1;
    for (const auto &p : umap)
    {
        vertices_out.push_back({p.first.x, p.first.y, p.first.z});
        vmap[TPoint3D(p.first.x, p.first.y, p.first.z)].push_back(new_index);
        new_index++;
    }
    // cout <<  vmap;;

    // Renumber vertices after the merging of the multiple occurrences
    vector<vector<int>> faces_tmp;
    for (size_t i = 0; i < faces_in.size(); i++)
    {
        vector<int> face;
        for (size_t j = 0; j < faces_in[i].size(); j++)
        {
            int jj = faces_in[i][j] - 1;
            ; // vertices numbering start at 1, while vector indices at 0 :) !
            int k = vmap[TPoint3D(vertices_in[jj][0], vertices_in[jj][1], vertices_in[jj][2])].front();
            face.push_back(k);
        }
        faces_tmp.push_back(face);
    }

    // Merge faces with identical vertex sequences
    vectorHasherMap uvmap;
    for (size_t i = 0; i < faces_tmp.size(); i++)
    {
        vector<long unsigned int> face;
        // TO DO long unsigned int <-> int ? ,
        // changement a cause des ERRs car operator() sur vector<int>
        // n'est pas defini?!
        for (size_t j = 0; j < faces_tmp[i].size(); j++)
            face.push_back(faces_tmp[i][j]);
        uvmap[face] = i;
    }
    for (const auto &p : uvmap)
    {
        std::vector<int> face;
        for (size_t i = 0; i < p.first.size(); i++)
            face.push_back(p.first[i]);
        faces_out.push_back(face);
    }
    num_vtcs_faces_out = wf_write_to_wavefront(vertices_out, faces_out, file_name_out);

    cout << " Number of merged vertices " << vertices_in.size() - vertices_out.size() << "\n";
    ;
    cout << " Number of merged faces " << faces_in.size() - faces_out.size() << "\n";
    ;
    return 0;
}
