#include <assert.h>

#include "graph.hpp"

void test_find_shortest_path();

void test_topological_sort();

void test_find_connected_components();

void test_bellman_ford_0();
void test_bellman_ford_1();
void test_bellman_ford_2();

void test_floyd_wallsher();

void test_dijkstra_0();
void test_dijkstra_1();
void test_dijkstra_2();

void test() {
    test_find_shortest_path();
    test_topological_sort();
    test_find_connected_components();

    test_bellman_ford_0();
    test_bellman_ford_1();
    test_bellman_ford_2();

    test_floyd_wallsher();

    test_dijkstra_0();
    test_dijkstra_1();
    test_dijkstra_2();
}

void test_find_shortest_path() {
    auto source = graph<std::size_t>(5, true);

    source.new_edge(1, 0, 2);
    source.new_edge(0, 1, 2);
    source.new_edge(2, 0, 3);
    source.new_edge(2, 1, 4);
    source.new_edge(4, 3, 5);
    source.new_edge(2, 3, 6);
    source.add_vertex();
    source.new_edge(5, 0, 11);
    source.new_edge(4, 5, 22);

    auto result = source.find_shortest_path(2);
    std::vector<std::size_t> correct = {3, 4, 0, 6, UNREACH, UNREACH};

    for (std::size_t i = 0; i < result.size(); ++i) {
        assert(result[i] == correct[i]);
    }
}

void test_dijkstra_0() {
    auto source = graph<std::size_t>(5, true);

    source.new_edge(0, 1, 5);
    source.new_edge(0, 2, 7);
    source.new_edge(1, 2, 1);
    source.new_edge(1, 3, 3);
    source.new_edge(0, 3, 8);
    source.new_edge(3, 0, 5);
    source.new_edge(2, 3, 1);
    source.new_edge(2, 4, 8);
    source.new_edge(0, 4, 14);
    source.new_edge(3, 4, 3);

    bool error_message;
    auto result = source.dijkstra(0, 4, error_message);
    std::vector<std::size_t> correct = {0, 1, 2, 3, 4};

    for (std::size_t i = 0; i < 5; ++i) {
        assert(result[i] == correct[i]);
    }
}

void test_dijkstra_1() {
    auto source = graph<std::size_t>(5, true);

    source.new_edge(0, 1, 5);
    source.new_edge(0, 2, 7);
    source.new_edge(1, 2, 1);
    source.new_edge(1, 3, 3);
    source.new_edge(0, 3, 8);
    source.new_edge(3, 0, 5);
    source.new_edge(2, 3, 1);


    bool error_message;
    auto result = source.dijkstra(0, 4, error_message);
    std::vector<std::size_t> correct = {0, 1, 2, 3, 4};

    assert(error_message = 1);
}

void test_dijkstra_2() {
    auto source = graph<std::size_t>(7, true);

    source.new_edge(0, 3, 20);
    source.new_edge(0, 1, 1);
    source.new_edge(0, 2, 1);
    source.new_edge(2, 3, 15);
    source.new_edge(1, 3, 14);
    source.new_edge(0, 4, 10);
    source.new_edge(4, 5, 1);
    source.new_edge(5, 6, 1);
    source.new_edge(6, 3, 1);


    bool error_message;
    auto result = source.dijkstra(0, 3, error_message);
    std::vector<std::size_t> correct = {0, 4, 5, 6, 3};

    for (std::size_t i = 0; i < 5; ++i) {
        assert(result[i] == correct[i]);
    }
}

void test_topological_sort() {
    auto source = graph<int>(5, true);

    source.new_edge(1, 0, 2);
    source.new_edge(0, 1, 2);
    source.new_edge(2, 0, 3);
    source.new_edge(2, 1, 4);
    source.new_edge(4, 3, 5);
    source.new_edge(2, 3, 6);
    source.add_vertex();
    source.new_edge(5, 0, 11);
    source.new_edge(4, 5, 22);

    auto result = source.topological_sort();
    std::vector<std::size_t> correct = {4, 5, 2, 3, 0, 1};

    for (std::size_t i = 0; i < result.size(); i++) {
        assert(result[i] == correct[i]);
    }
}

void test_find_connected_components() {
    auto source = graph<int>(5, true);

    source.new_edge(1, 0, 2);
    source.new_edge(0, 1, 2);
    source.new_edge(2, 0, 3);
    source.new_edge(2, 1, 4);
    source.new_edge(4, 3, 5);
    source.new_edge(2, 3, 6);
    source.add_vertex();
    source.new_edge(5, 0, 11);
    source.new_edge(4, 5, 22);

    auto result = source.find_connected_components();
    std::vector<std::vector<bool>> correct = {{1, 1, 0, 0, 0, 0},
                                             {1, 1, 0, 0, 0, 0}, 
                                             {1, 1, 1, 1, 0, 0},
                                             {0, 0, 0, 1, 0, 0},
                                             {1, 1, 0, 1, 1, 1},
                                             {1, 1, 0, 0, 0, 1}};

    for (std::size_t i = 0; i < correct.size(); i++) {
        for (std::size_t j = 0; j < result.size(); ++j) {
            assert(result[i].operator[](j) == correct[i][j]);
        }
    }
}

void test_bellman_ford_0() {
    auto source = graph<int>(5, true);

    source.new_edge(0, 1, 3);
    source.new_edge(0, 2, 1);
    source.new_edge(1, 3, 2);
    source.new_edge(2, 3, 1);
    source.new_edge(3, 4, 4);

    bool error_message;
    std::vector<int> res = source.bellman_ford(0, error_message);
    std::vector<int> true_result = {0, 3, 1, 2, 6};

    for (std::size_t i = 0; i < 5; ++i) {
        assert(res[i] == true_result[i]);
    }
}

void test_bellman_ford_1() {
    auto source = graph<int>(6, true);

    source.new_edge(0, 1, -5);
    source.new_edge(0, 2, 3);
    source.new_edge(1, 3, -1);
    source.new_edge(2, 3, 1);
    source.new_edge(3, 4, 4);
    source.new_edge(3, 2, 1);
    source.new_edge(2, 5, -6);
    source.new_edge(0, 5, 4);

    bool error_message;
    std::vector<int> res = source.bellman_ford(0, error_message);
    std::vector<int> true_result = {0, -5, -5, -6, -2, -11};

    for (std::size_t i = 0; i < 6; ++i) {
        assert(res[i] == true_result[i]);
    }
}

void test_bellman_ford_2() {
    auto source = graph<int>(6, true);

    source.new_edge(0, 1, -5);
    source.new_edge(0, 2, 3);
    source.new_edge(1, 3, -1);
    source.new_edge(2, 3, 1);
    source.new_edge(3, 4, 4);
    source.new_edge(3, 2, 1);
    source.new_edge(2, 5, -6);
    source.new_edge(0, 5, 4);
    source.new_edge(2, 0, -1); // negative cycle

    bool error_message;
    std::vector<int> res = source.bellman_ford(0, error_message);

    assert(error_message = 1);
}

void test_floyd_wallsher() {
      auto source = graph<int>(5, true, UNREACH);

      source.new_edge(1, 0, 2);
      source.new_edge(0, 1, 2);
      source.new_edge(2, 0, 3);
      source.new_edge(2, 1, 4);
      source.new_edge(4, 3, 5);
      source.new_edge(2, 3, 6);
      source.add_vertex();
      source.new_edge(5, 0, 11);
      source.new_edge(4, 5, 22);

      auto wallsher = source.floyd_wallsher();
      std::vector<std::vector<std::size_t>> correct = {{0, 2, UNREACH, UNREACH, UNREACH, UNREACH, UNREACH},
                                                      {2, 0, UNREACH, UNREACH, UNREACH, UNREACH, UNREACH},
                                                      {3, 4, 0, 6, UNREACH, UNREACH},
                                                      {UNREACH, UNREACH, UNREACH, 0, UNREACH, UNREACH},
                                                      {33, 35, UNREACH, 5, 0, 22}, 
                                                      {11, 13, UNREACH, UNREACH, UNREACH, 0}};
      for (std::size_t i = 0; i < wallsher.size(); ++i) {
          for (std::size_t j = 0; j < wallsher.size(); ++j) {
              assert(wallsher.get_weight(i, j) == correct[i][j]);
          }
      }
}