#pragma once
#include "array.hh"
#include "pair.hh"

#include <functional>
#include <iostream>
#include <initializer_list>

namespace prb17 {
    namespace utils {
        namespace structures {
            namespace detail {
                template <typename T>
                struct is_streamable {
                private:
                    template <typename U>
                    static auto test(int) -> decltype(std::declval<std::stringstream>() << std::declval<const U&>(), std::true_type());
                    template <typename U>
                    static auto test(...) -> std::false_type;

                public:
                    static constexpr bool value = decltype(test<T>(0))::value;
                };

                inline size_t hash_string_val(const std::string& s) {
                    size_t hash_val = 0;
                    for (char c : s) {
                        hash_val = hash_val * 31 + static_cast<size_t>(c);
                    }
                    return hash_val;
                }
            } // namespace detail

            template <typename K, typename V>
            class map {
                static_assert(!std::is_pointer<K>::value,
                              "Error: prb17::utils::structures::map's default hash function is not suitable for pointer types as keys. "
                              "Provide a custom hash function for pointer keys if you intend to hash the *value* pointed to, "
                              "or use a different hash map if you intend to hash pointer *addresses* deterministically.");

                private:
                    array<array<pair<K,V>> > buckets;
                    size_t sz;
                    std::function<size_t(K)> hash_function;
                    size_t get_bucket_index(const K& key) const {
                        return hash_function(key) % buckets.size();
                    } 

                public:
/*
                    map(std::initializer_list<prb17::utils::structures::pair<K,V>> init_list) {
                        for (const auto& p : init_list) {
                            add(p.key(), p.value()); // Use your existing add method
                        }
                    }
*/

                    explicit map(std::function<size_t(const K&)> hash_func = [](const K& key) {
                                if constexpr (detail::is_streamable<K>::value) {
                                    std::stringstream ss;
                                    ss << key;
                                    return detail::hash_string_val(ss.str());
                                } else {
                                    throw prb17::utils::exception("Key type is not streamable (does not have operator<< for std::ostream) "
                                                             "and therefore cannot use prb17::utils::structures::map's default hash function. "
                                                             "Please implement operator<< for your Key type or provide a custom hash function.");
                                }
                            }) : buckets{}, sz{0}, hash_function{hash_func} {
                        //instantiate all the buckets
                        for (int i=0; i<10; i++) {
                            buckets.add(array<pair<K,V>>());
                        }
                    }

                    map(const map<K,V> &m) {
                        buckets = m.buckets;
                        sz = m.sz;
                        hash_function = m.hash_function;
                    }

                    map& operator=(map &&);
                    map& operator=(const map &);

                    V& operator[](K&);
                    const V& operator[](K&) const;

                    void add(const K& key, const V& value);
                    void add(const map<K,V> &m);
                    V& get(const K& key);
                    const V& get(const K& key) const;
                    void remove(const K& key);

                    size_t size() const;
                    size_t capacity() const;
                    bool is_empty() const;
                    bool at_capacity() const;

                    std::string to_string() const; 
                    // --- HASH MAP ITERATOR IMPLEMENTATION ---

                    // The type the iterator yields (pair with const Key)
                    using value_type_pair = std::pair<const K, V>;

                    class iterator {
                        public:
                            using iterator_category = std::forward_iterator_tag; // Or bidirectional if you implement --
                            using value_type = value_type_pair;
                            using difference_type = std::ptrdiff_t;
                            using pointer = value_type_pair*;
                            using reference = value_type_pair&;

                        private:
                            // Pointers to the hash_map's internal data
                            prb17::utils::structures::array<prb17::utils::structures::array<std::pair<K,V>>>* outer_array_ptr; // Pointer to the whole 'buckets' array
                            size_t current_bucket_idx; // Index of the current bucket being examined
                            typename prb17::utils::structures::array<std::pair<K,V>>::iterator current_inner_array_it; // Iterator into the current inner array

                        public:
                            // Constructor for the iterator
                            iterator(
                                prb17::utils::structures::array<prb17::utils::structures::array<std::pair<K,V>>>* outer_ptr,
                                size_t bucket_idx,
                                typename prb17::utils::structures::array<std::pair<K,V>>::iterator inner_it
                            ) : outer_array_ptr(outer_ptr),
                                current_bucket_idx(bucket_idx),
                                current_inner_array_it(inner_it)
                            {}

                            reference operator*() const {
                                return *current_inner_array_it; // This returns pair<K,V>&, which is implicitly convertible to pair<const K,V>&
                            }

                            pointer operator->() const {
                                return &(operator*());
                            }

                            iterator& operator++() {
                                ++current_inner_array_it;

                                if (current_inner_array_it == outer_array_ptr->get(current_bucket_idx).end()) {
                                    current_bucket_idx++; // Move to the next bucket

                                    while (current_bucket_idx < outer_array_ptr->size() &&
                                           outer_array_ptr->get(current_bucket_idx).empty()) {
                                        current_bucket_idx++;
                                    }

                                    if (current_bucket_idx < outer_array_ptr->size()) {
                                        current_inner_array_it = outer_array_ptr->get(current_bucket_idx).begin();
                                    } else {
                                        // Reached the end of all buckets
                                        // Set to a special "end" state (e.g., current_inner_array_it = default constructed)
                                        // You might need a more robust way to represent "end" for outer array iterators
                                        // For now, let's say the current_bucket_idx reaching outer_array_ptr->size() signals end.
                                        // This makes the comparison operator correctly handle the end state.
                                    }
                                }
                                return *this;
                            }

                            iterator operator++(int) {
                                iterator temp = *this;
                                ++(*this);
                                return temp;
                            }

                            // Comparison operators
                            bool operator==(const iterator& other) const {
                                // Need to handle the end() case carefully.
                                // If both are at the end, they are equal.
                                // Otherwise, compare bucket index and inner iterator.
                                bool this_is_end = current_bucket_idx >= outer_array_ptr->size();
                                bool other_is_end = other.current_bucket_idx >= other.outer_array_ptr->size();

                                if (this_is_end && other_is_end) {
                                    return true;
                                }
                                if (this_is_end != other_is_end) {
                                    return false;
                                }
                                // If neither is end, compare their actual positions
                                return current_bucket_idx == other.current_bucket_idx &&
                                       current_inner_array_it == other.current_inner_array_it;
                            }

                            bool operator!=(const iterator& other) const { return !(*this == other); }
                    };

                    // Const iterator (similar, but for const access)
                    class const_iterator {
                        public:
                            using iterator_category = std::forward_iterator_tag; // Or bidirectional
                            using value_type = value_type_pair;
                            using difference_type = std::ptrdiff_t;
                            using pointer = const value_type_pair*;
                            using reference = const value_type_pair&;

                        private:
                            const prb17::utils::structures::array<prb17::utils::structures::array<std::pair<K,V>>>* outer_array_ptr;
                            size_t current_bucket_idx;
                            typename prb17::utils::structures::array<std::pair<K,V>>::const_iterator current_inner_array_it; // Use const_iterator for inner

                        public:
                            const_iterator(
                                const prb17::utils::structures::array<prb17::utils::structures::array<std::pair<K,V>>>* outer_ptr,
                                size_t bucket_idx,
                                typename prb17::utils::structures::array<std::pair<K,V>>::const_iterator inner_it
                            ) : outer_array_ptr(outer_ptr),
                                current_bucket_idx(bucket_idx),
                                current_inner_array_it(inner_it)
                            {}

                            // Conversion from non-const iterator
                            const_iterator(const iterator& other)
                                : outer_array_ptr(other.outer_array_ptr),
                                  current_bucket_idx(other.current_bucket_idx),
                                  current_inner_array_it(other.current_inner_array_it) // Requires conversion constructor in inner const_iterator
                            {}

                            reference operator*() const {
                                return *current_inner_array_it;
                            }

                            pointer operator->() const {
                                return &(operator*());
                            }

                            const_iterator& operator++() {
                                ++current_inner_array_it;
                                if (current_inner_array_it == outer_array_ptr->get(current_bucket_idx).end()) {
                                    current_bucket_idx++;
                                    while (current_bucket_idx < outer_array_ptr->size() &&
                                           outer_array_ptr->get(current_bucket_idx).empty()) {
                                        current_bucket_idx++;
                                    }
                                    if (current_bucket_idx < outer_array_ptr->size()) {
                                        current_inner_array_it = outer_array_ptr->get(current_bucket_idx).begin();
                                    } // Else, we are at the end state
                                }
                                return *this;
                            }
                            const_iterator operator++(int) { const_iterator temp = *this; ++(*this); return temp; }

                            bool operator==(const const_iterator& other) const {
                                bool this_is_end = current_bucket_idx >= outer_array_ptr->size();
                                bool other_is_end = other.current_bucket_idx >= other.outer_array_ptr->size();

                                if (this_is_end && other_is_end) {
                                    return true;
                                }
                                if (this_is_end != other_is_end) {
                                    return false;
                                }
                                return current_bucket_idx == other.current_bucket_idx &&
                                       current_inner_array_it == other.current_inner_array_it;
                            }
                            bool operator!=(const const_iterator& other) const { return !(*this == other); }
                    };

                    // begin() and end() methods for hash_map
                    iterator begin() {
                        size_t first_bucket_idx = 0;
                        // Find the first non-empty bucket
                        while (first_bucket_idx < buckets.size() && buckets.get(first_bucket_idx).empty()) {
                            first_bucket_idx++;
                        }

                        // If found, get its begin iterator
                        if (first_bucket_idx < buckets.size()) {
                            return iterator(&buckets, first_bucket_idx, buckets.get(first_bucket_idx).begin());
                        } else {
                            // Map is empty, return end iterator
                            return end();
                        }
                    }

                    iterator end() {
                        // The end iterator points past the last bucket.
                        // The inner_it can be default-constructed or point to end() of a dummy/empty array.
                        // The key is that current_bucket_idx == buckets.size()
                        return iterator(&buckets, buckets.size(), typename prb17::utils::structures::array<std::pair<K,V>>::iterator{});
                    }

                    const_iterator begin() const {
                        size_t first_bucket_idx = 0;
                        while (first_bucket_idx < buckets.size() && buckets.get(first_bucket_idx).empty()) {
                            first_bucket_idx++;
                        }
                        if (first_bucket_idx < buckets.size()) {
                            return const_iterator(&buckets, first_bucket_idx, buckets.get(first_bucket_idx).cbegin());
                        } else {
                            return cend();
                        }
                    }
                    const_iterator end() const {
                        return const_iterator(&buckets, buckets.size(), typename prb17::utils::structures::array<std::pair<K,V>>::const_iterator{});
                    }
                    const_iterator cbegin() const { return begin(); }
                    const_iterator cend() const { return end(); }
                };
            };

            template<typename K, typename V>
            map<K,V>& map<K,V>::operator=(map &&m) {
                if (&m != this) {
                    buckets = m.buckets;
                    sz = m.sz;
                    hash_function = m.hash_function;
                }
                return *this;
            }
 
            template<typename K, typename V>
            map<K,V>& map<K,V>::operator=(const map &m) {
                if (&m != this) {
                    buckets = m.buckets;
                    sz = m.sz;
                    hash_function = m.hash_function;
                }
                return *this;
            }
 

            template<typename K, typename V>
            V& map<K,V>::operator[](K& key) {
                return get(key);
            }
            
            template<typename K, typename V>
            const V& map<K,V>::operator[](K& key) const {
                return get(key);
            }

            template<typename K, typename V>
            void map<K,V>::add(const map<K,V>& m) {
                for(int i; i<m.capacity(); i++) {
                    array<pair<K,V>> bucket = m.buckets[i];
                    for(int j; j<bucket.size(); j++) {
                        this->add(bucket[j].key(), bucket[j].value()); 
                    } 
                } 
            }

            template<typename K, typename V>
            void map<K,V>::add(const K& key, const V& value) {
                size_t bucket_idx = get_bucket_index(key);
                array<pair<K,V>> &bucket = buckets[bucket_idx];

                // Check if key already exists in the bucket
                for (size_t i = 0; i < bucket.size(); ++i) {
                    if (bucket[i].key() == key) {
                        // Key found, update the value
                        bucket[i] = pair<K,V>(key, value);
                        return;
                    }
                }

                // Key not found, add a new pair
                bucket.add(pair<K,V>(key, value));
                sz++;
                // Optional: Resize logic can be added here if needed
                // e.g., if (sz > buckets.capacity() * LOAD_FACTOR) { resize(); }
            }
            
            template<typename K, typename V>
            V& map<K,V>::get(const K& key) {
                size_t bucket_idx = get_bucket_index(key);
                array<pair<K,V>> &bucket = buckets[bucket_idx];

                for (size_t i = 0; i < bucket.size(); ++i) {
                    if (bucket[i].key() == key) {
                        return bucket[i].value();
                    }
                }

                // Key not found
                throw prb17::utils::exception("Key not found in map.");
            }

            template<typename K, typename V>
            const V& map<K,V>::get(const K& key) const {
                size_t bucket_idx = get_bucket_index(key);
                array<pair<K,V>> &bucket = buckets[bucket_idx];

                for (size_t i = 0; i < bucket.size(); ++i) {
                    if (bucket[i].key() == key) {
                        return bucket[i].value();
                    }
                }

                // Key not found
                throw prb17::utils::exception("Key not found in map.");
            }

            template<typename K, typename V>
            void map<K,V>::remove(const K& key) {
                size_t bucket_idx = get_bucket_index(key);
                array<pair<K,V>> &bucket = buckets[bucket_idx];

                for (size_t i = 0; i < bucket.size(); ++i) {
                    if (bucket[i].key() == key) {
                        // Found the key, remove the pair from the bucket
                        bucket.remove(i); // Assuming 'array' has a remove by index method.
                                          // If not, you'd need to shift elements or mark as invalid.
                        sz--;
                        return;
                    }
                }

                // Key not found (optional: throw an exception or do nothing)
                // For this example, we'll just return if not found.
            }

            template<typename K, typename V>
            size_t map<K,V>::size() const {
                return sz;
            }

            template<typename K, typename V>
            size_t map<K,V>::capacity() const {
                return buckets.capacity(); //TODO: needed?
            }

            template<typename K, typename V>
            bool map<K,V>::is_empty() const {
                return sz == 0;
            }

            template<typename K, typename V>
            bool map<K,V>::at_capacity() const {
                return false; //TODO: how to tell if map is at/near capacity?
            }

            template<typename K, typename V>
            std::string map<K,V>::to_string() const {
                return buckets.to_string();
/*
                std::stringstream ss;
                ss << "{";
                bool first_element = true;

                for (size_t i = 0; i < buckets.capacity(); ++i) {
                    const array<pair<K,V>> &bucket = buckets[i];
                    for (size_t j = 0; j < bucket.size(); ++j) {
                        if (!first_element) {
                            ss << ", ";
                        }
                        ss << bucket[j];
                        first_element = false;
                    }
                }
                ss << "}";
                return ss.str();
*/
            }

            template<typename K, typename V>
            inline std::ostream& operator<<(std::ostream &stream, const map<K,V>& m) {
                return stream << m.to_string();
            }
            
            template<typename K, typename V>
            inline std::ostream& operator<<(std::ostream &stream, const map<K,V>* m) {
                return stream << m->to_string();
            }
        }
    }
}
