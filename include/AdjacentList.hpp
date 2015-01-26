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
    AdjItem(const Vertex* __v) : v(__v), adj_next(nullptr), adj_prev(nullptr){};
    void set_next(AdjItem* __next);
    void set_prev(AdjItem* __prev);

    const Vertex* get_vertex() const;

    AdjItem* get_next();
    AdjItem* get_prev();
};

class AdjacentList {
    const Vertex* v;
    // size_t larger_size;
    // size_t smaller_size;
    // size_t smaller_avoiding_size;

   public:
    const std::unique_ptr<AdjItem> larger_head_tail;
    const std::unique_ptr<AdjItem> smaller_head_tail;
    const std::unique_ptr<AdjItem> smaller_avoiding_head_tail;

    AdjacentList(const Vertex* __v)
        : v(__v),
          // larger_size(0),
          // smaller_size(0),
          // smaller_avoiding_size(0),
          larger_head_tail(new AdjItem(nullptr)),
          smaller_head_tail(new AdjItem(nullptr)),
          smaller_avoiding_head_tail(new AdjItem(nullptr))
    {
        larger_head_tail->set_prev(larger_head_tail.get());
        larger_head_tail->set_next(larger_head_tail.get());

        smaller_head_tail->set_prev(smaller_head_tail.get());
        smaller_head_tail->set_next(smaller_head_tail.get());

        smaller_avoiding_head_tail->set_prev(smaller_avoiding_head_tail.get());
        smaller_avoiding_head_tail->set_next(smaller_avoiding_head_tail.get());
    };

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
