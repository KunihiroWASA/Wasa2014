#ifndef __ENUMINDUCEDSUBTREES__
#define __ENUMINDUCEDSUBTREES__

#include <iomanip>
#include <iostream>
#include <memory>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Vertex.hpp"
#include "Graph.hpp"
#include "CandidateList.hpp"
#include "AdjacentList.hpp"

class EnumInducedSubtrees {
    Graph* g;
    CandidateList CAND;
    int rec_depth;
    bool output_search_tree_parenthesis; 
    bool output_induced_subtree_differential; 
    bool output_induced_subtree_entire; 
    bool output_something; 
    uint64_t induced_subtrees_num; 

    std::vector<const Vertex*> induced_subtree;

    std::unique_ptr<std::unique_ptr<std::unique_ptr<AdjItem>[]>[]> adj_items;
    std::unique_ptr<std::unique_ptr<CandItem>[]> cand_items;
    std::unique_ptr<std::unique_ptr<AdjacentList>[]> adjacent_lists;
    std::unique_ptr<bool[]> added_candidate;
    std::unique_ptr<bool[]> in_cand;


    std::stack<std::vector<CandItem*>> cand_add_history;
    std::stack<std::vector<CandItem*>> cand_remove_history;
    std::stack<CandItem*> cand_head_history;
    std::stack<std::vector<std::tuple<AdjItem*, AdjItem*, AdjItem*>>>
        adj_history;

    std::stack<std::vector<std::tuple<const Vertex*, const Vertex*, AdjItem*,
                                      AdjItem*>>> adjadj_history;

    std::stack<std::vector<std::tuple<const Vertex*, const Vertex*, AdjItem*,
                                      AdjItem*>>> no_cand_adj_history;
    std::stack<CandItem*> no_cand_cand_history;

   public:
    EnumInducedSubtrees();
    void init_graph(Graph* __g);

    void enumerate();
    void rec_enumerate();
    void rec_enumerate_output();
    void set_head(const Vertex * v); 
    bool update(const Vertex* v);
    bool restore(const Vertex* v);

    bool candidate_no_add(const Vertex *);
    bool restore_candidate_no_add();

    void show_graph();
    void show_induced_subtree();

    void set_output_search_tree_parenthesis(bool b); 
    void set_output_induced_subtree_differential(bool b); 
    void set_output_induced_subtree_entire(bool b); 
};
#endif
