#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>


#include "Graph.hpp"
#include "EnumInducedSubtrees.hpp"

void make_petersen_graph(Graph* g);
void make_k4_graph(Graph* g);
void make_graph_from_file(Graph * g, std::string& file_path);
std::vector<std::string> split(const std::string& s, char delim); 

int main(int argc, char const* argv[])
{
    std::unique_ptr<Graph> g(new Graph());
    if (argc == 2) {
        std::string file_path(argv[1]); 
        make_graph_from_file(g.get(), file_path); 
    } else {
    // make_petersen_graph(g.get());
        make_k4_graph(g.get());
    }


    EnumInducedSubtrees eis;

    eis.set_make_parenthesis(false); 
    eis.set_debug_output(true); 
    eis.set_output_differential(true); 
    eis.init_graph(g.get());
    eis.enumerate();
    return 0;
}

void make_graph_from_file(Graph * g, std::string& file_path)
{
    std::ifstream ifs(file_path.c_str()); 
    std::string str; 

    std::cout << "Making graph.... " << std::endl;
    if (ifs.fail()) {
        std::cerr << "FAILURE: Open file! " << std::endl; 
        exit(1); 
    }

    std::cout << "OPEN: " << file_path << std::endl; 

    while (getline(ifs, str)) {
        std::cout << str << std::endl; 
        std::vector<std::string> strs = split(str, ':'); 
        int source = atoi(strs[0].c_str()); 
        g->add_vertex(source); 
        std::vector<std::string> strss = split(strs[1], ' '); 
        for (auto& s : strss) {
            int dist = atoi(s.c_str()); 
            g->add_vertex(dist); 
            if (g->get_vertex(source)->get_id() < g->get_vertex(dist)->get_id() ) {
                g->add_edge(source, dist);
            }
        }
    }
    std::cout << "done. " << std::endl;
    std::cout << std::endl;
}

void make_k4_graph(Graph* g)
{
    std::cout << "Making graph.... " << std::endl;
    g->add_vertex(1);
    g->add_vertex(2);
    g->add_vertex(3);
    g->add_vertex(4);

    g->add_edge(1, 2);
    g->add_edge(1, 3);
    g->add_edge(1, 4);

    g->add_edge(2, 3);
    g->add_edge(2, 4);

    g->add_edge(3, 4);
    std::cout << "done. " << std::endl;
    std::cout << std::endl;
}
void make_petersen_graph(Graph* g)
{
    std::cout << "Making graph.... " << std::endl;
    g->add_vertex(1);
    g->add_vertex(2);
    g->add_vertex(3);
    g->add_vertex(4);
    g->add_vertex(5);
    g->add_vertex(6);
    g->add_vertex(7);
    g->add_vertex(8);
    g->add_vertex(9);
    g->add_vertex(10);

    g->add_edge(1, 2);
    g->add_edge(2, 3);
    g->add_edge(3, 4);
    g->add_edge(4, 5);
    g->add_edge(5, 1);
    g->add_edge(1, 6);
    g->add_edge(2, 7);
    g->add_edge(3, 8);
    g->add_edge(4, 9);
    g->add_edge(5, 10);
    g->add_edge(6, 8);
    g->add_edge(7, 9);
    g->add_edge(8, 10);
    g->add_edge(6, 9);
    g->add_edge(7, 10);
    std::cout << "done. " << std::endl;
    std::cout << std::endl;
}

std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}
