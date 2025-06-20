#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <cstring>
#include <iostream>

#include "container.hh"
#include "exception.hh"

namespace prb17 {
    namespace utils {
        namespace structures {
            template<typename T>
            class array {
                private:
                    size_t cap;
                    void resize();

                protected:
                    container<T> *data;
                    size_t sz;

                public:
                    //constructors
                    array();
                    array(size_t);
                    array(const array<T> &);
                    ~array();

                    ////modifiers
                    virtual bool insert(size_t, T);
                    virtual void remove(size_t);
                    virtual bool add(T);
                    void clear();
                    
                    //accessors
                    bool empty() const;
                    virtual int find(T) const;
                    virtual T& get(size_t);
                    virtual const T& get(size_t) const;
                    virtual size_t size() const;
                    virtual size_t capacity() const;
                    virtual bool valid(size_t) const;
                    virtual std::string to_string() const;

                    //overloaded operators
                    array& operator=(array &&); //move assignment operator
                    array& operator=(const array &); //copy assignment operator
                    bool operator==(const array&) const;
                    bool operator!=(const array&) const;
                    T& operator[](size_t);
                    const T& operator[](size_t) const;
            };

            //constructors
            /**
             * @brief Construct a new default array object
             * 
             */
            template<typename T>
            array<T>::array() : array<T>(0) {}

            //TODO: I think the memset might screw things up when I have nested arrays, i.e. array< array<T> >
            /**
             * @brief Construct a new array object starting with certain size and elements
             * 
             * @param cap  - capacity to start array with
             */
            template<typename T>
            array<T>::array(size_t cap) : sz{0}, cap{cap} {                
                this->data = (container<T> *)malloc(sizeof(container<T>*) * cap);                
                memset(this->data, 0, sizeof(container<T>*) * cap);
            }

            /**
             * @brief Construct a new array<T>::array object
             * 
             * @tparam T 
             * @param a - array to copy from
             */
            template<typename T>
            array<T>::array(const array<T> &a) {
                cap = a.cap;
                sz = a.sz;
                data = nullptr;
                data = (container<T> *)malloc(sizeof(container<T>*) * cap);
                memset(data, 0, sizeof(container<T>*) * cap);

                for(size_t i=0; i<size(); i++) {
                    data[i] = a.data[i];
                }
            }

            /**
             * @brief Destroy the array object
             *      make sure to free data and remove the pointer
             * 
             */
            template<typename T>
            array<T>::~array() {
                for (int i=0; i < capacity(); i++) {
                    data[i].~container<T>();
                }
                free(data);
                data = nullptr;           
            }


            //modifiers
            /**
             * @brief resizes the array when it gets full
             * 
             * leverages amortized time complexity. It's not often we have to resize the array so
             *      take a hit on time ocassionally for better performance later
             * @time_complexity: O(n) we need to iterate over all elements in the original data
             * @space_complexity: O(n) we need to make a whole other array essentially
             * 
             */
            template<typename T>
            void array<T>::resize() {
                cap = 2*(size() + 1);
                container<T>* tmp = (container<T> *)malloc(sizeof(container<T>*) * cap);
                memset(tmp, 0, sizeof(container<T>*) * cap);
                
                for(int i=0; i<size(); i++) {
                    tmp[i] = data[i];
                    data[i].~container<T>();
                }
                if (data) {
                    free(data);
                }
                data = tmp;
            }

            /**
             * @brief inserts value at the given index,
             *      shifting all elements starting, from index to size (inclusive) i.e. [index, size], 
             *      to the left 1.
             * 
             * @time_complexity - worst case: O(n) but depends on the insertion index
             *      an index near size will be closer to O(1) time than O(n) time
             * @space_complexity - worst case: O(n) (when we need to resize the array)
             *      otherwise it's O(1)
             * @golden_question - TODO: can it be better? - tbd
             * 
             * @param index - where value will be inserted
             * @param value - the value to insert
             */
            template<typename T>
            bool array<T>::insert(size_t index, T value) {
                if (size() == capacity()) {
                    resize();
                }

                while(index <= size()) {
                    T tmp = valid(index) ? get(index) : T{};
                    data[index++] = value;
                    value = tmp;
                }
                sz++;

                //todo: could there be a reason this wouldn't be true??
                return true;
            }

            /**
             * @brief removes the value at the given index,
             *      shifting all elements starting, from index to size (inclusive) i.e. [index, size], 
             *      to the left 1.
             * 
             * @time_complexity - worst case: O(n) but depends on the insertion index
             *      an index near size will be closer to O(1) time than O(n) time
             * @space_complexity - worst case: O(n) (when we need to resize the array)
             *      otherwise it's O(1)
             * @golden_question - TODO: can it be better? - tbd
             * 
             * @param index - where value will be removed from
             */
            template<typename T>
            void array<T>::remove(size_t index) {
                while(index < size() - 1) {
                    data[index++] = data[index + 1];
                }
                sz--;
            }

            /**
             * @brief adds value at the end of array
             * 
             * @time_complexity - worst case: O(n) when the array needs to be resized 
             *      but otherwise always O(1)
             * @space_complexity - worst case: O(n) (when we need to resize the array)
             *      otherwise it's O(1)
             * @golden_question - TODO: can it be better? - tbd
             * 
             * @param value - the value to add
             */
            template<typename T>
            bool array<T>::add(T value) {
                return insert(size(), value);
            }

            
            /**
             * @brief clears all elements of the array by setting the size to 0
             * 
             */
            template<typename T>
            void array<T>::clear() {
                sz = 0;
            }

            //accessors
            /**
             * @brief checks to see if the index requested is within the bounds of this array
             * 
             * @param idx - index to check 
             * @return true  - when index is Less than the total size of array
             * @return false - when index is greater than or equal to total size of array (out of range)
             */
            template<typename T>
            bool array<T>::valid(size_t idx) const {
                return idx < size();
            }
                    
            /**
             * @brief checks if the size of the array is zero
             * 
             * @return true - array is empty
             * @return false - array is not empty
             */
            template<typename T>
            bool array<T>::empty() const {
                return size() == 0;
            }            

            /**
             * @brief finds the povide value in the array
             * 
             * @param value - value to find
             * @return int - index the value was found, -1 if not found
             */
            template<typename T>
            int array<T>::find(T value) const {
                int index = 0;
                while (index < size() && data[index] != value) {
                    index++;
                }
                return index == size() ? -1 : index;
            }
             
            /**
             * @brief Get the value at the provided index
             * @throw std::out_of_range exception
             * 
             * @param idx - index to get value at
             * @return T - the object at the index
             */
            template<typename T>
            T& array<T>::get(size_t idx) {
                if (!valid(idx)) {
                    throw utils::exception("Index out of range");
                }

                return data[idx].value();
            }


            /**
             * @brief Get the value at the provided index
             * @throw std::out_of_range exception
             * 
             * @param idx - index to get value at
             * @return T - the object at the index
             */
            template<typename T>
            const T& array<T>::get(size_t idx) const {
                if (!valid(idx)) {
                    throw utils::exception("Index out of range");
                }

                return data[idx].value();
            }

            /**
             * @brief Get the size array
             * 
             * @return size_t - number of elems in array
             */
            template<typename T>
            size_t array<T>::size() const {
                return sz;
            }

            /**
             * @brief Get the capacity of the array
             * 
             * @return size_t - The total space(memory) the array takes up, messured in sizeof(T) chunks
             */
            template<typename T>
            size_t array<T>::capacity() const {
                return cap;
            }            

            /**
             * @brief return a string representation of the array
             * 
             * @return std::string - string representation of array
             */
            template<typename T>
            std::string array<T>::to_string() const {
                std::stringstream stream;
                stream << "[ ";
                for(int i=0; i<size(); i++) {
                    stream << data[i] << " ";
                }
                stream << "]";
                return stream.str();
            }

            //operator<< (ostream)
            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const array<T>& arr) {
                return stream << arr.to_string();
            }

            //operator = move assignment
            template<typename T>
            array<T>& array<T>::operator=(array &&a) {
                if (&a != this) {
                    // 1. Clean up current resources
                    if (data) {
                        for (int i=0; i < capacity(); i++) {
                            data[i].~container<T>();
                        }
                        free(data);
                    }

                    // 2. "Steal" resources from 'other'
                    data = a.data;
                    sz = a.sz;
                    cap = a.cap;

                    // 3. Leave 'other' in a valid, empty state
                    a.data = nullptr;
                    a.sz = 0;
                    a.cap = 0;
                }
                return *this;
            }
            
            //operator = copy assignment
            template<typename T>
            array<T>& array<T>::operator=(const array &a) {
                if (&a != this) {
                    if (data) {
                        for (int i=0; i < capacity(); i++) {
                            data[i].~container<T>();
                        }
                        free(data);
                    }
                    cap = a.cap;
                    sz = a.sz;
                    data = (container<T> *)malloc(sizeof(container<T>*) * cap);                    
                    memset(data, 0, sizeof(container<T>*) * cap);
                    for(int i=0; i<size(); i++) {
                        data[i] = a.data[i];
                    }
                }
                return *this;
            }

            //operator []
            template<typename T>
            T& array<T>::operator[] (size_t idx) {
                return get(idx);
            }
            template<typename T>
            const T& array<T>::operator[] (size_t idx) const {
                return get(idx); 
            }

            //operator ==
            template<typename T>
            bool array<T>::operator==(const array<T> &a) const {
                if (this->size() != a.size()) {
                    return false;
                }

                int idx = 0;
                bool equal = true;
                while(idx < this->size() && equal) {
                    equal = this->get(idx) == a.get(idx);
                    idx++;
                }
                return equal;
            }

            //operator !=
            template<typename T>
            bool array<T>::operator!=(const array<T> &a) const {
                return !(*this == a);
            }
        }
    }    
}
