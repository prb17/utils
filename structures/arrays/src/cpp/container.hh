#pragma once
#include<string>

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class container {
                private:
                    T v;

                public:
                    container();
                    container (T);

                    T value();
                    std::string to_string() const;
            };

            template<typename T>
            container<T>::container() : container(0) {}

            template<typename T>
            container<T>::container(T d): v{d} {}

            template<typename T>
            T container<T>::value() {
                return v;
            }

            /**
             * @brief return a string representation of the array
             * 
             * @return std::string - string representation of array
             */
            template<typename T>
            std::string container<T>::to_string() const {
                std::stringstream stream;
                stream << v;
                return stream.str();
            }
            

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const container<T>& c) {
                return stream << c.to_string();
            }
        }
    }
}