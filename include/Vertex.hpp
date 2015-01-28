#ifndef __VERTEX__
#define __VERTEX__

#include <algorithm>
#include <vector>

class CandItem;
class AdjItem;



class Vertex {
    static int cnt; 
    const int label; 
    const int id;
    int degeneracy_id;
    int degree;
    bool added_candidte_set;
    std::vector<Vertex *> adjacents;

   public:
    Vertex(int __id); 
    int get_id() const;
    int get_label() const;
    void add_adjacents(Vertex *v);
    int get_degree();
    void set_degeneracy_id(int __d_id);
    int get_degeneracy_id() const;
    const std::vector<Vertex *> get_adjacents() const;
    void sort_adjacent_by_degeneracy();
};

#endif
