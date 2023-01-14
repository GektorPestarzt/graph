#ifndef SRC_GRAPH_HPP_
#define SRC_GRAPH_HPP_

#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>

#define UNREACH 32450

template <typename T>
class graph {
public:
    void new_edge(std::size_t first_element, std::size_t second_element, const T& weight) {
        this->graph_[first_element][second_element] = weight;
        if (!this->is_directed) {
            this->graph_[second_element][first_element] = weight;
        }
    }

    void delete_edge(std::size_t first_element, std::size_t second_element) {
        this->graph_[first_element][second_element] = T(UNREACH);
        if (!this->is_directed) {
            this->graph_[second_element][first_element] = T(UNREACH);
        }
    }

    void add_vertex() {
        for (std::size_t i = 0; i < this->graph_.size(); ++i) {
            this->graph_[i].push_back(T(UNREACH));
        }
        this->graph_.push_back(std::vector<T>(this->graph_.size() + 1, T(UNREACH)));
    }

    void delete_vertex(std::size_t element) {
        for (std::size_t i = 0; i < this->graph_.size(); ++i) {
            this->graph_[i].erase(this->graph_[i].begin() + element);
        }
        this->graph_.erase(this->graph_.begin() + element);
    }

    const T& get_weight(std::size_t first_element, std::size_t second_element) {
        return this->graph_[first_element][second_element];
    }

    std::size_t size() { return this->graph_.size(); }

    std::vector<T> find_shortest_path(std::size_t start_element) {
        auto path = std::vector<T>(this->graph_.size(), T(UNREACH));
        auto visited = std::vector<bool>(this->graph_.size(), false);
    
        path[start_element] = T(0);
        for (std::size_t i = 0; i < this->graph_.size(); ++i) {
            std::size_t point = UNREACH;

            for (std::size_t j = 0; j < this->graph_.size(); ++j) {
                if (!visited[j] && (point == UNREACH || path[j] < path[point])) {
                    point = j;
                }
            }

            if (path[point] == UNREACH) break;
            visited[point] = true;

            for (std::size_t j = 0; j < this->graph_.size(); ++j) {
                if (this->graph_[point][j] == T(UNREACH)) continue;

                T length = this->graph_[point][j];
                if (path[point] + length < path[j]) {
                    path[j] = path[point] + length;
                }
            }
        }

        return path;
    }

    std::vector<T> dijkstra(std::size_t start_element, std::size_t end_element, bool &error_message) {
        auto parent = std::vector<std::size_t>(this->size(), UNREACH);
        auto path = std::vector<T>(this->graph_.size(), T(UNREACH));
        auto visited = std::vector<bool>(this->graph_.size(), false);
        error_message = 0;
    
        path[start_element] = T(0);
        for (std::size_t i = 0; i < this->graph_.size(); ++i) {
            std::size_t point = UNREACH;

            for (std::size_t j = 0; j < this->graph_.size(); ++j) {
                if (!visited[j] && (point == UNREACH || path[j] < path[point])) {
                    point = j;
                }
            }

            if (path[point] == UNREACH) break;
            visited[point] = true;

            for (std::size_t j = 0; j < this->graph_.size(); ++j) {
                if (this->graph_[point][j] == T(UNREACH)) continue;

                T length = this->graph_[point][j];
                if (path[point] + length < path[j]) {
                    path[j] = path[point] + length;
                    parent[j] = point;
                }
            }
        }

        auto path_by_elements = std::vector<std::size_t>();
        std::size_t current = end_element;
        while (current != start_element && current != UNREACH) {
            path_by_elements.push_back(parent[current]);
            current = parent[current];
        }
        if (current == start_element) {
            std::reverse(path_by_elements.begin(), path_by_elements.end());
            path_by_elements.push_back(end_element);
        } else {
            error_message = 1;
        }

        return path_by_elements;
    }

    std::vector<T> bellman_ford(std::size_t start_point, bool &error_message) {
        error_message = 0;
        auto path = std::vector<T>(this->size(), T(UNREACH));
        path[start_point] = T(0);

        for (std::size_t i = 0; i < this->size() - 1; ++i) {
            for (std::size_t u = 0; u < this->size(); ++u) {
                for (std::size_t p = 0; p < this->size(); ++p) {
                    T new_path = path[u] + this->graph_[u][p];
                    if (new_path < path[p]) {
                        path[p] = new_path;
                    }
                }
            }
        }

        for (std::size_t u = 0; u < this->size(); ++u) {
            for (std::size_t p = 0; p < this->size(); ++p) {
                T new_path = path[u] + this->graph_[u][p];
                if (new_path < path[p]) {
                    error_message = 1;
                    break;
                }
            }
            if (error_message) break;
        }

        return path;
    }

    graph<std::size_t> floyd_wallsher() {
        auto path_matrix = graph<std::size_t>(this->size(), this->is_directed, 0);
        for (std::size_t i = 0; i < this->size(); ++i) {
            for (std::size_t j = 0; j < this->size(); ++j) {
                if (i != j) path_matrix[i][j] = this->graph_[i][j];
            }
        }

        for (std::size_t k = 0; k < this->size(); ++k) {
            for (std::size_t i = 0; i < this->size(); ++i) {
                for (std::size_t j = 0; j < this->size(); ++j) {
                    if (path_matrix[i][k] != UNREACH
                        && path_matrix[k][j] != UNREACH) {
                        path_matrix[i][j]
                        = std::min(path_matrix[i][j],
                                path_matrix[i][k] + path_matrix[k][j]);
                    }
                }
            }
        }

        return path_matrix;
    }

    void topological_sort_rec(std::size_t point, std::vector<bool> &visited, std::stack<std::size_t> &stack) {
        visited[point] = true;
    
        for (std::size_t i = 0; i < this->graph_.size(); ++i) {
            if (!visited[i] && this->get_weight(point, i) != T(UNREACH)) {
                topological_sort_rec(i, visited, stack);
            }
        }

        stack.push(point);
    }

    std::vector<std::size_t> topological_sort() {
        std::stack<std::size_t> stack_ = std::stack<std::size_t>();
    
        auto visited = std::vector<bool>(this->graph_.size());
        for (std::size_t i = 0; i < this->graph_.size(); ++i) { visited[i] = false; }

        for (std::size_t i = 0; i < this->graph_.size(); ++i) {
            if (!visited[i]) {
                this->topological_sort_rec(i, visited, stack_);
            }
        }

        auto new_order = std::vector<std::size_t>(this->graph_.size());
        for (std::size_t i = 0; i < this->graph_.size(); ++i) {
            new_order[i] = stack_.top();
            stack_.pop();
        }

        return new_order;
    }

    void depth_first_search(std::size_t start, std::vector<bool> &is_used) {
        is_used[start] = true;

        for (std::size_t i = 0; i < this->graph_.size(); ++i) {
            if (!is_used[i] && this->get_weight(start, i) != T(UNREACH)) {
                depth_first_search(i, is_used);
            }
        }
    }

    std::vector<std::vector<bool>> find_connected_components() {
        auto connected_components = std::vector<std::vector<bool>>();

        for (std::size_t i = 0; i < this->graph_.size(); ++i) {
            connected_components.push_back(std::vector<bool>(this->graph_.size()));
            for (std::size_t j = 0; j < this->graph_.size(); ++j) connected_components[i][j] = false;

                depth_first_search(i, connected_components[i]);
        }

        return connected_components;
    }

    void print() {
        for (std::size_t i = 0; i < this->graph_.size(); ++i) {
            for (std::size_t j = 0; j < this->graph_.size(); ++j) {
                std::cout << this->graph_[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    std::vector<T> &operator [](std::size_t idx) {
        return this->graph_[idx];
    }

public:
    graph(std::size_t elements_count, bool is_directed)
    : graph_(std::vector<std::vector<T>>(elements_count)), is_directed(is_directed) {
        for (std::size_t i = 0; i < elements_count; ++i) {
            this->graph_[i] = std::vector<T>(elements_count);
            for (std::size_t j = 0; j < elements_count; ++j) {
                this->graph_[i][j] = T(UNREACH);
            }
        }
    }

    graph(std::size_t elements_count, bool is_directed, const T& weight)
    : graph_(std::vector<std::vector<T>>(elements_count)), is_directed(is_directed) {
        for (std::size_t i = 0; i < elements_count; ++i) {
            this->graph_[i] = std::vector<T>(elements_count);
            for (std::size_t j = 0; j < elements_count; ++j) {
                this->graph_[i][j] = weight;
            }
        }
    }

private:
    std::vector<std::vector<T>> graph_;
    bool is_directed = false;
};

#endif  // SRC_GRAPH_HPP_
