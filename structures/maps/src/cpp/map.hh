#include "array.hh"
#include "pair.hh"

#include <functional>

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
                        return hash_function(key) % buckets.capacity();
                    } 

                public:
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
                            }) : buckets(10), sz(0), hash_function(hash_func) {}

                    void add(const K& key, const V& value);
                    V get(const K& key);
                    void remove(const K& key);

                    size_t size() const;
                    size_t capacity() const;
                    bool is_empty() const;
                    bool at_capacity() const;

                    std::string to_string() const; 
            };


            template<typename K, typename V>
            void map<K,V>::add(const K& key, const V& value) {
                size_t bucket_idx = get_bucket_index(key);
                array<pair<K,V>> bucket = buckets[bucket_idx];

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
            V map<K,V>::get(const K& key) {
                size_t bucket_idx = get_bucket_index(key);
                array<pair<K,V>> bucket = buckets[bucket_idx];

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
                array<pair<K,V>> bucket = buckets[bucket_idx];

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
                return 0; //TODO: needed?
            }

            template<typename K, typename V>
            bool map<K,V>::is_empty() const {
                return sz == 0;
            }

            template<typename K, typename V>
            bool map<K,V>::at_capacity() const {
                return false; //TODO: how to tell if map is at/near capacity?
            }

            // New: to_string method
            template<typename K, typename V>
            std::string map<K,V>::to_string() const {
                std::stringstream ss;
                ss << "{";
                bool first_element = true;

                // Iterate through all buckets
                for (size_t i = 0; i < buckets.capacity(); ++i) {
                    const array<pair<K,V>>& bucket = buckets[i];
                    // Iterate through pairs in the current bucket
                    for (size_t j = 0; j < bucket.size(); ++j) {
                        if (!first_element) {
                            ss << ", ";
                        }
                        // Use the pair's overloaded operator<< or to_string()
                        // To handle pointers, ensure your pair's operator<< or K/V's operator<< can handle them.
                        // Assuming pair<K,V>::to_string() correctly handles K and V's streamability.
                        ss << bucket[j]; // This will use pair<K,V>::operator<<
                        first_element = false;
                    }
                }
                ss << "}";
                return ss.str();
            }

            // New: operator<< overload for map
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
