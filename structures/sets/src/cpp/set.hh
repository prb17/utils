#include "map.hh" // Your custom map class
#include <string>   // For example usage with std::string if needed

namespace prb17 {
    namespace utils {
        namespace structures {

            template <typename T>
            class set {
            private:
                struct dummy {};
                map<T, dummy> internal_map;

            public:
                set() : internal_map() {}

                bool add(const T& element) {
                    if (internal_map.contains(element)) {
                        return false;
                    }
                    internal_map.add(element, dummy{});
                    return true;
                }

                bool contains(const T& element) const {
                    return internal_map.contains(element);
                }

                void remove(const T& element) {
                    internal_map.remove(element);
                }

                size_t size() const {
                    return internal_map.size();
                }

                bool empty() const {
                    return internal_map.empty();
                }

                void clear() {
                    internal_map.clear();
                }

                class const_iterator {
                public:
                    using map_iterator = typename map<T, dummy>::const_iterator;
                    using value_type = T;
                    using reference = const T&;
                    using pointer = const T*;
                    using difference_type = typename map_iterator::difference_type;
                    using iterator_category = typename map_iterator::iterator_category;

                    const_iterator(map_iterator it) : map_it(it) {}

                    reference operator*() const { return map_it->first; }
                    pointer operator->() const { return &(map_it->first); }
                    const_iterator& operator++() { ++map_it; return *this; }
                    const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
                    bool operator==(const const_iterator& other) const { return map_it == other.map_it; }
                    bool operator!=(const const_iterator& other) const { return map_it != other.map_it; }

                private:
                    map_iterator map_it;
                };

                const_iterator cbegin() const { return const_iterator(internal_map.cbegin()); }
                const_iterator cend() const { return const_iterator(internal_map.cend()); }

                // Non-const iterators if your map supports them and you need them
                // auto begin() { return internal_map.begin(); }
                // auto end() { return internal_map.end(); }
            };

        } // namespace structures
    } // namespace utils
} // namespace prb17
