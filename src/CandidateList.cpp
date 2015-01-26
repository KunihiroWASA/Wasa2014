#include "CandidateList.hpp"
#include "Vertex.hpp"

CandItem* CandItem::get_next()
{
    return cand_next;
}

CandItem* CandItem::get_prev()
{
    return cand_prev;
}

const Vertex* CandItem::get_vertex() const
{
    return v;
}

void CandItem::set_next(CandItem* next)
{
    cand_next = next;
}

void CandItem::set_prev(CandItem* prev)
{
    cand_prev = prev;
}

void CandidateList::set_head(CandItem* v_item)
{
    head_tail->set_next(v_item);
    head_tail->set_prev(v_item);
    v_item->set_next(head_tail.get());
    v_item->set_prev(head_tail.get());
}

CandItem* CandidateList::get_head()
{
    return head_tail->get_next();
}

CandItem* CandidateList::get_tail()
{
    return head_tail->get_prev();
}

void CandidateList::inc_size()
{
    ++size;
}

void CandidateList::dec_size()
{
    --size;
}

size_t CandidateList::get_size()
{
    return size;
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
            CandItem* e = c->get_prev();

            d->set_next(c);
            d->set_prev(e);

            c->set_prev(d);
            e->set_next(d);
            ++i;
        }
        else {
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
        std::cout << c->get_vertex()->get_id() << "("
                  << c->get_vertex()->get_degeneracy_id() << ") ";
    }
    std::cout << std::endl;
}

bool CandidateList::empty()
{
    return head_tail.get()->get_next() == head_tail.get();
}
