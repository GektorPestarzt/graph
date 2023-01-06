#include <assert.h>

#include "graph.hpp"

void test_find_shortest_path();
void test_topological_sort();
void test_find_connected_components();

void test() {
    test_find_shortest_path();
    test_topological_sort();
    test_find_connected_components();
}

void test_find_shortest_path() {
    auto source = new graph<std::size_t>(5, true);

    source->new_edge(1, 0, 2);
    source->new_edge(0, 1, 2);
    source->new_edge(2, 0, 3);
    source->new_edge(2, 1, 4);
    source->new_edge(4, 3, 5);
    source->new_edge(2, 3, 6);
    source->add_vertex();
    source->new_edge(5, 0, 11);
    source->new_edge(4, 5, 22);

    auto result = source->find_shortest_path(2);
    std::vector<std::size_t> correct = {3, 4, 0, 6, UNTOUCHED, UNTOUCHED};

    for (std::size_t i = 0; i < result->size(); i++) {
        assert(result->operator[](i) == correct[i]);
    }
    
    delete result;
    delete source;
}

void test_topological_sort() {
    auto source = new graph<int>(5, true);

    source->new_edge(1, 0, 2);
    source->new_edge(0, 1, 2);
    source->new_edge(2, 0, 3);
    source->new_edge(2, 1, 4);
    source->new_edge(4, 3, 5);
    source->new_edge(2, 3, 6);
    source->add_vertex();
    source->new_edge(5, 0, 11);
    source->new_edge(4, 5, 22);

    auto result = source->topological_sort();
    std::vector<std::size_t> correct = {4, 5, 2, 3, 0, 1};

    for (std::size_t i = 0; i < result->size(); i++) {
        assert(result->operator[](i) == correct[i]);
    }

    delete result;
    delete source;
}

void test_find_connected_components() {
    auto source = new graph<int>(5, true);

    source->new_edge(1, 0, 2);
    source->new_edge(0, 1, 2);
    source->new_edge(2, 0, 3);
    source->new_edge(2, 1, 4);
    source->new_edge(4, 3, 5);
    source->new_edge(2, 3, 6);
    source->add_vertex();
    source->new_edge(5, 0, 11);
    source->new_edge(4, 5, 22);

    auto result = source->find_connected_components();
    std::vector<std::vector<bool>> correct = {{1, 1, 0, 0, 0, 0},
                                             {1, 1, 0, 0, 0, 0}, 
                                             {1, 1, 1, 1, 0, 0},
                                             {0, 0, 0, 1, 0, 0},
                                             {1, 1, 0, 1, 1, 1},
                                             {1, 1, 0, 0, 0, 1}};

    for (std::size_t i = 0; i < correct.size(); i++) {
        for (std::size_t j = 0; j < result->size(); ++j) {
            assert(result->operator[](i)->operator[](j) == correct[i][j]);
        }
    }

    delete source;
    delete result->operator[](0);
    delete result;
}
