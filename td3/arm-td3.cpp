#include "arm-td3.h"

#include "wavefront_io.h"

using namespace std;

int main(int argc, char **argv)
{
    // wf_merge("prism_prism_shell/prism.obj", "prism_prism_shell/prism_shell.obj", "prism_prism_shell/merge_manifold.obj");
    wf_merge_multiple_vertices("plant/cubicuboctahedron.obj", "plant/cubicuboctahedron_nm.obj");
    return EXIT_SUCCESS;
}
