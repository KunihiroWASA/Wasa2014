#ifndef __CANDIDATELIST__
#define __CANDIDATELIST__

#include <iostream>
#include <memory>
#include <vector>

class Vertex;

class CandItem {
    const Vertex* v;
    CandItem* cand_next;
    CandItem* cand_prev;

   public:
    CandItem(const Vertex* __v) : v(__v), cand_next(this), cand_prev(this){};

    CandItem* get_next();
    CandItem* get_prev();
    const Vertex* get_vertex() const;

    void set_next(CandItem* next);
    void set_prev(CandItem* prev);
};

class CandidateList {
    size_t size;
    std::unique_ptr<CandItem> head_tail;

   public:
    CandidateList() : size(0), head_tail(new CandItem(nullptr)){};

    void inc_size();
    void dec_size();

    void set_head(CandItem* v_item);
    CandItem* get_head();
    CandItem* get_tail();
    size_t get_size();

    void merge(std::vector<CandItem*>& addible_cand_items);

    void remove_item(CandItem* c_item);

    void show();

    bool empty();
};

#endif
