//
// Created by longgong on 3/24/17.
//

#ifndef CLION_STATIC_TREE_HPP
#define CLION_STATIC_TREE_HPP

#include <array>
#include <vector>
#include <cassert>

/*!
@brief namespace for simple data structures
@see https://github.com/lgong30
@since version 1.0.0
*/
namespace sds {

    template<typename Type>
    class static_tree {
    protected:
        std::vector<Type> nodes;
        std::size_t first_leaf_index;
    public:
        class level_iterator
        {
            public:
                typedef level_iterator self_type;
                typedef Type value_type;
                typedef Type& reference;
                typedef typename std::vector<Type>::iterator pointer;
                typedef std::forward_iterator_tag iterator_category;
                typedef int difference_type;
                level_iterator(pointer ptr): ptr_(ptr) { }
                self_type operator++() { ptr_ ++; return *this; }
                reference operator*() { return *ptr_; }
                pointer operator->() { return ptr_; }
                bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
                bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
            private:
                pointer ptr_;
        };

        static_tree(std::size_t n): nodes(n, 0){
            first_leaf_index = n / 2 + 1;
        }
        static_tree(const std::vector<Type>& data): nodes(data){
            first_leaf_index = nodes.size() / 2 + 1;
        }
        static_tree(std::vector<Type>&& data): nodes(data){
            first_leaf_index = nodes.size() / 2 + 1;
        }

        std::size_t size(){
            return nodes.size();
        }

        std::size_t root(){
            return 0;
        }

        std::size_t left_child(std::size_t current_pos) {
            return 2 * current_pos + 1;
        }

        std::size_t right_child(std::size_t current_pos) {
            return 2 * current_pos + 2;
        }
        std::size_t parent(std::size_t current_pos) {
            return current_pos / 2;
        }

        Type get(std::size_t index) const {
            assert(index < size() && index >= 0);
            return nodes[index];
        }

        level_iterator begin() { return level_iterator(nodes.begin()); }
        level_iterator end() { return level_iterator(nodes.end()); }

        bool is_leaf(std::size_t index) {
            return left_child(index) >= size();
        }

        std::size_t first_leaf() {
            return first_leaf_index;
        }

        std::size_t leaf_offset(std::size_t index) {
            assert(is_leaf(index));
            return index - first_leaf_index;
        }
        Type& operator[](std::size_t index) {
            assert(index >= 0 && index < size());
            return nodes[index];
        }
        Type operator[](std::size_t index) const {
            assert(index >= 0 && index < size());
            return nodes[index];
        }

    };
}
#endif //CLION_STATIC_TREE_HPP
