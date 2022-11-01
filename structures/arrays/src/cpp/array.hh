#pragma once
#include <string>

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class array {
                private:
                    T *data;
                    size_t size;
                    size_t capacity;

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
                    void resize() {
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

                protected:
                    /**
                     * @brief Get the data pointer which points to this array
                     * 
                     * @return T* - pointer to this object
                     */
                    T* get_data() {
                        return data;
                    }

                public:
                    //constructors
                    /**
                     * @brief Construct a new default array object
                     * 
                     */
                    array() : array(0) {}

                    /**
                     * @brief Construct a new array object with certain size
                     * 
                     * @param size - size to start array off with
                     */
                    array(size_t size) : array(nullptr, size) {}

                    /**
                     * @brief Construct a new array object starting with certain size and elements
                     * 
                     * @param in_data - data elements to start with
                     * @param size  - size to start array with
                     */
                    array(T* in_data, size_t size) : data{in_data}, size{size}, capacity{2*size} {
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
                    ~array() {
                        free(data);
                        data = nullptr;
                    }

                    ////modifiers

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
                    void insert(size_t index, T value) {
                        if (size == capacity) {
                            resize();
                        }

                        while(index <= size) {
                            int tmp = data[index];
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
                    void remove(size_t index) {
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
                    void add(T value) {
                        insert(size, value);
                    }

                    /**
                     * @brief clears all elements of the array by setting the size to 0
                     * 
                     */
                    void clear() {
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
                    bool is_valid_index(size_t idx) {
                        return idx < size;
                    }

                    /**
                     * @brief checks if the size of the array is zero
                     * 
                     * @return true - array is empty
                     * @return false - array is not empty
                     */
                    bool is_empty() {
                        return size == 0;
                    }

                    /**
                     * @brief finds the povide value in the array
                     * 
                     * @param value - value to find
                     * @return int - index the value was found, -1 if not found
                     */
                    int find(T value) {
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
                    T get_value(size_t idx) {
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
                    size_t get_size() {
                        return size;
                    }

                    /**
                     * @brief Get the capacity of the array
                     * 
                     * @return size_t - The total space(memory) the array takes up, messured in sizeof(T) chunks
                     */
                    size_t get_capacity() {
                        return capacity;
                    }

                    /**
                     * @brief return a string representation of the array
                     * 
                     * @return std::string - string representation of array
                     */
                    std::string to_string() {
                        std::string data_str = "[ ";
                        for(int i=0; i<size; i++) {
                            data_str += std::to_string(data[i]) + " ";
                        }
                        data_str += "]";
                        return data_str;
                    }
            };
        }
    }    
}
