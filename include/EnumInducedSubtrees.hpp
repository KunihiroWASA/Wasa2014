#ifndef __ENUMINDUCEDSUBTREES__
#define __ENUMINDUCEDSUBTREES__

#include <iomanip>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>

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

    std::unique_ptr<const Vertex* []> induced_subtree;
    int induced_subtree_size;

    std::unique_ptr<std::unique_ptr<std::unique_ptr<AdjItem> []> []> adj_items;
    std::unique_ptr<std::unique_ptr<CandItem> []> cand_items;
    std::unique_ptr<std::unique_ptr<AdjacentList> []> adjacent_lists;
    std::unique_ptr<bool[]> added_candidate;
    std::unique_ptr<bool[]> in_cand;

    std::unique_ptr<std::unique_ptr<CandItem* []> []> cand_add_history;
    std::unique_ptr<int[]> cand_add_count_history;
    std::unique_ptr<std::unique_ptr<CandItem* []> []> cand_remove_history;
    std::unique_ptr<int[]> cand_remove_count_history;

    std::unique_ptr<std::unique_ptr<
        std::tuple<AdjItem*, AdjItem*, AdjItem*> []> []> adj_history;
    std::unique_ptr<int[]> adj_count_history;

    std::unique_ptr<std::unique_ptr<
        std::tuple<const Vertex*, const Vertex*, AdjItem*, AdjItem*> []> []>
        adjadj_history;
    std::unique_ptr<int[]> adjadj_count_history;

    std::unique_ptr<CandItem* []> no_cand_cand_history;

   public:
    EnumInducedSubtrees();
    void init_graph(Graph* __g);

    void enumerate();
    void rec_enumerate();
    void rec_enumerate_output();
    void set_head(const Vertex* v);
    bool update(const Vertex* v);
    bool restore(const Vertex* v);

    bool candidate_no_add(const Vertex*);
    bool restore_candidate_no_add();

    void show_graph();
    void show_induced_subtree();

    void set_output_search_tree_parenthesis(bool b);
    void set_output_induced_subtree_differential(bool b);
    void set_output_induced_subtree_entire(bool b);
};
#endif
