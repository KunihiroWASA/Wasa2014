#include "AdjacentList.hpp"
#include "Vertex.hpp"

void AdjItem::set_next(AdjItem* __next)
{
    adj_next = __next;
}

void AdjItem::set_prev(AdjItem* __prev)
{
    adj_prev = __prev;
}

const Vertex* AdjItem::get_vertex() const
{
    return v;
}

AdjItem* AdjItem::get_next()
{
    return adj_next;
}

AdjItem* AdjItem::get_prev()
{
    return adj_prev;
}

void AdjacentList::push_back_to_larger(AdjItem* a)
{
    auto tail = get_larger_tail();
    tail->set_next(a);
    a->set_prev(tail);
    a->set_next(larger_head_tail.get());
    larger_head_tail->set_prev(a);

    // ++larger_size;
}

void AdjacentList::push_back_to_smaller(AdjItem* a)
{
    auto tail = get_smaller_tail();
    tail->set_next(a);
    a->set_prev(tail);
    a->set_next(smaller_head_tail.get());
    smaller_head_tail->set_prev(a);

    // ++smaller_size;
}

void AdjacentList::push_back_to_smaller_avoiding(AdjItem* a)
{
    auto tail = get_smaller_avoiding_tail();
    tail->set_next(a);
    a->set_prev(tail);
    a->set_next(smaller_avoiding_head_tail.get());
    smaller_avoiding_head_tail->set_prev(a);

    // ++smaller_avoiding_size;
}

void AdjacentList::show()
{
    std::cout << v->get_id() << ": ";
    for (AdjItem* a = get_smaller_head(); a != smaller_head_tail.get();
         a = a->get_next()) {
        std::cout << a->get_vertex()->get_id() << " ";
    }
    std::cout << "-- ";
    for (AdjItem* a = get_smaller_avoiding_head();
         a != smaller_avoiding_head_tail.get(); a = a->get_next()) {
        std::cout << a->get_vertex()->get_id() << " ";
    }
    std::cout << "-- ";
    for (AdjItem* a = get_larger_head(); a != larger_head_tail.get();
         a = a->get_next()) {
        std::cout << a->get_vertex()->get_id() << " ";
    }
    std::cout << std::endl;
}

AdjItem* AdjacentList::get_larger_tail()
{
    return larger_head_tail->get_prev();
}

AdjItem* AdjacentList::get_smaller_tail()
{
    return smaller_head_tail->get_prev();
}

AdjItem* AdjacentList::get_smaller_avoiding_tail()
{
    return smaller_avoiding_head_tail->get_prev();
}

AdjItem* AdjacentList::get_larger_head()
{
    return larger_head_tail->get_next();
}

AdjItem* AdjacentList::get_smaller_head()
{
    return smaller_head_tail->get_next();
}

AdjItem* AdjacentList::get_smaller_avoiding_head()
{
    return smaller_avoiding_head_tail->get_next();
}

void AdjacentList::remove_item(AdjItem* a)
{
    auto prev = a->get_prev();
    auto next = a->get_next();
    prev->set_next(next);
    next->set_prev(prev);
}
