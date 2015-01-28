#include "EnumInducedSubtrees.hpp"

EnumInducedSubtrees::EnumInducedSubtrees():
    make_parenthesis(false), 
    debug_output(false), 
    output_differential(false), 
    induced_subtrees_num(0)
{};

void EnumInducedSubtrees::set_make_parenthesis(bool b)
{
    make_parenthesis = b; 
} 

void EnumInducedSubtrees::set_debug_output(bool b)
{
    debug_output = b; 
} 

void EnumInducedSubtrees::set_output_differential(bool b)
{
    output_differential = b; 
} 

void EnumInducedSubtrees::init_graph(Graph* __g)
{
    g = __g;
    g->sort_by_degeneracy();
    g->sort_vertices();

    // init adjacent lists
    for (const auto& v_id : g->get_sorted_vector()) {
        const Vertex* v = g->get_vertex(v_id);
        std::unique_ptr<AdjacentList> v_alist(new AdjacentList(v));
        adjcent_lists[v] = std::move(v_alist);
        for (const Vertex* u : v->get_adjacents()) {
            std::unique_ptr<AdjItem> v_u(new AdjItem(u));
            AdjItem* v_u_p = v_u.get();
            adj_items[v][u] = std::move(v_u);

            if (v->get_degeneracy_id() < u->get_degeneracy_id()) {
                adjcent_lists[v]->push_back_to_larger(v_u_p);
            } else {
                adjcent_lists[v]->push_back_to_smaller(v_u_p);
            }
        }
    }

    // init candidate list
    for (const auto& v_id : g->get_sorted_vector()) {
        const Vertex* v = g->get_vertex(v_id);
        std::unique_ptr<CandItem> v_citem(new CandItem(v));
        cand_items[v] = std::move(v_citem);
    }

    added_candidate.reset(new bool[g->get_sorted_vector().size()]); 
    in_cand.reset(new bool[g->get_sorted_vector().size()]); 
    for (const auto& v_id : g->get_sorted_vector()) {
        const Vertex* v = g->get_vertex(v_id);
        added_candidate[v_id] = false;
        in_cand[v_id] = false;
    }
}

void EnumInducedSubtrees::set_head(const Vertex * v)
{
    CAND.set_head(cand_items[v].get());
    added_candidate[v->get_id()]    = true;
    AdjacentList* v_alist = adjcent_lists[v].get();
    for (AdjItem* u_item = v_alist->get_larger_head();
            u_item != v_alist->larger_head_tail.get();
            u_item = u_item->get_next()) {

        const Vertex* u          = u_item->get_vertex();
        AdjacentList* u_adj_list = adjcent_lists[u].get();
        AdjItem* v_item          = adj_items[u][v].get();

        u_adj_list->remove_item(v_item);
        u_adj_list->push_back_to_smaller_avoiding(v_item);
    }
}

void EnumInducedSubtrees::enumerate()
{
    std::cout << "START ENUMERATION: " << std::endl;
    rec_depth = 0;
    if (make_parenthesis) {
        parenthesis = "(";
    }
    for (const auto& v_id : g->get_sorted_vector()) {

        const Vertex * v = g->get_vertex(v_id); 
        set_head(v); 
        induced_subtree.push_back(v);
        update(v);

        if (debug_output) {
            std::cout << std::endl; 
            if (output_differential) {
            std::cout << std::setw(rec_depth + 3) << std::setfill(' ')
                      << v->get_label() << std::endl;
            ++rec_depth;
            }
        }
        if (make_parenthesis) {
            parenthesis.push_back('(');
        }

        rec_enumerate();

        if (make_parenthesis) {
            parenthesis.push_back(')');
        }
        if (debug_output and output_differential) {
            --rec_depth;
            std::cout << std::setw(rec_depth + 2) << std::setfill(' ') << '-'
                      << v->get_label() << std::endl;
        }

        restore(v);
        induced_subtree.pop_back();
        in_cand[v->get_id()] = false; 
        CAND.remove_item(cand_items[v].get()); 
    }

    if (make_parenthesis) {
        parenthesis.push_back(')');
        std::cout << parenthesis << std::endl;
    }
    std::cout << "SUBTREE NUM.: " << induced_subtrees_num << std::endl; 
    return; 
}

void EnumInducedSubtrees::rec_enumerate()
{

    const Vertex* v = CAND.get_head()->get_vertex();

    if (CAND.empty()) {
        ++induced_subtrees_num; 

        if (debug_output and !output_differential) {
            show_induced_subtree();
        }
        return;
    }

    candidate_no_add(v);

    if (make_parenthesis) {
        parenthesis.push_back('(');
    }
    if (debug_output and output_differential) {
        ++rec_depth;
    }

    rec_enumerate();

    if (make_parenthesis) {
        parenthesis.push_back(')');
    }

    restore_candidate_no_add();


    induced_subtree.push_back(v);
    update(v);

    if (make_parenthesis) {
        parenthesis.push_back('(');
    }

    if (debug_output and output_differential) {
        std::cout << std::setw(rec_depth + 3) << std::setfill(' ') << v->get_label() << std::endl;
    }

    rec_enumerate();

    if (debug_output and output_differential) {
        --rec_depth;
        std::cout << std::setw(rec_depth + 2) << std::setfill(' ') << '-'
                  << v->get_label() << std::endl;
    }

    if (make_parenthesis) {
        parenthesis.push_back(')');
    }

    restore(v);
    induced_subtree.pop_back();

}

bool EnumInducedSubtrees::update(const Vertex* v)
{
    AdjacentList* v_adj_list = adjcent_lists[v].get();

    // Update CAND (remove)
    std::vector<CandItem*> cand_remove; 
    {
        CandItem* v_cand_item = cand_items[v].get();
        in_cand[v->get_id()] = false; 
        CAND.remove_item(v_cand_item);
        cand_remove.push_back(v_cand_item); 
    }

    for (AdjItem* u_item = v_adj_list->get_larger_head();
            u_item != v_adj_list->larger_head_tail.get();
            u_item = u_item->get_next()) {
        const Vertex* u = u_item->get_vertex();
        if (in_cand[u->get_id()] == true) {
            in_cand[u->get_id()] = false; 
            cand_remove.push_back(cand_items[u].get()); 
            CAND.remove_item(cand_items[u].get()); 
        }
    }
    cand_remove_history.push(cand_remove);
    // END Update CAND (remove)

    // Update CAND (add) and smaller adjacents move to avoiding set
    std::vector<const Vertex*> addible_vertices;
    std::vector<CandItem*> addible_cand_items;

    std::vector<std::tuple<AdjItem*, AdjItem*, AdjItem*>> adj_ops;
    for (AdjItem* u_item = v_adj_list->get_smaller_head();
         u_item != v_adj_list->smaller_head_tail.get();) {
        auto u_item_next = u_item->get_next(); 

        const Vertex* u  = u_item->get_vertex();
        if (added_candidate[u->get_id()] == true) {
            std::cerr << "ERR! CAN NOT ADD u!" <<std::endl; 
            exit(1);
        }
        added_candidate[u->get_id()] = true;
        in_cand[u->get_id()]         = true;
        addible_vertices.push_back(u);
        addible_cand_items.push_back(cand_items[u].get());

        adj_ops.push_back(
            std::make_tuple(u_item, u_item->get_prev(), u_item->get_next()));

        v_adj_list->remove_item(u_item);
        v_adj_list->push_back_to_smaller_avoiding(u_item);


        u_item = u_item_next; 
    }
    adj_history.push(adj_ops);

    for (AdjItem* u_item = v_adj_list->get_larger_head();
            u_item != v_adj_list->larger_head_tail.get();
            u_item = u_item->get_next()) {
        const Vertex* u = u_item->get_vertex();
        if (added_candidate[u->get_id()] == false) {
            added_candidate[u->get_id()] = true;
            in_cand[u->get_id()]         = true;
            addible_vertices.push_back(u);
            addible_cand_items.push_back(cand_items[u].get());
        }
    }
    cand_add_history.push(addible_cand_items);

    CAND.merge(addible_cand_items);
    // END Update CAND (add) and smaller adjacents move to avoiding set


    // Update larger adjacents of added adjacents of v 
    std::vector<std::tuple<const Vertex*, const Vertex*, AdjItem*, AdjItem*>>
        adjadj_ops;
    for (const auto& u : addible_vertices) {
        AdjacentList* u_adj_list = adjcent_lists[u].get();
        for (AdjItem* w_item = u_adj_list->get_larger_head();
             w_item != u_adj_list->larger_head_tail.get();
             w_item = w_item->get_next()) {

            const Vertex* w          = w_item->get_vertex();
            AdjacentList* w_adj_list = adjcent_lists[w].get();
            AdjItem* u_item          = adj_items[w][u].get();

            adjadj_ops.push_back(
                std::make_tuple(w, u, u_item->get_prev(), u_item->get_next()));
            w_adj_list->remove_item(u_item);
            w_adj_list->push_back_to_smaller_avoiding(u_item);
        }
    }
    adjadj_history.push(adjadj_ops);

    return true;
}

bool EnumInducedSubtrees::restore(const Vertex* v)
{
    for (int i = (int)adjadj_history.top().size() - 1; i >= 0; --i) {
        auto adjadj_ops = adjadj_history.top().at(i); 
        const Vertex* w = std::get<0>(adjadj_ops);
        const Vertex* u = std::get<1>(adjadj_ops);
        auto prev_item  = std::get<2>(adjadj_ops);
        auto next_item  = std::get<3>(adjadj_ops);

        AdjacentList* w_adj_list = adjcent_lists[w].get();
        AdjItem* u_item          = adj_items[w][u].get();

        w_adj_list->remove_item(u_item);

        prev_item->set_next(u_item);
        next_item->set_prev(u_item);
        u_item->set_prev(prev_item);
        u_item->set_next(next_item);
    }
    adjadj_history.pop();


    for (int i = (int)cand_add_history.top().size() - 1; i >= 0; --i) {
        CandItem* u_item      = cand_add_history.top().at(i);

        added_candidate[u_item->get_vertex()->get_id()] = false; 
        in_cand[u_item->get_vertex()->get_id()] = false; 

        CAND.remove_item(u_item);

    }
    cand_add_history.pop();

    std::vector<std::tuple<AdjItem*, AdjItem*, AdjItem*>> adj_ops =
        adj_history.top();
    for (int i = (int)adj_ops.size() - 1; i >= 0; --i) {
        const Vertex* u    = std::get<0>(adj_ops.at(i))->get_vertex();
        auto u_item        = std::get<0>(adj_ops.at(i));
        auto prev_item     = std::get<1>(adj_ops.at(i));
        auto next_item     = std::get<2>(adj_ops.at(i));

        added_candidate[u->get_id()] = false; 
        adjcent_lists[v]->remove_item(u_item);

        prev_item->set_next(u_item);
        next_item->set_prev(u_item);
        u_item->set_prev(prev_item);
        u_item->set_next(next_item);
    }
    adj_history.pop();

    for (int i = (int)cand_remove_history.top().size() - 1; i >= 0; --i) {

        CandItem* v_cand_item      = cand_remove_history.top().at(i);
        CandItem* v_cand_item_prev = v_cand_item->get_prev();
        CandItem* v_cand_item_next = v_cand_item->get_next();

        in_cand[v_cand_item->get_vertex()->get_id()] = true; 

        v_cand_item_next->set_prev(v_cand_item);
        v_cand_item_prev->set_next(v_cand_item);
    }
    cand_remove_history.pop();


    return true;
}

// v is the minimum vertex of the candidate set.
bool EnumInducedSubtrees::candidate_no_add(const Vertex * v)
{

    {
        in_cand[v->get_id()] = false; 
        CandItem* cand_item_v = cand_items[v].get(); 
        CAND.remove_item(cand_item_v);
        no_cand_cand_history.push(cand_item_v);
    }


    return true;
}

bool EnumInducedSubtrees::restore_candidate_no_add()
{

    {
        CandItem* cand_item_v      = no_cand_cand_history.top(); 
        CandItem* cand_item_v_prev = cand_item_v->get_prev();
        CandItem* cand_item_v_next = cand_item_v->get_next();

        cand_item_v_prev->set_next(cand_item_v);
        cand_item_v_next->set_prev(cand_item_v);
    }
    no_cand_cand_history.pop();

    return true;
}

void EnumInducedSubtrees::show_graph()
{
    // std::cout << "SHOW GRAPH" << std::endl;
    // for (const auto& v : g->get_sorted_vector()) {
    // adjcent_lists[g->get_vertex(v)]->show();
    // }
    g->show();
    std::cout << std::endl;
    std::cout << std::endl;
}

void EnumInducedSubtrees::show_induced_subtree()
{
    std::cout << "SHOW INDUCED SUBTREE ";
    for (const auto& v : induced_subtree) {
        std::cout << v->get_label() << " ";
    }
    std::cout << std::endl;
}
