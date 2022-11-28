#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/IO/write_points.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;

int main(int argc, char **argv)
{
    if (argc != 3 && argc != 4)
    {
        cerr << "Usage: " << argv[0] << " <input file> <output file> [<cell size>]." << endl;
        cerr << endl;
        cerr << "Default parameters:" << endl;
        cerr << "        - cell size: 0.03." << endl;
        return EXIT_FAILURE;
    }

    /// Retrieve program args.
    const string input_file_name = argv[1];
    const string output_file_name = argv[2];
    const double cell_size = argc == 4 ? atof(argv[3]) : .03;
    if (argc == 3)
    {
        cout << "Using default parameters:" << endl;
        cerr << "        - cell size: 0.03." << endl;
    }

    /// Retrieve points.
    vector<Point> points;
    if (!CGAL::IO::read_points(input_file_name, back_inserter(points)))
    {
        cerr << "Failed to read input file \"" << input_file_name << "\"." << endl;
        return EXIT_FAILURE;
    }

    /// Simplify points.
    size_t original_points_size = points.size();
    auto start = chrono::high_resolution_clock::now();
    auto iterator_to_first_to_remove = CGAL::grid_simplify_point_set(points, cell_size);
    points.erase(iterator_to_first_to_remove, points.end());
    points.shrink_to_fit();
    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count() * 1e-9;
    size_t new_points_size = points.size();

    /// Display metrics.
    cout << "Point cloud size: from " << original_points_size << " points to " << new_points_size << " points." << endl;
    cout << "Simplification time: " << fixed << duration << setprecision(3) << "s." << endl;

    /// Export points.
    if (!CGAL::IO::write_points(output_file_name, points, CGAL::parameters::stream_precision(17)))
    {
        cerr << "Failed to write output file \"" << output_file_name << "\"." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
