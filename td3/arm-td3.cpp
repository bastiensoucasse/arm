#include "arm-td3.h"

#include "wavefront_io.h"

using namespace std;

int main(int argc, char **argv)
{
    wf_merge("data/prism_01.obj", "data/pyramid_01.obj", "output/merge_prism_01_pyramid_01.obj");
    wf_merge_multiple_vertices("data/merge_prism_01_pyramid_01.obj", "output/merge_multiple_vertices_prism_01_pyramid_01.obj");
    return EXIT_SUCCESS;
}
