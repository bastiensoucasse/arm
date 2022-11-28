#include "main.h"
#include "wavefront_io.h"

using namespace std;


int main()
{
  wf_merge("./OBJ/prism_01.obj","./OBJ/pyramid_01.obj",
			      "merge_prism_01_pyramid_01.obj");
  wf_merge_multiple_vertices("merge_prism_01_pyramid_01.obj",
			      "merge_multiple_vertices_prism_01_pyramid_01.obj");
  cout << " SUCCESS \n";
  return 0;
}
