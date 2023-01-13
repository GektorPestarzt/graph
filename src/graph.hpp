#ifndef SRC_GRAPH_HPP_
#define SRC_GRAPH_HPP_

#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>

#define UNREACH 99999

template <typename T>
class graph {
public:
    void new_edge(std::size_t first_element, std::size_t second_element, const T& weight) {
        this->graph_->operator[](first_element)->operator[](second_element) = std::move(weight);
        if (!this->is_directed) {
            this->graph_->operator[](second_element)->operator[](first_element) = std::move(weight);
        }
    }

    void delete_edge(std::size_t first_element, std::size_t second_element) {
        this->graph_->operator[](first_element)->operator[](second_element) = T(UNREACH);
        if (!this->is_directed) {
            this->graph_->operator[](second_element)->operator[](first_element) = T(UNREACH);
        }
    }

    void add_vertex() {
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            this->graph_->operator[](i)->push_back(T(UNREACH));
        }
        this->graph_->push_back(new std::vector<T>(this->graph_->size() + 1, T(UNREACH)));
    }

    void delete_vertex(std::size_t element) {
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            this->graph_->operator[](i)->erase(this->graph_->operator[](i)->begin() + element);
        }
        this->graph_->erase(this->graph_->begin() + element);
    }

    const T& get_weight(std::size_t first_element, std::size_t second_element) {
        return this->graph_->operator[](first_element)->operator[](second_element);
    }

    std::size_t size() { return this->graph_->size(); }

    std::vector<T> *find_shortest_path(std::size_t start_element) {
        auto path = new std::vector<T>(this->graph_->size(), T(UNREACH));
        auto visited = new std::vector<bool>(this->graph_->size(), false);
    
        path->operator[](start_element) = T(0);
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            std::size_t point = UNREACH;

            for (std::size_t j = 0; j < this->graph_->size(); ++j) {
                if (!visited->operator[](j) && (point == UNREACH || path->operator[](j) < path->operator[](point))) {
                    point = j;
                }
            }

            if (path->operator[](point) == UNREACH) break;
            visited->operator[](point) = true;

            for (std::size_t j = 0; j < this->graph_->size(); ++j) {
                if (this->graph_->operator[](point)->operator[](j) == T(UNREACH)) continue;

                T length = this->graph_->operator[](point)->operator[](j);
                if (path->operator[](point) + length < path->operator[](j)) {
                    path->operator[](j) = path->operator[](point) + length;
                }
            }
        }

        delete visited;
        return path;
    }

    std::vector<T> *dijkstra(std::size_t start_element, std::size_t end_element) {
        auto parent = std::vector<std::size_t>(this->size(), UNREACH);
        auto path = new std::vector<T>(this->graph_->size(), T(UNREACH));
        auto visited = new std::vector<bool>(this->graph_->size(), false);
    
        path->operator[](start_element) = T(0);
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            std::size_t point = UNREACH;

            for (std::size_t j = 0; j < this->graph_->size(); ++j) {
                if (!visited->operator[](j) && (point == UNREACH || path->operator[](j) < path->operator[](point))) {
                    point = j;
                }
            }

            if (path->operator[](point) == UNREACH) break;
            visited->operator[](point) = true;

            for (std::size_t j = 0; j < this->graph_->size(); ++j) {
                if (this->graph_->operator[](point)->operator[](j) == T(UNREACH)) continue;

                T length = this->graph_->operator[](point)->operator[](j);
                if (path->operator[](point) + length < path->operator[](j)) {
                    path->operator[](j) = path->operator[](point) + length;
                    parent[j] = point;
                }
            }
        }

        auto path_by_elements = new std::vector<std::size_t>();
        std::size_t current = end_element;
        while (current != start_element && current != UNREACH) {
            path_by_elements->push_back(parent[current]);
            current = parent[current];
        }
        if (current == start_element) {
            std::reverse(path_by_elements->begin(), path_by_elements->end());
            path_by_elements->push_back(end_element);
        } else {
            delete path_by_elements;
            path_by_elements = nullptr;
        }

        delete visited;
        delete path;

        return path_by_elements;
    }

    std::vector<T> *bellman_ford(std::size_t start_point) {
        bool error = 0;
        auto path = new std::vector<T>(this->size(), T(UNREACH));
        path->operator[](start_point) = T(0);

        for (std::size_t i = 0; i < this->size() - 1; ++i) {
            for (std::size_t u = 0; u < this->size(); ++u) {
                for (std::size_t p = 0; p < this->size(); ++p) {
                    T new_path = path->operator[](u) + this->graph_->operator[](u)->operator[](p);
                    if (new_path < path->operator[](p)) {
                        path->operator[](p) = new_path;
                    }
                }
            }
        }

        for (std::size_t u = 0; u < this->size(); ++u) {
            for (std::size_t p = 0; p < this->size(); ++p) {
                T new_path = path->operator[](u) + this->graph_->operator[](u)->operator[](p);
                if (new_path < path->operator[](p)) {
                    error = 1;
                    break;
                }
            }
            if (error) break;
        }

        if (error) {
            delete path;
            path = nullptr;
        }

        return path;
    }

    graph<std::size_t> *floyd_wallsher() {
        auto path_matrix = new graph<std::size_t>(this->size(), this->is_directed, 0);
        for (std::size_t i = 0; i < this->size(); ++i) {
            for (std::size_t j = 0; j < this->size(); ++j) {
                if (i != j) path_matrix->operator[](i)->operator[](j) = this->graph_->operator[](i)->operator[](j);
            }
        }

        for (std::size_t k = 0; k < this->size(); ++k) {
            for (std::size_t i = 0; i < this->size(); ++i) {
                for (std::size_t j = 0; j < this->size(); ++j) {
                    if (path_matrix->operator[](i)->operator[](k) != UNREACH
                        && path_matrix->operator[](k)->operator[](j) != UNREACH) {
                        path_matrix->operator[](i)->operator[](j)
                        = std::min(path_matrix->operator[](i)->operator[](j),
                                path_matrix->operator[](i)->operator[](k) + path_matrix->operator[](k)->operator[](j));
                    }
                }
            }
        }

        return path_matrix;
    }

    void topological_sort_rec(std::size_t point, std::vector<bool> *visited, std::stack<std::size_t> *stack) {
        visited->operator[](point) = true;
    
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            if (!visited->operator[](i) && this->get_weight(point, i) != T(UNREACH)) {
                topological_sort_rec(i, visited, stack);
            }
        }

        stack->push(point);
    }

    std::vector<std::size_t> *topological_sort() {
        std::stack<std::size_t> *stack_ = new std::stack<std::size_t>();
    
        auto visited = new std::vector<bool>(this->graph_->size());
        for (std::size_t i = 0; i < this->graph_->size(); ++i) { visited->operator[](i) = false; }

        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            if (!visited->operator[](i)) {
                this->topological_sort_rec(i, visited, stack_);
            }
        }

        auto new_order = new std::vector<std::size_t>(this->graph_->size());
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            new_order->operator[](i) = stack_->top();
            stack_->pop();
        }

        delete stack_;
        delete visited;
        return new_order;
    }

    void depth_first_search(std::size_t start, std::vector<bool> *is_used) {
        is_used->operator[](start) = true;

        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            if (!is_used->operator[](i) && this->get_weight(start, i) != T(UNREACH)) {
                depth_first_search(i, is_used);
            }
        }
    }

    std::vector<std::vector<bool> *> *find_connected_components() {
        auto connected_components = new std::vector<std::vector<bool> *>();

        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            connected_components->push_back(new std::vector<bool>(this->graph_->size()));
            for (std::size_t j = 0; j < this->graph_->size(); ++j) connected_components->operator[](i)->operator[](j) = false;

                depth_first_search(i, connected_components->operator[](i));
        }

        return connected_components;
    }

    void print() {
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            for (std::size_t j = 0; j < this->graph_->size(); ++j) {
                std::cout << this->graph_->operator[](i)->operator[](j) << " ";
            }
            std::cout << std::endl;
        }
    }

    std::vector<T> *operator [](std::size_t idx) {
        return this->graph_->operator[](idx);
    }

public:
    graph(std::size_t elements_count, bool is_directed)
    : graph_(new std::vector<std::vector<T> *>(elements_count)), is_directed(is_directed) {
        for (std::size_t i = 0; i < elements_count; ++i) {
            this->graph_->operator[](i) = new std::vector<T>(elements_count);
            for (std::size_t j = 0; j < elements_count; ++j) {
                this->graph_->operator[](i)->operator[](j) = T(UNREACH);
            }
        }
    }

    graph(std::size_t elements_count, bool is_directed, const T& weight)
    : graph_(new std::vector<std::vector<T> *>(elements_count)), is_directed(is_directed) {
        for (std::size_t i = 0; i < elements_count; ++i) {
            this->graph_->operator[](i) = new std::vector<T>(elements_count);
            for (std::size_t j = 0; j < elements_count; ++j) {
                this->graph_->operator[](i)->operator[](j) = weight;
            }
        }
    }

    ~graph() {
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            delete this->graph_->operator[](i);
        }

        delete this->graph_;
    }

private:
    std::vector<std::vector<T> *> *graph_ = nullptr;
    bool is_directed = false;
};

#endif  // SRC_GRAPH_HPP_
