#include "Vertex.hpp"
#include "CandidateList.hpp"


CandidateList::CandidateList() : size(0), head_tail(new CandItem(nullptr)){};

void CandidateList::set_head(CandItem* v_item)
{
    if (empty()) {
        head_tail->set_next(v_item);
        head_tail->set_prev(v_item);
        v_item->set_next(head_tail.get());
        v_item->set_prev(head_tail.get());
    } else {
        std::cerr << "ERROR (1)" << std::endl; 
        exit(1); 
    }
}

void CandidateList::merge(std::vector<CandItem*>& addible_cand_items)
{
    auto head = get_head();

    size_t i = 0;
    size_t aci_size = addible_cand_items.size();
    CandItem* c = get_head();
    CandItem* ht_p = head_tail.get();

    while (c != ht_p and i < aci_size) {
        CandItem* d = addible_cand_items.at(i);

        if ((d->get_vertex())->get_degeneracy_id() <
            c->get_vertex()->get_degeneracy_id()) {
            CandItem* c_prev = c->get_prev();

            c_prev->set_next(d);
            d->set_prev(c_prev);

            d->set_next(c);
            c->set_prev(d);
            ++i;
        } else {
            c = c->get_next();
        }
    }
    for (; i < aci_size; ++i) {
        CandItem* tail = get_tail();
        CandItem* c = tail->get_next();
        CandItem* d = addible_cand_items.at(i);

        d->set_next(c);
        d->set_prev(tail);

        c->set_prev(d);
        tail->set_next(d);
    }
}

void CandidateList::remove_item(CandItem* c_item)
{
    auto c_item_prev = c_item->get_prev();
    auto c_item_next = c_item->get_next();

    c_item_prev->set_next(c_item_next);
    c_item_next->set_prev(c_item_prev);
}

void CandidateList::show()
{
    std::cout << "CAND: ";
    for (CandItem* c = get_head(); c != head_tail.get(); c = c->get_next()) {
        std::cout << c->get_vertex()->get_label() << "("
                  << c->get_vertex()->get_degeneracy_id() << ") ";
    }
    std::cout << std::endl;
}

