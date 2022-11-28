#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/wlop_simplify_and_regularize_point_set.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/IO/write_points.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Parallel_if_available_tag ConcurrencyTag;

int main(int argc, char **argv)
{
    if (argc != 3 && argc != 5)
    {
        cerr << "Usage: " << argv[0] << " <input file> <output file> [<retain percentage> <neighbor radius>]." << endl;
        cerr << endl;
        cerr << "Default parameters:" << endl;
        cerr << "        - retain percentage: 2.0." << endl;
        cerr << "        - neighbor radius: 0.5." << endl;
        return EXIT_FAILURE;
    }

    /// Retrieve program args.
    const string input_file_name = argv[1];
    const string output_file_name = argv[2];
    const double retain_percentage = argc == 5 ? atof(argv[3]) : 2.;
    const double neighbor_radius = argc == 5 ? atof(argv[4]) : .5;
    if (argc == 3)
    {
        cout << "Using default parameters:" << endl;
        cerr << "        - retain percentage: 2.0." << endl;
        cerr << "        - neightbor radius: 0.5." << endl;
    }

    /// Retrieve points.
    vector<Point> original_points;
    if (!CGAL::IO::read_points(input_file_name, back_inserter(original_points)))
    {
        cerr << "Failed to read input file \"" << input_file_name << "\"." << endl;
        return EXIT_FAILURE;
    }

    /// Simplify points.
    size_t original_points_size = original_points.size();
    auto start = chrono::high_resolution_clock::now();
    vector<Point> new_points;
    auto iterator_to_first_to_remove = CGAL::wlop_simplify_and_regularize_point_set<ConcurrencyTag>(original_points, back_inserter(new_points), CGAL::parameters::select_percentage(retain_percentage).neighbor_radius(neighbor_radius));
    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count() * 1e-9;
    size_t new_points_size = new_points.size();

    /// Display metrics.
    cout << "Point cloud size: from " << original_points_size << " points to " << new_points_size << " points." << endl;
    cout << "Simplification time: " << fixed << duration << setprecision(3) << "s." << endl;

    /// Export points.
    if (!CGAL::IO::write_points(output_file_name, new_points, CGAL::parameters::stream_precision(17)))
    {
        cerr << "Failed to write output file \"" << output_file_name << "\"." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
