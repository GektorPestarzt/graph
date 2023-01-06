#ifndef SRC_GRAPH_HPP_
#define SRC_GRAPH_HPP_

#include <iostream>
#include <vector>
#include <stack>

#define UNTOUCHED 99999

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
        this->graph_->operator[](first_element)->operator[](second_element) = T(UNTOUCHED);
        if (!this->is_directed) {
            this->graph_->operator[](second_element)->operator[](first_element) = T(UNTOUCHED);
        }
    }

    void add_vertex() {
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            this->graph_->operator[](i)->push_back(T(UNTOUCHED));
        }
        this->graph_->push_back(new std::vector<T>(this->graph_->size() + 1, T(UNTOUCHED)));
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
        auto path = new std::vector<T>(this->graph_->size());
        auto visited = new std::vector<bool>(this->graph_->size());
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            path->operator[](i) = T(UNTOUCHED);
            visited->operator[](i) = false;
        }
    
        path->operator[](start_element) = T(0);
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            std::size_t point = UNTOUCHED;

            for (std::size_t j = 0; j < this->graph_->size(); ++j) {
                if (!visited->operator[](j) && (point == UNTOUCHED || path->operator[](j) < path->operator[](point))) {
                    point = j;
                }
            }

            if (path->operator[](point) == UNTOUCHED) break;
            visited->operator[](point) = true;

            for (std::size_t j = 0; j < this->graph_->size(); ++j) {
                if (this->graph_->operator[](point)->operator[](j) == T(UNTOUCHED)) continue;

                T length = this->graph_->operator[](point)->operator[](j);
                if (path->operator[](point) + length < path->operator[](j)) {
                    path->operator[](j) = path->operator[](point) + length;
                }
            }
        }

        delete visited;
        return path;
    }

    void topological_sort_rec(std::size_t point, std::vector<bool> *visited, std::stack<std::size_t> *stack) {
        visited->operator[](point) = true;
    
        for (std::size_t i = 0; i < this->graph_->size(); ++i) {
            if (!visited->operator[](i) && this->get_weight(point, i) != T(UNTOUCHED)) {
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
            if (!is_used->operator[](i) && this->get_weight(start, i) != T(UNTOUCHED)) {
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

public:
    graph(std::size_t elements_count, bool is_directed)
    : graph_(new std::vector<std::vector<T> *>(elements_count)), is_directed(is_directed) {
        for (std::size_t i = 0; i < elements_count; ++i) {
            this->graph_->operator[](i) = new std::vector<T>(elements_count);
            for (std::size_t j = 0; j < elements_count; ++j) {
                this->graph_->operator[](i)->operator[](j) = T(UNTOUCHED);
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
