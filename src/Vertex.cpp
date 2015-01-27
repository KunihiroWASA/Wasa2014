#include "Vertex.hpp"
#include "CandidateList.hpp"
#include "AdjacentList.hpp"

Vertex::Vertex(int __id)
    : id(__id), degeneracy_id(-1), degree(0), added_candidte_set(false){};

int Vertex::get_id() const
{
    return id;
}

void Vertex::add_adjacents(Vertex *v)
{
    adjacents.push_back(v);
    ++degree;
}

int Vertex::get_degree()
{
    return degree;
}

void Vertex::set_degeneracy_id(int __d_id)
{
    degeneracy_id = __d_id;
}

int Vertex::get_degeneracy_id() const
{
    return degeneracy_id;
}

const std::vector<Vertex *> Vertex::get_adjacents() const
{
    return adjacents;
};

void Vertex::sort_adjacent_by_degeneracy()
{
    auto based_on_degeneracy = [&](Vertex *u, Vertex *v) {
        return u->degeneracy_id < v->degeneracy_id;
    };
    std::sort(adjacents.begin(), adjacents.end(), based_on_degeneracy);
}
