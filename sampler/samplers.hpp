//
// Created by Long Gong on 3/14/17.
//

#ifndef CLION_SELECTOR_H
#define CLION_SELECTOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <cassert>


//#include "../static_tree/static_tree.hpp"

// we only have three colors, so no need for ints!
enum class SupportOperator : char { ADD = 1, SUBTRACTION = 2, ASSIGNMENT = 3 };


/*!
 * @brief base class for sampler
 * @tparam WType
 */
template <class WType>
class SamplerBase {
public:
    virtual int sample(WType test_value=std::numeric_limits<WType>::lowest()) = 0;
    virtual void update(SupportOperator op, std::size_t index, WType value) = 0;
    virtual WType get(std::size_t index) const = 0;
};

/*!
 * @brief class for wrapping element in Samplers
 * @tparam WType
 */
template <typename WType>
class element_wrapper {
private:
    std::size_t index;
    SamplerBase<WType> *ls;
public:
    typedef element_wrapper self_type;
    element_wrapper(): index(-1), ls(nullptr) { }
    element_wrapper(std::size_t index, SamplerBase<WType> *ls): index(index),
                                                                  ls(ls) { }
    /**
     * @brief  set the value of index and sampler
     * @param index
     * @param ls
     */
    void set(std::size_t index, SamplerBase<WType> *ls) {
        this->index = index;
        this->ls = ls;
    }

    self_type& operator++() {
        ls->update(SupportOperator::ADD, index, 1);
        return this;
    }

    WType operator++(int) {
        WType temp = ls->get(index);
        ls->update(SupportOperator::ADD, index, 1);
        return temp;
    }

    self_type&operator--() {
        ls->update(SupportOperator::SUBTRACTION, index, 1);
        return this;
    }

    WType operator--(int) {
        WType temp = ls->get(index);
        ls->update(SupportOperator::SUBTRACTION, index, 1);
        return temp;
    }

    self_type& operator+=(WType value) {
        ls->update(SupportOperator::ADD, index, value);
        return this;
    }

    self_type& operator-=(WType value) {
        ls->update(SupportOperator::SUBTRACTION, index, value);
        return this;
    }

    self_type& operator=(WType value) {
        if (value != ls->get(index)) {
            ls->update(SupportOperator::ASSIGNMENT, index, value);
        }
        return *this;
    }
//    operator int() const {
//        return int(ls->get(index));
//    }
//    operator double() const {
//        return double(ls->get(index));
//    }
//    operator float() const {
//        return float(ls->get(index));
//    }

    operator WType() const {
        return ls->get(index);
    }

    friend std::ostream& operator<<(std::ostream& os, const self_type& obj)
    {
        // write obj to stream
        os << obj.ls->get(obj.index);
        return os;
    }
    friend std::istream& operator>>(std::istream& is, self_type& obj)
    {
        // read obj from stream
        if(obj.ls == nullptr)
            is.setstate(std::ios::failbit);
        is >> obj.ls->get(obj.index);
        return is;
    }

//    friend WType operator+(const self_type& obj, WType x) {
//
//        return obj.ls->get(obj.index) + x;
//    }
//
//    friend WType operator-(const self_type& obj, WType x) {
//        return obj.ls->get(obj.index) - x;
//    }
//
//    friend WType operator+(WType x, const self_type& obj) {
//        return obj.ls->get(obj.index) + x;
//    }
//
//    friend WType operator-(WType x, const self_type& obj) {
//        return x - obj.ls->get(obj.index);
//    }
//
//    friend WType operator*(WType x, const self_type& obj) {
//        return x * obj.ls->get(obj.index);
//    }
//
//    friend WType operator/(const self_type& obj, WType x) {
//        return obj.ls->get(obj.index) / x;
//    }
//
//    friend WType operator*(const self_type& obj, WType x) {
//        return x * obj.ls->get(obj.index);
//    }
//
//    friend WType operator/(WType x, const self_type& obj) {
//        return x / obj.ls->get(obj.index);
//    }
//
//    friend bool operator==(WType x, const self_type& obj) {
//        return x == obj.ls->get(obj.index);
//    }
//
//    friend bool operator==(const self_type& obj, WType x) {
//        return x == obj.ls->get(obj.index);
//    }
//
//    friend bool operator>=(WType x, const self_type& obj) {
//        return x >= obj.ls->get(obj.index);
//    }
//
//    friend bool operator>=(const self_type& obj, WType x) {
//        return obj.ls->get(obj.index) >= x;
//    }
//
//    friend bool operator<=(WType x, const self_type& obj) {
//        return x <= obj.ls->get(obj.index);
//    }
//
//    friend bool operator<=(const self_type& obj, WType x) {
//        return obj.ls->get(obj.index) <= x;
//    }
//
//    friend bool operator>(const self_type& obj, WType x) {
//        return obj.ls->get(obj.index) > x;
//    }
//
//    friend bool operator<(WType x, const self_type& obj) {
//        return x < obj.ls->get(obj.index);
//    }
//
//    friend bool operator<(const self_type& obj, WType x) {
//        return obj.ls->get(obj.index) < x;
//    }


};

/*!
 * @brief class for linear-time sampler
 * @tparam WType
 */
template <class WType>
class LinearSampler: public SamplerBase<WType> {
private:
    // sum of all elements
    std::vector<WType> weights;
    WType sum;
    int search(double target);
public:
    element_wrapper<WType> ew;
    LinearSampler(std::size_t n): weights(n, 0), ew(-1, nullptr) {
        sum = 0;
    }
    LinearSampler(std::vector<WType>&& weights): weights(weights), ew(-1, nullptr){
        // non-negative check
        assert(std::all_of(weights.begin(), weights.end(), [](WType x){return x >= 0;}));
        // get sum
        sum = std::accumulate(weights.begin(), weights.end(), 0);
    }
    LinearSampler(const std::vector<WType>& weights): weights(weights), ew(-1, nullptr){
        // non-negative check
        assert(std::all_of(weights.begin(), weights.end(), [](WType x){return x >= 0;}));
        // get sum
        sum = std::accumulate(weights.begin(), weights.end(), 0);
    }

    std::size_t size() {
        return weights.size();
    }

    element_wrapper<WType>& operator[](std::size_t index) {
        assert(index >= 0 && index < size());
        ew.set(index, this);
        return ew;
    };

    const WType& operator[](std::size_t index) const {
        assert(index >= 0 && index < size());
        return weights[index];
    }


    void insert(WType value) {
        weights.push_back(value);
        sum += value;
    }

    bool remove() {
        // TODO
    }

    virtual int sample(WType test_value=std::numeric_limits<WType>::lowest());

    virtual void update(SupportOperator op, std::size_t index, WType value) {
        assert(index >= 0 && index < weights.size());
        switch(op) {
            case SupportOperator::ADD :
            {
                weights[index] += value;
                sum += value;
                break;
            }
            case SupportOperator::SUBTRACTION :
            {
                weights[index] -= value;
                sum -= value;
                break;
            }
            case SupportOperator::ASSIGNMENT :
            {
                WType temp = value - weights[index];
                weights[index] = value;
                sum += temp;
                break;
            }
            default:
            {
                std::cerr << "Unsupported operator! " << std::endl;
            }
        }
        assert(weights[index] >= 0);
    }


    virtual WType get(std::size_t index) const {
        assert(index >= 0 && index < weights.size());
        return weights[index];
    }
};


template  <class WType>
int LinearSampler<WType>::sample(WType test_value) {
//    std::cout << "sample func is called" << std::endl;
    if (test_value == std::numeric_limits<WType>::lowest()){// not test
        // generate a uniform random variable
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(0.0, this->sum);
        return search(distribution(generator));
    }
    return search(test_value);
}


/**
 * @brief Search for target value
 *
 * @tparam WType {int, float, double}
 * @param target The target value.
 * @return The index \f$k=\min\{j:\sum_{i=0}^{j}weights[i] > target\}\f$
 */
template <typename WType>
int LinearSampler<WType>::search(double target) {
    WType current = 0;

    for (std::size_t i = 0;i < this->weights.size();++ i) {
        current += this->weights[i];
        if (current > target) return i;
    }
    return 0;// for invalid target
}



/*!
@brief class for logarithmically sampling an element from an array
*/
template <typename WType>
class LogarithmSampler: public SamplerBase<WType> {
private:
    std::size_t size;
    std::size_t capacity;
    // a complete binary tree (stored in a std::vector)
    static_tree<WType> st;

    void init(std::size_t index, const std::vector<WType>& weights);
    std::size_t search(WType value);
    void update_(std::size_t index, WType value, bool decrement=false);
public:
    element_wrapper<WType> ew;
    LogarithmSampler(std::size_t n): size(n),
                                     capacity(1 << (int(std::ceil(std::log2(n))))),
                                     static_tree<WType>(capacity),
                                     ew()

    {

    }

    LogarithmSampler(const std::vector<WType>& weights): size(weights.size()),
                                                         capacity(1 << (int(std::ceil(std::log2(n))))),
                                                         static_tree<WType>(capacity),
                                                         ew()
    {
        assert(std::all_of(weights.begin(), weights.end(), [](WType x){return x >= 0;}));
        init(0, weights);
    }

//    LogarithmSampler(std::vector<WType>&& weights): SamplerBase<WType>(weights),
//                                                    size(weights.size()),
//                                                    capacity(1 << (int(std::ceil(std::log2(n))))),
//                                                    static_tree<WType>(capacity),
//                                                    ew()
//    {
//        init(0);
//    }

    element_wrapper& operator[](std::size_t index) {
        // subscript
        assert(index >= 0 && index < weights.size());
        ew.set(index, this);
        return ew;
    }

    WType operator[](std::size_t index) const {
        // const subscript
        assert(index >= 0 && index < weights.size());
        return weights[index];
    }

    virtual std::size_t sample(WType test_value=std::numeric_limits<WType>::lowest()) {
        if (test_value == std::numeric_limits<WType>::lowest()){// not test
            // generate a uniform random variable
            std::default_random_engine generator;
            std::uniform_real_distribution<double> distribution(0.0, this->sum);
            return search(distribution(generator));
        }
        return search(test_value);
    }

    bool insert(WType value) {
        if (size == capacity) {
            std::cout << "Insertion failed because of no available space."
                    "However, we will include resize feature in the future version."
                      << std::endl;
            return false;
        }

        weights.push_back(value);
        ++ size;
        (*this)[size - 1] += value;
        return true;
    }

    bool remove() {
        // TODO
    }

    virtual void update(SupportOperator op, std::size_t index, WType value) {
        assert(index >= 0 && index < size);
        switch(op) {
            case SupportOperator::ADD :
            {
                assert(get(index) + value >= 0);
                update_(index, value, false);
                break;
            }
            case SupportOperator::SUBTRACTION :
            {
                assert(get(index) - value >= 0);
                update_(index, value, true);
                break;
            }
            case SupportOperator::ASSIGNMENT :
            {
                assert(value >= 0);
                // Here, the use of if-else is for handling
                // the non-negative issue of unsigned type
                // (maybe it is not necessary)
                if (value > get(index)) {
                    update_(index, value - get(index), false);
                }
                else {
                    update_(index, get(index) - value, true);
                }
                break;
            }
            default:
            {
                std::cerr << "Unsupported operator" << std::endl;
            }
        }
    }

    virtual WType get(std::size_t index) const {
        assert(index >= 0 && index < size);
        return st[st.leaf(index)];
    }


};


template <typename WType>
void LogarithmSampler<WType>::init(std::size_t index, const std::vector<WType>& weights) {
    if (!this->st.is_leaf(index)) {
        this->init(this->st.leaf_child(index), weights);
        this->init(this->st.right_child(index), weights);
        // Here, we did not check out of boundary
        // because we are sure it would not (st is a complete binary tree)
        this->st[index] = this->st[this->st.leaf_child(index)] + this->st[this->st.right_child(index)];
    }
    else {
        this->st[index] = this->weights[this->st.leaf_offset(index)];
    }
}


template <typename WType>
void LogarithmSampler<WType>::update_(std::size_t index, WType value, bool decrement) {

    try{
        std::size_t tree_index = st.leaf(index);
        do {
            if (decrement)
                st[tree_index] -= value;
            else
                st[tree_index] += value;
            tree_index = st.parent(tree_index);
        } while (tree_index != 0);
    }
    catch(...) {
        std::cerr << "Weight updating failed, program will terminate" << std::endl;
        exit(1);
    }

}


template <typename  WType>
std::size_t LogarithmSampler<WType>::search(WType value) {
    if (value >= this->st[this->st.root()]) {
        std::cerr << "[WARNING] value exceeds the boundary, just returning the last element" << std::endl;
        return (size - 1);
    }
    std::size_t current_pos = 0;
    std::size_t temp = 0;
    while (! this->st.is_leaf(current_pos)) {
        if (value < this->st[this->st.left_child(current_pos)]) {
            current_pos = this->st.left_child(current_pos);
        } else {
            temp = this->st.left_child(current_pos);
            current_pos = this->st.right_child(current_pos);
            value -= this->st[temp];
        }
    }

    return this->st.left_offset(current_pos);
}

#endif //CLION_SELECTOR_H
