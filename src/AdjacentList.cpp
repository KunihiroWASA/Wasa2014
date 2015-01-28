#include "Vertex.hpp"
#include "AdjacentList.hpp"


AdjacentList::AdjacentList(const Vertex* __v)
    : v(__v),
      larger_head_tail(new AdjItem(nullptr)),
      smaller_head_tail(new AdjItem(nullptr)),
      smaller_avoiding_head_tail(new AdjItem(nullptr))
{
    larger_head_tail->set_prev(larger_head_tail.get());
    larger_head_tail->set_next(larger_head_tail.get());

    smaller_head_tail->set_prev(smaller_head_tail.get());
    smaller_head_tail->set_next(smaller_head_tail.get());

    smaller_avoiding_head_tail->set_prev(smaller_avoiding_head_tail.get());
    smaller_avoiding_head_tail->set_next(smaller_avoiding_head_tail.get());
};

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
        std::cout << a->get_vertex()->get_label() << " ";
    }
    std::cout << "-- ";
    for (AdjItem* a = get_smaller_avoiding_head();
         a != smaller_avoiding_head_tail.get(); a = a->get_next()) {
        std::cout << a->get_vertex()->get_label() << " ";
    }
    std::cout << "-- ";
    for (AdjItem* a = get_larger_head(); a != larger_head_tail.get();
         a = a->get_next()) {
        std::cout << a->get_vertex()->get_label() << " ";
    }
    std::cout << std::endl;
}


void AdjacentList::remove_item(AdjItem* a)
{
    auto prev = a->get_prev();
    auto next = a->get_next();
    prev->set_next(next);
    next->set_prev(prev);
}
