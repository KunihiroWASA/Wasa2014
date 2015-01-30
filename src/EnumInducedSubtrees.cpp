#include "EnumInducedSubtrees.hpp"

EnumInducedSubtrees::EnumInducedSubtrees()
    : output_search_tree_parenthesis(false),
      output_induced_subtree_differential(false),
      output_induced_subtree_entire(false),
      output_something(false),
      output_newick(false),
      induced_subtrees_num(0){};

void EnumInducedSubtrees::set_output_search_tree_parenthesis(bool b)
{
    output_search_tree_parenthesis = b;
    output_something = true;
}

void EnumInducedSubtrees::set_output_induced_subtree_differential(bool b)
{
    output_induced_subtree_differential = b;
    output_something = true;
}

void EnumInducedSubtrees::set_output_induced_subtree_entire(bool b)
{
    output_induced_subtree_entire = b;
    output_something = true;
}

void EnumInducedSubtrees::set_output_newick(bool b)
{
    output_newick = b;
    if (b) {
        output_search_tree_parenthesis = true;
    }
    output_something = true;
}


void EnumInducedSubtrees::init_graph(Graph* __g)
{
    g = __g;
    g->sort_by_degeneracy();
    g->sort_vertices();

    size_t vertex_num = g->get_sorted_vector().size();
    int max_degree = g->get_max_degree();

    // init adjacent lists
    adjacent_lists.reset(new std::unique_ptr<AdjacentList>[vertex_num]);
    adj_items.reset(
        new std::unique_ptr<std::unique_ptr<AdjItem> []>[vertex_num]);
    for (size_t i = 0; i < vertex_num; ++i) {
        adj_items[i].reset(new std::unique_ptr<AdjItem>[vertex_num]);
    }
    for (const auto& v_id : g->get_sorted_vector()) {
        const Vertex* v = g->get_vertex(v_id);
        std::unique_ptr<AdjacentList> v_alist(new AdjacentList(v));
        adjacent_lists[v_id] = std::move(v_alist);
        for (const Vertex* u : v->get_adjacents()) {
            std::unique_ptr<AdjItem> v_u(new AdjItem(u));
            const int u_id = u->get_id();
            AdjItem* v_u_p = v_u.get();
            adj_items[v_id][u_id] = std::move(v_u);

            if (v->get_degeneracy_id() < u->get_degeneracy_id()) {
                adjacent_lists[v_id]->push_back_to_larger(v_u_p);
            } else {
                adjacent_lists[v_id]->push_back_to_smaller(v_u_p);
            }
        }
    }

    // init candidate list
    cand_items.reset(new std::unique_ptr<CandItem>[vertex_num]);
    for (const auto& v_id : g->get_sorted_vector()) {
        const Vertex* v = g->get_vertex(v_id);
        std::unique_ptr<CandItem> v_citem(new CandItem(v));
        cand_items[v_id] = std::move(v_citem);
    }

    added_candidate.reset(new bool[vertex_num]);
    in_cand.reset(new bool[vertex_num]);
    for (const auto& v_id : g->get_sorted_vector()) {
        added_candidate[v_id] = false;
        in_cand[v_id] = false;
    }

    // init stack (array)
    cand_add_history.reset(new std::unique_ptr<CandItem* []>[vertex_num]);
    cand_add_count_history.reset(new int[vertex_num]);
    for (size_t i = 0; i < vertex_num; i++) {
        cand_add_history[i].reset(new CandItem* [max_degree]);
    }

    cand_remove_history.reset(new std::unique_ptr<CandItem* []>[vertex_num]);
    cand_remove_count_history.reset(new int[vertex_num]);
    for (size_t i = 0; i < vertex_num; i++) {
        cand_remove_history[i].reset(new CandItem* [max_degree]);
    }

    adj_history.reset(new std::unique_ptr<
        std::tuple<AdjItem*, AdjItem*, AdjItem*> []>[vertex_num]);
    adj_count_history.reset(new int[vertex_num]);
    for (size_t i = 0; i < vertex_num; i++) {
        adj_history[i].reset(
            new std::tuple<AdjItem*, AdjItem*, AdjItem*>[max_degree]);
    }

    adjadj_history.reset(new std::unique_ptr<std::tuple<
        const Vertex*, const Vertex*, AdjItem*, AdjItem*> []>[vertex_num]);
    adjadj_count_history.reset(new int[vertex_num]);
    for (size_t i = 0; i < vertex_num; i++) {
        adjadj_history[i].reset(
            new std::tuple<const Vertex*, const Vertex*, AdjItem*,
                           AdjItem*>[max_degree * g->get_degeneracy()]);
    }

    no_cand_cand_history.reset(new CandItem* [vertex_num]);

    addible_vertices.reset(new const Vertex*[max_degree + max_degree * degeneracy]);

    // init subtree_size
    induced_subtree.reset(new const Vertex* [vertex_num]);
}

void EnumInducedSubtrees::set_head(const Vertex* v)
{
    const int v_id = v->get_id();
    CAND.set_head(cand_items[v_id].get());
    added_candidate[v_id] = true;
    AdjacentList* v_alist = adjacent_lists[v_id].get();
    for (AdjItem* u_item = v_alist->get_larger_head();
         u_item != v_alist->larger_head_tail.get();
         u_item = u_item->get_next()) {
        const Vertex* u = u_item->get_vertex();
        const int u_id = u->get_id();
        AdjacentList* u_adj_list = adjacent_lists[u_id].get();
        AdjItem* v_item = adj_items[u_id][v_id].get();

        u_adj_list->remove_item(v_item);
        u_adj_list->push_back_to_smaller_avoiding(v_item);
    }
}

void EnumInducedSubtrees::enumerate()
{
    std::cout << "START ENUMERATION: " << std::endl;
    rec_depth = 0;
    induced_subtrees_num = 0;
    induced_subtree_size = 0;
    vertex_num = g->get_sorted_vector().size();
    max_degree = g->get_max_degree(); 
    degeneracy = g->get_degeneracy(); 
    if (output_search_tree_parenthesis) {
        if (output_newick) {
            comma = ','; 
        } else {
            comma = '\0'; 
        }
        std::cout << "(";
    }

    output_something = output_search_tree_parenthesis or
                       output_induced_subtree_entire or
                       output_induced_subtree_differential;
    for (const auto& v_id : g->get_sorted_vector()) {
        const Vertex* v = g->get_vertex(v_id);
        set_head(v);
        ++induced_subtree_size;
        induced_subtree[induced_subtree_size - 1] = v;
        update(v);

        if (output_induced_subtree_differential) {
            std::cout << std::endl; 
            std::cout << "* " << v->get_label() << " "; 
            std::cout << std::endl; 
            if (output_induced_subtree_entire) {
                show_induced_subtree(); 
            }
            if (!CAND.empty()) {
                std::cout << "* "; 
            }
        }
        if (output_search_tree_parenthesis) {
            std::cout << "(";
        }

        if (output_something) {
            rec_enumerate_output();
        } else {
            rec_enumerate();
        }

        if (output_induced_subtree_differential) {
                std::cout << std::endl; 
        }
        if (output_search_tree_parenthesis) {
            std::cout << ")" << comma << "";
        }

        if (output_induced_subtree_differential) {
            std::cout << "* -" << v->get_label() << " "; 
            std::cout << std::endl; 
        }

        restore(v);
        --induced_subtree_size;
        in_cand[v_id] = false;
        CAND.remove_item(cand_items[v_id].get());
    }

    if (output_search_tree_parenthesis) {
        std::cout << ")"; 
        if (output_newick) {
            std::cout << ";"; 
        }
        std::cout << std::endl;
    }
    std::cout << "DONE." << std::endl;
    std::cout << "SUBTREE NUM.: " << induced_subtrees_num << std::endl;
    return;
}

void EnumInducedSubtrees::rec_enumerate()
{
    if (CAND.empty()) {
        ++induced_subtrees_num;
        return;
    }

    const Vertex* v = CAND.get_head()->get_vertex();
    ++rec_depth;
    candidate_no_add(v);
    rec_enumerate();
    restore_candidate_no_add();

    ++induced_subtree_size;
    induced_subtree[induced_subtree_size - 1] = v;
    update(v);
    rec_enumerate();
    restore(v);
    --induced_subtree_size;
    --rec_depth;
}

void EnumInducedSubtrees::rec_enumerate_output()
{
    if (CAND.empty()) {
        ++induced_subtrees_num;

        if (output_search_tree_parenthesis) {
            std::cout << "(" << comma << ")";
        }
        if (output_induced_subtree_entire and !output_induced_subtree_differential) {
            show_induced_subtree();
        }
        return;
    }

    ++rec_depth;
    const Vertex* v = CAND.get_head()->get_vertex();

    candidate_no_add(v);

    if (output_search_tree_parenthesis) {
        std::cout << "(";
    }

    rec_enumerate_output();

    if (output_search_tree_parenthesis) {
        std::cout << ")" << comma;
    }

    restore_candidate_no_add();

    ++induced_subtree_size;
    induced_subtree[induced_subtree_size - 1] = v;
    update(v);
    
    if (output_search_tree_parenthesis) {
        std::cout << "(";
    }

    if (output_induced_subtree_differential) {
        std::cout << v->get_label() << " "; 
        if (induced_subtree_size % 2 == 1) {
            std::cout << std::endl; 
            if (output_induced_subtree_entire) {
                show_induced_subtree();
            }
            std::cout << "* "; 
        }
    }

    rec_enumerate_output();

    if (output_induced_subtree_differential) {
        if (induced_subtree_size % 2 == 0) {
            std::cout << std::endl; 
            if (output_induced_subtree_entire) {
                show_induced_subtree();
            }
            std::cout << "* "; 
        }
        std::cout << "-" << v->get_label() << " "; 
    }

    if (output_search_tree_parenthesis) {
        std::cout << ")";
    }

    restore(v);
    --induced_subtree_size;
    --rec_depth;
}

bool EnumInducedSubtrees::update(const Vertex* v)
{
    const int v_id = v->get_id();
    AdjacentList* v_adj_list = adjacent_lists[v_id].get();

    // Update CAND (remove)
    cand_remove_count_history[rec_depth] = -1;
    {
        CandItem* v_cand_item = cand_items[v_id].get();
        in_cand[v_id] = false;
        CAND.remove_item(v_cand_item);
        cand_remove_history[rec_depth][++cand_remove_count_history[rec_depth]] =
            v_cand_item;
    } 
    for (AdjItem* u_item = v_adj_list->get_larger_head();
         u_item != v_adj_list->larger_head_tail.get();
         u_item = u_item->get_next()) {
        const Vertex* u = u_item->get_vertex();
        const int u_id = u->get_id();
        if (in_cand[u_id] == true) {
            in_cand[u_id] = false;
            cand_remove_history[rec_depth]
                               [++cand_remove_count_history[rec_depth]] =
                                   cand_items[u_id].get();
            CAND.remove_item(cand_items[u_id].get());
        }
    }
    // END Update CAND (remove)

    // Update CAND (add) and smaller adjacents move to avoiding set
    
    int addible_vertices_count = -1; 


    cand_add_count_history[rec_depth]  = -1;
    adj_count_history[rec_depth] = -1;

    for (AdjItem* u_item = v_adj_list->get_smaller_head();
         u_item != v_adj_list->smaller_head_tail.get();) {
        auto u_item_next = u_item->get_next();

        const Vertex* u = u_item->get_vertex();
        const int u_id = u->get_id();
        if (added_candidate[u_id] == true) {
            std::cerr << "ERR! CAN NOT ADD u!" << std::endl;
            exit(1);
        }
        added_candidate[u_id] = true;
        in_cand[u_id] = true;
        addible_vertices[++addible_vertices_count] = u;
        cand_add_history[rec_depth][++cand_add_count_history[rec_depth]] 
            = cand_items[u_id].get();

        adj_history[rec_depth][++adj_count_history[rec_depth]] =
            std::make_tuple(u_item, u_item->get_prev(), u_item->get_next());

        v_adj_list->remove_item(u_item);
        v_adj_list->push_back_to_smaller_avoiding(u_item);

        u_item = u_item_next;
    }

    for (AdjItem* u_item = v_adj_list->get_larger_head();
         u_item != v_adj_list->larger_head_tail.get();
         u_item = u_item->get_next()) {
        const Vertex* u = u_item->get_vertex();
        const int u_id = u->get_id();
        if (added_candidate[u_id] == false) {
            added_candidate[u_id] = true;
            in_cand[u_id] = true;
            addible_vertices[++addible_vertices_count] = u;
            cand_add_history[rec_depth][++cand_add_count_history[rec_depth]] 
                = cand_items[u_id].get();
        }
    }

    CAND.merge(cand_add_history[rec_depth].get(), cand_add_count_history[rec_depth]+1);
    // END Update CAND (add) and smaller adjacents move to avoiding set

    // Update larger adjacents of added adjacents of v
    adjadj_count_history[rec_depth] = -1;
    for (int i = 0; i <= addible_vertices_count; ++i) {
        const Vertex * u = addible_vertices[i]; 
        AdjacentList* u_adj_list = adjacent_lists[u->get_id()].get();
        for (AdjItem* w_item = u_adj_list->get_larger_head();
             w_item != u_adj_list->larger_head_tail.get();
             w_item = w_item->get_next()) {
            const Vertex* w = w_item->get_vertex();
            const int w_id = w->get_id();
            const int u_id = u->get_id();
            AdjacentList* w_adj_list = adjacent_lists[w_id].get();
            AdjItem* u_item = adj_items[w_id][u_id].get();

            adjadj_history[rec_depth][++adjadj_count_history[rec_depth]] =
                std::make_tuple(w, u, u_item->get_prev(), u_item->get_next());
            w_adj_list->remove_item(u_item);
            w_adj_list->push_back_to_smaller_avoiding(u_item);
        }
    }

    return true;
}

bool EnumInducedSubtrees::restore(const Vertex* v)
{
    for (int i = (int)adjadj_count_history[rec_depth]; i >= 0; --i) {
        const Vertex* w = std::get<0>(adjadj_history[rec_depth][i]);
        const Vertex* u = std::get<1>(adjadj_history[rec_depth][i]);
        auto prev_item = std::get<2>(adjadj_history[rec_depth][i]);
        auto next_item = std::get<3>(adjadj_history[rec_depth][i]);

        const int w_id = w->get_id();
        const int u_id = u->get_id();
        AdjacentList* w_adj_list = adjacent_lists[w_id].get();
        AdjItem* u_item = adj_items[w_id][u_id].get();

        w_adj_list->remove_item(u_item);

        prev_item->set_next(u_item);
        next_item->set_prev(u_item);
        u_item->set_prev(prev_item);
        u_item->set_next(next_item);
    }

    for (int i = (int)cand_add_count_history[rec_depth]; i >= 0; --i) {
        CandItem* u_item = cand_add_history[rec_depth][i];

        const int u_id = u_item->get_vertex()->get_id();
        added_candidate[u_id] = false;
        in_cand[u_id]         = false;

        CAND.remove_item(u_item);
    }

    for (int i = (int)adj_count_history[rec_depth]; i >= 0; --i) {
        const Vertex* u = std::get<0>(adj_history[rec_depth][i])->get_vertex();
        auto u_item = std::get<0>(adj_history[rec_depth][i]);
        auto prev_item = std::get<1>(adj_history[rec_depth][i]);
        auto next_item = std::get<2>(adj_history[rec_depth][i]);

        added_candidate[u->get_id()] = false;
        adjacent_lists[v->get_id()]->remove_item(u_item);

        prev_item->set_next(u_item);
        next_item->set_prev(u_item);
        u_item->set_prev(prev_item);
        u_item->set_next(next_item);
    }

    for (int i = (int)cand_remove_count_history[rec_depth]; i >= 0; --i) {
        CandItem* v_cand_item = cand_remove_history[rec_depth][i];
        CandItem* v_cand_item_prev = v_cand_item->get_prev();
        CandItem* v_cand_item_next = v_cand_item->get_next();

        in_cand[v_cand_item->get_vertex()->get_id()] = true;

        v_cand_item_next->set_prev(v_cand_item);
        v_cand_item_prev->set_next(v_cand_item);
    }

    return true;
}

// v is the minimum vertex of the candidate set.
bool EnumInducedSubtrees::candidate_no_add(const Vertex* v)
{
    const int v_id        = v->get_id();
    in_cand[v_id]         = false;
    CandItem* cand_item_v = cand_items[v_id].get();
    CAND.remove_item(cand_item_v);
    no_cand_cand_history[rec_depth] = std::move(cand_item_v);

    return true;
}

bool EnumInducedSubtrees::restore_candidate_no_add()
{
    CandItem* cand_item_v = no_cand_cand_history[rec_depth];
    CandItem* cand_item_v_prev = cand_item_v->get_prev();
    CandItem* cand_item_v_next = cand_item_v->get_next();
    cand_item_v_prev->set_next(cand_item_v);
    cand_item_v_next->set_prev(cand_item_v);

    return true;
}

void EnumInducedSubtrees::show_graph()
{
    // std::cout << "SHOW GRAPH" << std::endl;
    // for (const auto& v : g->get_sorted_vector()) {
    // adjacent_lists[g->get_vertex(v)]->show();
    // }
    g->show();
    std::cout << std::endl;
}

void EnumInducedSubtrees::show_induced_subtree()
{
    // std::cout << "Show induced subtree ";
    for (int i = 0; i < induced_subtree_size; ++i) {
        std::cout << induced_subtree[i]->get_label() << " ";
    }
    std::cout << std::endl;
}
