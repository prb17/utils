#pragma once
#include <string>
#include <stdexcept>
#include <sstream>

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class array {
                private:
                    T *data;
                    size_t size;
                    size_t capacity;
                    void resize();

                protected:
                    T* get_data();

                public:
                    //constructors
                    array();
                    array(size_t);
                    array(T*, size_t);
                    ~array();

                    ////modifiers
                    void insert(size_t, T);
                    void remove(size_t);
                    void add(T);
                    void clear();
                    
                    //accessors
                    bool is_valid_index(size_t idx) const;
                    bool is_empty() const;
                    int find(T value) const;
                    T get_value(size_t idx) const;
                    size_t get_size() const;
                    size_t get_capacity() const;
                    std::string to_string() const;
            };

            //constructors
            /**
             * @brief Construct a new default array object
             * 
             */
            template<typename T>
            array<T>::array() : array(0) {}


            /**
             * @brief Construct a new array object with certain size
             * 
             * @param size - size to start array off with
             */
            template<typename T>
            array<T>::array(size_t size) : array(nullptr, size) {}

            /**
             * @brief Construct a new array object starting with certain size and elements
             * 
             * @param in_data - data elements to start with
             * @param size  - size to start array with
             */
            template<typename T>
            array<T>::array(T* in_data, size_t size) : data{in_data}, size{size}, capacity{2*size} {
                if (size) {
                    this->data = (T *)malloc(sizeof(T) * capacity);
                    for(int i=0; i<size; i++) {
                        this->data[i] = in_data ? in_data[i] : 0;
                    }
                }
            }

            /**
             * @brief Destroy the array object
             *      make sure to free data and remove the pointer
             * 
             */
            template<typename T>
            array<T>::~array() {
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
                capacity = 2*(size + 1);
                T* tmp = (T *)malloc(sizeof(T) * capacity);
                if (data) {
                    for(int i=0; i<size; i++) {
                        tmp[i] = data[i];
                    }
                    free(data);
                }
                data = tmp;
            }

            /**
             * @brief Get the data pointer which points to this array
             * 
             * @return T* - pointer to this object
             */
            template<typename T>
            T* array<T>::get_data() {
                return data;
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
            void array<T>::insert(size_t index, T value) {
                if (size == capacity) {
                    resize();
                }

                while(index <= size) {
                    T tmp = data[index];
                    data[index++] = value;
                    value = tmp;
                }
                size++;
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
                //todo: implement removal
                size_t iterator = index;
                if (is_valid_index(index)) {
                    while(iterator < size - 1) {
                        data[iterator++] = data[iterator + 1];
                    }
                    size--;
                } //todo: what should I do if index is not valid?
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
            void array<T>::add(T value) {
                insert(size, value);
            }

            
            /**
             * @brief clears all elements of the array by setting the size to 0
             * 
             */
            template<typename T>
            void array<T>::clear() {
                size = 0;
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
            bool array<T>::is_valid_index(size_t idx) const {
                return idx < size;
            }
                    
            /**
             * @brief checks if the size of the array is zero
             * 
             * @return true - array is empty
             * @return false - array is not empty
             */
            template<typename T>
            bool array<T>::is_empty() const {
                return size == 0;
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
                while (index < size && data[index] != value) {
                    index++;
                }
                return index == size ? -1 : index;
            }
            
            /**
             * @brief Get the value at the provided index
             * @throw std::out_of_range exception
             * 
             * @param idx - index to get value at
             * @return T - the object at the index
             */
            template<typename T>
            T array<T>::get_value(size_t idx) const {
                if (!is_valid_index(idx)) {
                    throw std::out_of_range{"index is out of range"};
                }

                return data[idx];
            }

            /**
             * @brief Get the size array
             * 
             * @return size_t - number of elems in array
             */
            template<typename T>
            size_t array<T>::get_size() const {
                return size;
            }

            /**
             * @brief Get the capacity of the array
             * 
             * @return size_t - The total space(memory) the array takes up, messured in sizeof(T) chunks
             */
            template<typename T>
            size_t array<T>::get_capacity() const {
                return capacity;
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
                for(int i=0; i<size; i++) {
                    stream << data[i] << " ";
                }
                stream << "]";
                return stream.str();
            }
            

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const array<T>& arr) {
                return stream << arr.to_string();
            }
        }
    }    
}
