#pragma once
#include <string>
#include <sstream>

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class container {
                private:
                    T v;

                public:
                    container();
                    container (T);
                    container(const container &);
                    ~container();

                    T value();
                    std::string to_string() const;

                    //overloaded operators
                    container& operator=(const container &);
                    container& operator=(const T);
                    
                    bool operator==(const container<T>&);
                    bool operator==(T);
                    bool operator!=(const container<T>&);
                    bool operator!=(T);

                    bool operator<(const container<T>&);
                    bool operator<(T);
                    bool operator>(const container<T>&);
                    bool operator>(T);
                    bool operator<=(const container<T>&);
                    bool operator<=(T);
                    bool operator>=(const container<T>&);
                    bool operator>=(T);
                    
            };

            template<typename T>
            container<T>::container() : container(T{}) {}

            template<typename T>
            container<T>::container(T d) {
                v = d;
            }

            template<typename T>
            container<T>::container(const container &c) {
                v = const_cast<container<T>&>(c).value();
            }

            template<typename T>
            container<T>::~container() {
                // if (v != nullptr) {
                //     delete v;
                // }                
            }

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
                stream << const_cast<container<T>*>(this)->value();
                return stream.str();
            }            

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const container<T>& c) {
                return stream << c.to_string();
            }

            //operator =
            template<typename T>
            container<T>& container<T>::operator=(const container &c) {
                if (&c != this) {
                    v = (c.v);
                }
                return *this;
            }
            template<typename T>
            container<T>& container<T>::operator=(const T val) {
                v = val;
                return *this;
            }

            //operator ==
            template<typename T>
            bool container<T>::operator==(const container<T> &c) {
                return this->value() == const_cast<container<T>&>(c).value();
            }
            template<typename T>
            bool container<T>::operator==(T v) {
                return this->value() == v;
            }

            //operator !=
            template<typename T>
            bool container<T>::operator!=(const container<T> &c) {
                return !(*this == c);
            }
            template<typename T>
            bool container<T>::operator!=(T v) {
                return !(this->value() == v);
            }

            //operator <
            template<typename T>
            bool container<T>::operator<(const container<T> &c) {
                return this->value() < const_cast<container<T>&>(c).value();
            }
            template<typename T>
            bool container<T>::operator<(T v) {
                return this->value() < v;
            }

            //operator >
            template<typename T>
            bool container<T>::operator>(const container<T> &c) {
                return const_cast<container<T>&>(c) < *this;
            }
            template<typename T>
            bool container<T>::operator>(T v) {
                return v < this->value();
            }
            
            //operator <=
            template<typename T>
            bool container<T>::operator<=(const container<T> &c) {
                return !(*this > c);
            }
            template<typename T>
            bool container<T>::operator<=(T v) {
                return !(this->value() > v);
            }      
            
            //operator >=
            template<typename T>
            bool container<T>::operator>=(const container<T> &c) {
                return !(*this < c);
            }
            template<typename T>
            bool container<T>::operator>=(T v) {
                return !(this->value() < v);
            }
            
        }
    }
}