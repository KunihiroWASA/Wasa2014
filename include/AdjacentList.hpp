#ifndef __ADJACENTLIST
#define __ADJACENTLIST

#include <iostream>
#include <memory>

class Vertex;

class AdjItem {
    const Vertex* v;
    AdjItem* adj_next;
    AdjItem* adj_prev;

   public:
    AdjItem(const Vertex* __v); 
    void set_next(AdjItem* __next);
    void set_prev(AdjItem* __prev);

    const Vertex* get_vertex() const;

    AdjItem* get_next();
    AdjItem* get_prev();
};

class AdjacentList {
    const Vertex* v;

   public:
    const std::unique_ptr<AdjItem> larger_head_tail;
    const std::unique_ptr<AdjItem> smaller_head_tail;
    const std::unique_ptr<AdjItem> smaller_avoiding_head_tail;


    AdjacentList(const Vertex* __v); 
    void push_back_to_larger(AdjItem* a);
    void push_back_to_smaller(AdjItem* a);
    void push_back_to_smaller_avoiding(AdjItem* a);

    AdjItem* get_larger_head();
    AdjItem* get_smaller_head();
    AdjItem* get_smaller_avoiding_head();

    AdjItem* get_larger_tail();
    AdjItem* get_smaller_tail();
    AdjItem* get_smaller_avoiding_tail();

    void remove_item(AdjItem* a);

    void show();
};

#endif
