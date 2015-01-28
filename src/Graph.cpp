#include "Vertex.hpp"
#include "Graph.hpp"

Graph::Graph() : degeneracy(-1), max_degree(0){};

int Graph::add_vertex(int __label)
{
    if (label_to_id.find(__label) != label_to_id.end()) {
        return label_to_id[__label];
    }
    std::unique_ptr<Vertex> v(new Vertex(__label));
    int id               = v->get_id();
    label_to_id[__label] = id;
    vertices[id]         = std::move(v);

    return id; 
};

void Graph::add_edge(int v_label, int u_label)
{
    if (label_to_id.find(v_label) == label_to_id.end()) {
        std::cout << "ERR" << std::endl; 
        exit(1); 
    }
    if (label_to_id.find(u_label) == label_to_id.end()) {
        std::cout << "ERR" << std::endl; 
        exit(1); 
    }
    int v_id = label_to_id[v_label]; 
    int u_id = label_to_id[u_label];

    if (u_id < v_id) {
        if (find(edges.begin(), edges.end(), std::make_pair(v_id, u_id)) !=
            edges.end()) {
            return;
        }
    } else {
        if (find(edges.begin(), edges.end(), std::make_pair(u_id, v_id)) !=
            edges.end()) {
            return;
        }
    }
    edges.push_back(std::make_pair(v_id, u_id));

    vertices[u_id]->add_adjacents(vertices[v_id].get());
    vertices[v_id]->add_adjacents(vertices[u_id].get());

    if (vertices[u_id]->get_degree() > max_degree) {
        max_degree = vertices[u_id]->get_degree();
    }
    if (vertices[v_id]->get_degree() > max_degree) {
        max_degree = vertices[v_id]->get_degree();
    }
}


void Graph::sort_by_degeneracy()
{
    degeneracy = 0; 
    std::vector<std::vector<int>> linear_sort_vector;

    sorted_vector.clear();
    linear_sort_vector.resize(max_degree + 1);

    for (const auto& v : vertices) {
        int v_id = v.first;
        Vertex* v_pointer = v.second.get();
        linear_sort_vector.at(v_pointer->get_degree()).push_back(v_id);
    }


    for (size_t i = 0; i < get_number_of_vertices(); ++i) {
        for (int j = 0; j < max_degree + 1; ++j) {
            if (linear_sort_vector.at(j).empty()) {
                continue;
            }
            if (degeneracy < j) {
                degeneracy = j; 
            }
            int v_id = linear_sort_vector.at(j).back();
            linear_sort_vector.at(j).pop_back();
            sorted_vector.push_back(v_id);
            vertices[v_id]->set_degeneracy_id((int)sorted_vector.size());
            for (const auto& u : vertices.at(v_id)->get_adjacents()) {
                for (int k = 1; k <= max_degree; ++k) {
                    auto pos =
                        std::find(linear_sort_vector.at(k).begin(),
                                  linear_sort_vector.at(k).end(), u->get_id());
                    if (pos == linear_sort_vector.at(k).end()) {
                        continue;
                    } else {
                        linear_sort_vector.at(k).erase(pos); 
                        linear_sort_vector.at(k-1).push_back(u->get_id()); 
                        break; 
                    }
                }
            }
            break;
        }
    }

};

void Graph::sort_vertices()
{
    for (const auto& v_id : sorted_vector) {
        vertices[v_id]->sort_adjacent_by_degeneracy();
    }
}

void Graph::show()
{
    std::cout << "SHOW GRAPH" << std::endl;
    std::cout << std::setw(4); 
    std::cout << "v" << ": Adjacents of v" << std::endl; 

    for (const auto& v_id : sorted_vector) {
        std::cout << std::setw(4); 
        std::cout << vertices[v_id]->get_label() << ": ";
        for (const auto& u : vertices[v_id]->get_adjacents()) {
            std::cout << u->get_label() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "The degeneracy of the input graph: " << get_degeneracy() << std::endl; 
    std::cout << std::endl; 
}
