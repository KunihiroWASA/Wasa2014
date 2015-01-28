#ifndef __GRAPH__
#define __GRAPH__

#include <algorithm>
#include <iostream>
#include <iomanip>
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

    inline const Vertex* get_vertex(int v_id) { return vertices[v_id].get(); };
    inline const std::vector<int> get_sorted_vector() { return sorted_vector; };
    inline int get_degeneracy() { return degeneracy; };
    inline size_t get_number_of_edges() { return edges.size(); }; 
    inline size_t get_number_of_vertices() { return vertices.size(); }; 


    void sort_by_degeneracy();
    void sort_vertices();
    void show();
};

#endif
