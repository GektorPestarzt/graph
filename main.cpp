#include <iostream>

#include "graph.hpp"
#include "test.hpp"

int main() {
    test();
    /*
    graph<std::size_t> *graph_ = new graph<std::size_t>(3, 0);
    graph_->new_edge(0, 1, 1);
    graph_->new_edge(0, 2, 2);
    graph_->new_edge(1, 2, 4);
    graph_->new_edge(2, 1, 2);
    graph_->new_edge(0, 1, 3);

    graph_->print();

    auto path = graph_->find_shortest_path(2);
    for (std::size_t i = 0; i < graph_->size(); ++i) std::cout << path->operator[](i) << " ";
    std::cout << std::endl; 
*/
    return 0;
}
