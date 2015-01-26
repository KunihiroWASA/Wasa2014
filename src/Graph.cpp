#include "Vertex.hpp"
#include "Graph.hpp"

void Graph::add_vertex(int __id)
{
    if (vertices.find(__id) != vertices.end()) {
        // std::cout << "The vertex with id " << __id << " is already added. "
                  // << std::endl;
        return;
    }
    std::unique_ptr<Vertex> v(new Vertex(__id));
    vertices[__id] = std::move(v);
};

void Graph::add_edge(int v_id, int u_id)
{
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

const Vertex* Graph::get_vertex(int v_id)
{
    return vertices[v_id].get();
};

size_t Graph::get_number_of_vertices()
{
    return vertices.size();
};

size_t Graph::get_number_of_edges()
{
    return edges.size();
};

void Graph::sort_by_degeneracy()
{
    std::vector<std::vector<int>> linear_sort_vector;

    sorted_vector.clear();
    linear_sort_vector.resize(max_degree + 1);

    for (const auto& v : vertices) {
        int v_id = v.first;
        Vertex* v_pointer = v.second.get();
        linear_sort_vector.at(v_pointer->get_degree()).push_back(v_id);
    }

    /*
     * for (std::size_t i = 0; i < linear_sort_vector.size(); ++i) {
     *     std::cout << i << ": ";
     *     for (const auto& v_id : linear_sort_vector.at(i)) {
     *         std::cout << v_id << " ";
     *     }
     *     std::cout << std::endl;
     * }
     */

    for (size_t i = 0; i < get_number_of_vertices(); ++i) {
        for (int j = 0; j < max_degree + 1; ++j) {
            if (linear_sort_vector.at(j).empty()) {
                continue;
            }
            int v_id = linear_sort_vector.at(j).back();
            sorted_vector.push_back(v_id);
            vertices[v_id]->set_degeneracy_id((int)sorted_vector.size());
            for (const auto& u : vertices.at(v_id)->get_adjacents()) {
                for (int k = 1; k <= max_degree; ++k) {
                    auto pos =
                        std::find(linear_sort_vector.at(k).begin(),
                                  linear_sort_vector.at(k).end(), u->get_id());
                    if (pos == linear_sort_vector.at(k).end()) {
                        continue;
                    }
                }
            }
            linear_sort_vector.at(j).pop_back();
            break;
        }
    }

    /*
     * for (std::size_t i = 0; i < sorted_vector.size(); ++i) {
     *     std::cout << i << ": ";
     *     int v_id = sorted_vector.at(i);
     *     std::cout << v_id
     *               << " ("
     *               << vertices[v_id]->get_degeneracy_id()
     *               << ") ";
     *     std::cout << std::endl;
     * }
     */
};

const std::vector<int> Graph::get_sorted_vector()
{
    return sorted_vector;
}

void Graph::sort_vertices()
{
    for (const auto& v_id : sorted_vector) {
        vertices[v_id]->sort_adjacent_by_degeneracy();
    }
}

void Graph::show()
{
    std::cout << "SHOW GRAPH" << std::endl;
    for (const auto& v_id : sorted_vector) {
        std::cout << v_id << ": ";
        for (const auto& u : vertices[v_id]->get_adjacents()) {
            std::cout << u->get_id() << " ";
        }
        std::cout << std::endl;
    }
}
