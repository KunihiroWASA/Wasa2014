#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <boost/program_options.hpp>


#include "Graph.hpp"
#include "EnumInducedSubtrees.hpp"

void make_petersen_graph(Graph* g);
void make_k4_graph(Graph* g);
void make_graph_from_file(Graph * g, std::string& file_path);
std::vector<std::string> split(const std::string& s, char delim); 

int main(int argc, char const* argv[])
{
    boost::program_options::options_description opt("Options");
    opt.add_options()
        ("help,h",         "display help")
        ("file,f",         boost::program_options::value<std::string>(), "file path")
        ("parenthesis,p",   "make parenthesis")
        ("output,o",        "output")
        ("differential,d",  "output by differential");
    boost::program_options::variables_map vm;
    try {
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, opt), vm);
    } catch(const boost::program_options::error_with_option_name& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    boost::program_options::notify(vm);
 
    if (vm.count("help")) {
        std::cout << opt << std::endl;
        return 0; 
    }

    std::unique_ptr<Graph> g(new Graph());

    if (vm.count("file")) {
        std::string file_path = vm["file"].as<std::string>(); 
        make_graph_from_file(g.get(), file_path); 
    } else {
        make_petersen_graph(g.get());
    }

    EnumInducedSubtrees eis;

    if (vm.count("output")) {
        eis.set_debug_output(true); 
    } else {
        eis.set_debug_output(false); 
    }

    if (vm.count("differential")) {
        eis.set_output_differential(true); 
    } else {
        eis.set_output_differential(false); 
    }

    if (vm.count("parenthesis")) {
        eis.set_make_parenthesis(true); 
    } else {
        eis.set_make_parenthesis(false); 
    }

    eis.init_graph(g.get());

    eis.show_graph(); 
    std::cout << "The degeneracy of the input graph: " << g->get_degeneracy() << std::endl; 
    std::cout << std::endl; 

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
