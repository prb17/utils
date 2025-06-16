#include "array.hh"

namespace prb17 {
    namespace utils {
        namespace structures {
            template <typename T>
            class set {
                private:
                    array<T> elements;

                public:
                    explicit set(size_t initial_capacity = 0) : elements(initial_capacity) {}
            };
        }
    }
}
