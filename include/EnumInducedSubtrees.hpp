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
    bool make_parenthesis; 
    bool debug_output; 
    bool output_differential; 
    uint64_t induced_subtrees_num; 

    std::vector<const Vertex*> induced_subtree;

    std::unordered_map<
        const Vertex*,
        std::unordered_map<const Vertex*, std::unique_ptr<AdjItem>>> adj_items;
    std::unordered_map<const Vertex*, std::unique_ptr<CandItem>> cand_items;
    std::unordered_map<const Vertex*, std::unique_ptr<AdjacentList>>
        adjcent_lists;
    std::unordered_map<const Vertex*, bool> added_candidate;

    std::stack<std::vector<CandItem*>> cand_history;
    std::stack<CandItem*> cand_remove_history;
    std::stack<std::vector<std::tuple<AdjItem*, AdjItem*, AdjItem*>>>
        adj_history;
    std::stack<std::vector<std::tuple<const Vertex*, const Vertex*, AdjItem*,
                                      AdjItem*>>> adjadj_history;

    std::stack<std::vector<std::tuple<const Vertex*, const Vertex*, AdjItem*,
                                      AdjItem*>>> no_cand_adj_history;
    std::stack<std::tuple<CandItem*, CandItem*, CandItem*>>
        no_cand_cand_history;

    std::string parenthesis;

   public:
    EnumInducedSubtrees();
    void init_graph(Graph* __g);

    void enumerate();
    void rec_enumerate();
    bool update(const Vertex* v);
    bool restore(const Vertex* v);

    bool candidate_no_add(const Vertex* v);
    bool restore_candidate_no_add();

    void show_graph();
    void show_induced_subtree();

    void set_make_parenthesis(bool b); 
    void set_debug_output(bool b); 
    void set_output_differential(bool b); 
};
#endif
