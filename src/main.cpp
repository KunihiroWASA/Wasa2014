#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include "Graph.hpp"
#include "EnumInducedSubtrees.hpp"

void make_petersen_graph(Graph* g);
void make_graph_from_file(Graph * g, std::string& file_path);
std::vector<std::string> split(const std::string& s, char delim); 

void show_help(char * ename); 


int main(int argc, char * argv[])
{
    int result; 
    bool show_help_flag                      = false;
    bool output_search_tree_parenthesis      = false;
    bool output_induced_subtree_differential = false;
    bool output_induced_subtree_entire       = false;
    bool output_newick                       = false;
    bool file_opt                            = false;
    bool wrong_options                       = false;
    std::string file_path; 

    while ((result = getopt(argc, argv, "hpnbedf:")) != -1) {
        switch (result) {
            case 'p':
                output_search_tree_parenthesis = true;
                break; 
            case 'n':
                output_newick = true;
                break; 
            case 'b':
                output_induced_subtree_entire = true;
                output_induced_subtree_differential = true;
                break; 
            case 'e':
                output_induced_subtree_entire = true;
                break; 
            case 'd':
                output_induced_subtree_differential = true;
                break; 
            case 'h':
                show_help_flag = true;
                break; 
            case 'f':
                file_opt  = true;
                file_path = optarg;
                break; 
            case ':':
                std::cerr << result << " needs value. " << std::endl; 
            default:
                show_help_flag     = true;
                wrong_options = true; 
                break; 
        }
    }

    if (wrong_options or show_help_flag or argc == 1) {
        show_help(argv[0]); 
        return 0; 
    }

    std::unique_ptr<Graph> g(new Graph());
    EnumInducedSubtrees eis;

    // option setting
    eis.set_output_induced_subtree_entire(output_induced_subtree_entire); 
    eis.set_output_induced_subtree_differential(output_induced_subtree_differential); 
    eis.set_output_search_tree_parenthesis(output_search_tree_parenthesis); 
    eis.set_output_newick(output_newick); 

    if (file_opt) {
        make_graph_from_file(g.get(), file_path); 
    } else {
        std::cout << "Default graph... " << std::endl; 
        make_petersen_graph(g.get()); 
    }

    // Enumerate!
    eis.init_graph(g.get());
    eis.show_graph(); 
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
            if (source < dist) {
                g->add_edge(source, dist);
            }
        }
    }
    std::cout << "done. " << std::endl;
    std::cout << std::endl;
}

void make_petersen_graph(Graph* g)
{
    std::cout << "Making pertersen graph.... " << std::endl;
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

void show_help(char * ename)
{
    std::cout << std::endl;
    std::cout << "Useage: " << ename << " -f filepath -hpnbed" << std::endl; 
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h [ --help ]                 display help (this page) " << std::endl;
    std::cout << "  -f [ --file ] arg             file path" << std::endl;
    std::cout << "  -p [ --output_parenthesis ]   output the parenthesis of the search tree" << std::endl;
    std::cout << "  -n [ --output_newick ]        output the newick format of the search tree" << std::endl;
    std::cout << "  -b [ --output_both ]          output all induced subtrees (entire and diff)" << std::endl;
    std::cout << "  -e [ --output_entire ]        output all induced subtrees (entire)" << std::endl;
    std::cout << "  -d [ --output_differential ]  output all induced subtrees (differential)" << std::endl;
    std::cout << std::endl;
    std::cout << "Input file sample (petersen graph) " << std::endl; 
    std::cout << "1: 2 5 6 " << std::endl; 
    std::cout << "2: 1 3 7 " << std::endl; 
    std::cout << "3: 2 4 8 " << std::endl; 
    std::cout << "4: 3 5 9 " << std::endl; 
    std::cout << "5: 1 4 10 " << std::endl; 
    std::cout << "6: 1 8 9 " << std::endl; 
    std::cout << "7: 2 9 10 " << std::endl; 
    std::cout << "8: 3 6 10 " << std::endl; 
    std::cout << "9: 4 6 7 " << std::endl; 
    std::cout << "10: 5 7 8 " << std::endl; 
    std::cout << std::endl;
    std::cout << "Each line represents the adjacent list for a vertex v. " << std::endl; 
    std::cout << "\tv: u[1] u[2] ... " << std::endl; 
    std::cout << ", where u[i] is the i-th adjacent of v. " << std::endl; 
    std::cout << std::endl;
}
