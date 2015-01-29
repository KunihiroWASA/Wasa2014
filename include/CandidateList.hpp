#ifndef __CANDIDATELIST__
#define __CANDIDATELIST__

#include <iostream>
#include <memory>


class CandItem {
    const Vertex* v;
    CandItem* cand_next;
    CandItem* cand_prev;

   public:
    CandItem(const Vertex* __v)
        : v(__v), cand_next(this), cand_prev(this){};

    inline void set_next(CandItem* next) { cand_next = next; }; 
    inline void set_prev(CandItem* prev) { cand_prev = prev; }; 

    inline const Vertex* get_vertex() const { return v; }; 
    inline CandItem* get_next() { return cand_next; }; 
    inline CandItem* get_prev() { return cand_prev; }; 

};

class CandidateList {
    size_t size;
    std::unique_ptr<CandItem> head_tail;

   public:
    CandidateList(); 

    void set_head(CandItem* v_item);

    inline CandItem* get_head() { return head_tail->get_next(); };
    inline CandItem* get_tail() { return head_tail->get_prev(); };

    void merge(CandItem** addible_cand_items, int addible_cand_items_size);

    void remove_item(CandItem* c_item);

    void show();
    inline bool empty() { return head_tail.get()->get_next() == head_tail.get(); };  

};

#endif
