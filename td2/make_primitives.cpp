#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

void make_prism_shell(int n, vector<vector<double>> polygon, vector<double> offset, vector<vector<double>> &vertices, vector<vector<int>> &faces)
{
    // Vertices of the polygon
    for (vector<double> v : polygon)
        vertices.push_back(v);

    // Vertices of the polygon with offset
    int d = polygon[0].size();
    for (vector<double> v : polygon)
    {
        vector<double> new_v;
        for (int i = 0; i < d; i++)
            new_v.push_back(v[i] + offset[i]);
        vertices.push_back(new_v);
    }

    // Lateral faces of the prism
    for (int i = 1; i <= n; i++)
    {
        vector<int> f;
        f.push_back(i);
        f.push_back(i + 1 > n ? 1 : i + 1);
        f.push_back(i + 1 + n > 2 * n ? i + 1 : i + 1 + n);
        f.push_back(i + n);
        faces.push_back(f);
    }
}

void make_prism(int n, vector<vector<double>> polygon, vector<double> offset, vector<vector<double>> &vertices, vector<vector<int>> &faces)
{
    // Vertices of the polygon
    for (vector<double> v : polygon)
        vertices.push_back(v);

    // Vertices of the polygon with offset
    int d = polygon[0].size();
    for (vector<double> v : polygon)
    {
        vector<double> new_v;
        for (int i = 0; i < d; i++)
            new_v.push_back(v[i] + offset[i]);
        vertices.push_back(new_v);
    }

    // Bases (faces) of the prism
    // for (int i = 0; i < 2; i++) {
    //   vector<int> f;
    //   for (int j = 1; j <= n; j++)
    //     f.push_back(j + i*n);
    //   faces.push_back(f);
    // }
    vector<int> f;
    for (int i = n; i > 0; i--)
        f.push_back(i);
    faces.push_back(f);
    f.clear();

    for (int i = n + 1; i <= 2 * n; i++)
        f.push_back(i);
    faces.push_back(f);

    // Lateral faces of the prism
    for (int i = 1; i <= n; i++)
    {
        vector<int> f;
        f.push_back(i);
        f.push_back(i + 1 > n ? 1 : i + 1);
        f.push_back(i + 1 + n > 2 * n ? i + 1 : i + 1 + n);
        f.push_back(i + n);
        faces.push_back(f);
    }
}

void make_pyramid(int n, vector<vector<double>> polygon, vector<double> appex, vector<vector<double>> &vertices, vector<vector<int>> &faces)
{
    // Vertices of the polygon
    for (vector<double> v : polygon)
        vertices.push_back(v);

    // Add appex of the pyramid
    vertices.push_back(appex);

    // Base (face) of the pyramid
    vector<int> f;
    for (int i = 1; i <= n; i++)
        f.push_back(i);
    faces.push_back(f);

    // Lateral faces of the pyramid
    for (int i = 1; i <= n; i++)
    {
        vector<int> f;
        f.push_back(i);
        f.push_back(i + 1 > n ? 1 : i + 1);
        f.push_back(n + 1);
        faces.push_back(f);
    }
}
