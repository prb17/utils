#include "array.hh"
#include "pair.hh"

namespace prb17 {
    namespace utils {
        namespace structures {
            template <typename K, typename V>
            class simple_map {
                private:
                    prb17::utils::structures::array<prb17::utils::structures::pair<K,V>> data_;
                    size_t current_map_size_;

                public:
                    explicit simple_map(size_t initial_capacity = 0)
                        : data_(initial_capacity), current_map_size_(0) {
                        //for(size_t i = 0; i < initial_capacity; ++i) {
                        //    data_.add(prb17::utils::structures::pair<K,V>{});
                        //}
                    }
            };
        }
    }
}
