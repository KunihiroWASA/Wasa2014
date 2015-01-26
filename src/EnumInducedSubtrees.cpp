#include "EnumInducedSubtrees.hpp"

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
            }
            else {
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

    for (const auto& v_id : g->get_sorted_vector()) {
        const Vertex* v = g->get_vertex(v_id);
        added_candidate[v] = false;
    }

}

void EnumInducedSubtrees::enumerate()
{
    rec_depth = 0; 
    parenthesis = "("; 
    for (const auto& v_id : g->get_sorted_vector()) {
        const Vertex* v = g->get_vertex(v_id);
        CAND.set_head(cand_items[v].get());
        candidate_no_add(v);
        added_candidate[v] = true;
        std::cout << std::setw(rec_depth+3) << std::setfill(' ')<< v->get_id() << std::endl; 
        induced_subtree.push_back(v);
        update(v);
        parenthesis.push_back('('); 
        ++rec_depth;  
        rec_enumerate();
        --rec_depth;  
        parenthesis.push_back(')'); 
        restore(v);
        induced_subtree.pop_back();
        std::cout << std::setw(rec_depth+2) << std::setfill(' ')<< '-' << v->get_id() << std::endl; 
    }

    parenthesis.push_back(')'); 
    // std::cout << parenthesis << std::endl; 
}

void EnumInducedSubtrees::rec_enumerate()
{
    if (CAND.empty()) {
        // show_induced_subtree();
        return;
    }
    const Vertex* v = CAND.get_head()->get_vertex();

    candidate_no_add(v);
    parenthesis.push_back('('); 
    ++rec_depth;  
    rec_enumerate();
    --rec_depth;  
    parenthesis.push_back(')'); 
    restore_candidate_no_add();

    induced_subtree.push_back(v);
    std::cout << std::setw(rec_depth+3) << std::setfill(' ')<< v->get_id() << std::endl; 
    update(v);
    parenthesis.push_back('('); 
    ++rec_depth;  
    rec_enumerate();
    --rec_depth;  
    parenthesis.push_back(')'); 
    restore(v);
    std::cout << std::setw(rec_depth+2) << std::setfill(' ')<< '-' << v->get_id() << std::endl; 
    induced_subtree.pop_back();

}

bool EnumInducedSubtrees::update(const Vertex* v)
{
    // std::cout << "Update (add " << v->get_id() << ")" << std::endl;
    CandItem* v_cand_item = cand_items[v].get();
    CAND.remove_item(v_cand_item);

    AdjacentList* v_adj_list = adjcent_lists[v].get();
    std::vector<const Vertex*> addible_vertices;

    std::vector<std::tuple<AdjItem*, AdjItem*, AdjItem*>> adj_ops;
    std::vector<std::tuple<const Vertex*, const Vertex*, AdjItem*, AdjItem*>>
        adjadj_ops;

    for (AdjItem* u_item = v_adj_list->get_smaller_head();
         u_item != v_adj_list->smaller_head_tail.get();) {
        const Vertex* u = u_item->get_vertex();
        auto u_item_next = u_item->get_next();

        added_candidate[u] = true;
        addible_vertices.push_back(u);

        v_adj_list->remove_item(u_item);
        adj_ops.push_back(
            std::make_tuple(u_item, u_item->get_prev(), u_item->get_next()));
        v_adj_list->push_back_to_smaller_avoiding(u_item);

        u_item = u_item_next;
    }

    for (AdjItem* u_item = v_adj_list->get_larger_head();
         u_item != v_adj_list->larger_head_tail.get();
         u_item = u_item->get_next()) {
        const Vertex* u = u_item->get_vertex();
        if (added_candidate[u] == false) {
            added_candidate[u] = true;
            adj_ops.push_back(std::make_tuple(u_item, nullptr, nullptr));
            addible_vertices.push_back(u);
        }
    }

    for (const auto& u : addible_vertices) {
        AdjacentList* u_adj_list = adjcent_lists[u].get();
        for (AdjItem* w_item = u_adj_list->get_larger_head();
             w_item != u_adj_list->larger_head_tail.get();
             w_item = w_item->get_next()) {
            const Vertex* w = w_item->get_vertex();
            AdjacentList* w_adj_list = adjcent_lists[w].get();
            AdjItem* item = adj_items[w][u].get();
            w_adj_list->remove_item(item);
            adjadj_ops.push_back(
                std::make_tuple(w, u, item->get_prev(), item->get_next()));
            w_adj_list->push_back_to_smaller_avoiding(item);
        }
    }

    std::vector<CandItem*> addible_cand_items;
    for (const auto& u : addible_vertices) {
        CandItem* u_item = cand_items[u].get();
        addible_cand_items.push_back(u_item);
    }

    CAND.merge(addible_cand_items);

    cand_remove_history.push(v_cand_item);
    cand_history.push(addible_cand_items);
    adj_history.push(adj_ops);
    adjadj_history.push(adjadj_ops);

    return true;
}

bool EnumInducedSubtrees::restore(const Vertex* v)
{
    std::vector<CandItem*> addible_cand_items = cand_history.top();
    cand_history.pop();
    for (int i = (int)addible_cand_items.size() - 1; i >= 0; --i) {
        CAND.remove_item(addible_cand_items.at(i));
    }

    CandItem* v_cand_item = cand_remove_history.top();
    CandItem* v_cand_item_prev = v_cand_item->get_prev();
    CandItem* v_cand_item_next = v_cand_item->get_next();
    cand_remove_history.pop();

    v_cand_item->set_next(v_cand_item_next);
    v_cand_item->set_prev(v_cand_item_prev);
    v_cand_item_next->set_prev(v_cand_item);
    v_cand_item_prev->set_next(v_cand_item);

    std::vector<std::tuple<const Vertex*, const Vertex*, AdjItem*, AdjItem*>>
        adjadj_ops = adjadj_history.top();
    adjadj_history.pop();
    for (int i = (int)adjadj_ops.size() - 1; i >= 0; --i) {
        const Vertex* w = std::get<0>(adjadj_ops.at(i));
        const Vertex* u = std::get<1>(adjadj_ops.at(i));
        auto prev_item = std::get<2>(adjadj_ops.at(i));
        auto next_item = std::get<3>(adjadj_ops.at(i));

        AdjItem* item = adj_items[w][u].get();

        adjcent_lists[w]->remove_item(item);

        prev_item->set_next(item);
        next_item->set_prev(item);
        item->set_prev(prev_item);
        item->set_next(next_item);
    }

    std::vector<std::tuple<AdjItem*, AdjItem*, AdjItem*>> adj_ops =
        adj_history.top();
    adj_history.pop();
    for (int i = (int)adj_ops.size() - 1; i >= 0; --i) {
        const Vertex* u = std::get<0>(adj_ops.at(i))->get_vertex();
        auto u_item = std::get<0>(adj_ops.at(i));
        auto prev_item = std::get<1>(adj_ops.at(i));
        auto next_item = std::get<2>(adj_ops.at(i));

        added_candidate[u] = false;
        if (prev_item != nullptr and next_item != nullptr) {
            adjcent_lists[v]->remove_item(u_item);

            prev_item->set_next(u_item);
            next_item->set_prev(u_item);
            u_item->set_prev(prev_item);
            u_item->set_next(next_item);
        }
    }

    return true;
}

// v is the minimum vertex of the candidate set.
bool EnumInducedSubtrees::candidate_no_add(const Vertex* v)
{
    AdjacentList* v_alist = adjcent_lists[v].get();
    std::vector<std::tuple<const Vertex*, const Vertex*, AdjItem*, AdjItem*>>
        no_cand_ops;

    CandItem* cand_item_v = cand_items[v].get();
    CandItem* cand_item_v_prev = cand_items[v]->get_prev();
    CandItem* cand_item_v_next = cand_items[v]->get_next();
    CAND.remove_item(cand_item_v);
    no_cand_cand_history.push(
        std::make_tuple(cand_item_v, cand_item_v_prev, cand_item_v_next));
    for (AdjItem* u_item = v_alist->get_larger_head();
         u_item != v_alist->larger_head_tail.get();
         u_item = u_item->get_next()) {
        const Vertex* u = u_item->get_vertex();
        AdjacentList* u_adj_list = adjcent_lists[u].get();
        AdjItem* item = adj_items[u][v].get();

        u_adj_list->remove_item(item);
        no_cand_ops.push_back(
            std::make_tuple(u, v, item->get_prev(), item->get_next()));
        u_adj_list->push_back_to_smaller_avoiding(item);
    }
    no_cand_adj_history.push(no_cand_ops);
    return true;
}

bool EnumInducedSubtrees::restore_candidate_no_add()
{
    std::vector<std::tuple<const Vertex*, const Vertex*, AdjItem*, AdjItem*>>
        no_cand_ops = no_cand_adj_history.top();
    no_cand_adj_history.pop();

    // added_candidate[v] = false;
    for (int i = (int)no_cand_ops.size() - 1; i > -1; --i) {
        const Vertex* u = std::get<0>(no_cand_ops.at(i));
        const Vertex* v = std::get<1>(no_cand_ops.at(i));
        AdjItem* prev_item = std::get<2>(no_cand_ops.at(i));
        AdjItem* next_item = std::get<3>(no_cand_ops.at(i));
    }

    std::tuple<CandItem*, CandItem*, CandItem*> no_cand_cand =
        no_cand_cand_history.top();
    no_cand_cand_history.pop();

    CandItem* cand_item_v = std::get<0>(no_cand_cand);
    CandItem* cand_item_v_prev = std::get<1>(no_cand_cand);
    CandItem* cand_item_v_next = std::get<1>(no_cand_cand);

    cand_item_v->set_prev(cand_item_v_prev);
    cand_item_v->set_next(cand_item_v_next);
    cand_item_v_prev->set_next(cand_item_v);
    cand_item_v_next->set_prev(cand_item_v);

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
        std::cout << v->get_id() << " ";
    }
    std::cout << std::endl;
}
