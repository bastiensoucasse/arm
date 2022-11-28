#ifndef MAKE_PRIMITIVES_H
#define MAKE_PRIMITIVES_H

extern void make_prism_shell(int n, std::vector<std::vector<double>> polygon, std::vector<double> offset, std::vector<std::vector<double>> &vertices, std::vector<std::vector<int>> &faces);
extern void make_prism(int n, std::vector<std::vector<double>> polygon, std::vector<double> offset, std::vector<std::vector<double>> &vertices, std::vector<std::vector<int>> &faces);
extern void make_pyramid(int n, std::vector<std::vector<double>> polygon, std::vector<double> appex, std::vector<std::vector<double>> &vertices, std::vector<std::vector<int>> &faces);

#endif // MAKE_PRIMITIVES_H
