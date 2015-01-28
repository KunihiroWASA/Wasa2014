#ifndef __GRAPH__
#define __GRAPH__

#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

class Vertex;

class Graph {
    int degeneracy;
    int max_degree;
    std::unordered_map<int, int> label_to_id; 
    std::unordered_map<int, std::unique_ptr<Vertex>> vertices;
    std::vector<std::pair<int, int>> edges;  // use vertices id;
    std::vector<int> sorted_vector;

   public:
    Graph(); 

    int add_vertex(int __label);
    void add_edge(int v_label, int u_label);

    size_t get_number_of_vertices();
    size_t get_number_of_edges();
    int get_degeneracy(); 
    const std::vector<int> get_sorted_vector();
    const Vertex* get_vertex(int v_id);

    void sort_by_degeneracy();
    void sort_vertices();
    void show();
};

#endif
