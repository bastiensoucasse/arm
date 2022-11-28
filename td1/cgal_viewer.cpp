#include <chrono>
#include <fstream>
#include <iostream>

#include <CGAL/draw_point_set_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Point_set_3<Point> PointSet;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <input file>." << endl;
        return EXIT_FAILURE;
    }

    /// Retrieve program args.
    const string input_file_name = argv[1];

    /// Retrieve points.
    PointSet point_set;
    auto start = chrono::high_resolution_clock::now();
    if (!CGAL::IO::read_point_set(input_file_name, point_set))
    {
        cerr << "Failed to read input file \"" << input_file_name << "\"." << endl;
        return EXIT_FAILURE;
    }
    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count() * 1e-9;

    /// Display metrics.
    cout << "Point cloud size: " << point_set.size() << " points." << endl;
    cout << "Load time: " << fixed << duration << setprecision(3) << "s." << endl;

    /// Draw points.
    CGAL::draw(point_set);

    return EXIT_SUCCESS;
}
