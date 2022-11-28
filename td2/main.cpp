#include "main.h"

using namespace std;

int main(int argc, char **argv)
{
    for (int i = 1; i <= 3; i++)
    {
        ostringstream dat_file_name, obj_file_name, vef_file_name;
        dat_file_name << "DAT/prism_0" << i << ".dat";
        obj_file_name << "OBJ/prism_0" << i << ".obj";
        vef_file_name << "VEF/prism_0" << i << ".vef";

        vector<vector<double>> polygon;
        vector<double> v_off(3);
        int n = geom_read_prism_data(polygon, v_off, dat_file_name.str().c_str());

        vector<vector<double>> vertices;
        vector<vector<int>> faces;
        make_prism(n, polygon, v_off, vertices, faces);
        wf_write_to_wavefront(vertices, faces, obj_file_name.str().c_str());

        vef v_e_f;
        vef_from_wavefront(v_e_f, obj_file_name.str().c_str());
        vef_to_vef(v_e_f, vef_file_name.str().c_str());
    }

    for (int i = 1; i <= 3; i++)
    {
        ostringstream dat_file_name, obj_file_name, vef_file_name;
        dat_file_name << "DAT/pyramid_0" << i << ".dat";
        obj_file_name << "OBJ/pyramid_0" << i << ".obj";
        vef_file_name << "VEF/pyramid_0" << i << ".vef";

        vector<vector<double>> polygon;
        vector<double> v_off(3);
        int n = geom_read_pyramid_data(polygon, v_off, dat_file_name.str().c_str());

        vector<vector<double>> vertices;
        vector<vector<int>> faces;
        make_pyramid(n, polygon, v_off, vertices, faces);
        wf_write_to_wavefront(vertices, faces, obj_file_name.str().c_str());

        vef v_e_f;
        vef_from_wavefront(v_e_f, obj_file_name.str().c_str());
        vef_to_vef(v_e_f, vef_file_name.str().c_str());
    }

    for (int i = 1; i <= 3; i++)
    {
        ostringstream dat_file_name, obj_file_name, vef_file_name;
        dat_file_name << "DAT/prism_0" << i << ".dat";
        obj_file_name << "OBJ/prism_shell_0" << i << ".obj";
        vef_file_name << "VEF/prism_shell_0" << i << ".vef";

        vector<vector<double>> polygon;
        vector<double> v_off(3);
        int n = geom_read_prism_data(polygon, v_off, dat_file_name.str().c_str());

        vector<vector<double>> vertices;
        vector<vector<int>> faces;
        make_prism_shell(n, polygon, v_off, vertices, faces);
        wf_write_to_wavefront(vertices, faces, obj_file_name.str().c_str());

        vef v_e_f;
        vef_from_wavefront(v_e_f, obj_file_name.str().c_str());
        vef_to_vef(v_e_f, vef_file_name.str().c_str());
    }

    return EXIT_SUCCESS;
}
