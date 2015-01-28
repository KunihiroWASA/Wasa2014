#ifndef __ADJACENTLIST
#define __ADJACENTLIST

#include <iostream>
#include <memory>


class AdjItem {
    const Vertex* v;
    AdjItem* adj_next;
    AdjItem* adj_prev;

   public:
    AdjItem(const Vertex* __v)
        : v(__v), adj_next(nullptr), adj_prev(nullptr){};

    inline void set_next(AdjItem* __next) { adj_next = __next;}; 
    inline void set_prev(AdjItem* __prev) { adj_prev = __prev;}; 

    inline const Vertex* get_vertex() const { return v; }; 
    inline AdjItem* get_next() { return adj_next; }; 
    inline AdjItem* get_prev() { return adj_prev; }; 
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

    inline AdjItem* get_larger_tail() { return larger_head_tail->get_prev(); }; 
    inline AdjItem* get_smaller_tail() { return smaller_head_tail->get_prev(); }; 
    inline AdjItem* get_smaller_avoiding_tail() { return smaller_avoiding_head_tail->get_prev(); }; 

    inline AdjItem* get_larger_head() { return larger_head_tail->get_next(); }; 
    inline AdjItem* get_smaller_head() { return smaller_head_tail->get_next(); }; 
    inline AdjItem* get_smaller_avoiding_head() { return smaller_avoiding_head_tail->get_next(); }; 

    void remove_item(AdjItem* a);

    void show();
};

#endif
