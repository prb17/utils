#pragma once
#include <string>
#include <sstream>
#include <iostream>

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class container {
                private:
                    T* v;

                public:
                    container();
                    container (T);
                    container(const container &);
                    ~container();

                    T& value();
                    const T& value() const;
                    std::string to_string() const;

                    //overloaded operators
                    container& operator=(const container &);
                    container& operator=(T);
                    
                    bool operator==(const container<T>&) const;
                    bool operator==(T) const;
                    bool operator!=(const container<T>&) const;
                    bool operator!=(T) const;

                    bool operator<(const container<T>&) const;
                    bool operator<(T) const;
                    bool operator>(const container<T>&) const;
                    bool operator>(T) const;
                    bool operator<=(const container<T>&) const;
                    bool operator<=(T) const;
                    bool operator>=(const container<T>&) const;
                    bool operator>=(T) const;
                    
            };

            template<typename T>
            container<T>::container() : container(T{}) {}

            template<typename T>
            container<T>::container(T d) {
                v = new T{d};
            }

            template<typename T>
            container<T>::container(const container &c) {
                if (v != nullptr) {
                    delete v;
                }
                v = new T{(const_cast<container<T>&>(c).value())};
            }

            template<typename T>
            container<T>::~container() {
                if (v != nullptr && v != 0) {
                    delete v;
                }               
            }

            template<typename T>
            T& container<T>::value() {
                return *v;
            }

            template<typename T>
            const T& container<T>::value() const {
                return *v;
            }

            /**
             * @brief return a string representation of the value in the container 
             * 
             * @return std::string - string representation of the value in the container 
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
                    if (v != nullptr) {
                        delete v;
                    }
                    v = new T{(const_cast<container<T>&>(c).value())};
                }
                return *this;
            }
            template<typename T>
            container<T>& container<T>::operator=(T val) {
                if (v != nullptr) {
                    delete v;
                }
                v = new T{val};
                return *this;
            }

            //operator ==
            template<typename T>
            bool container<T>::operator==(const container<T> &c) const {
                return this->value() == const_cast<container<T>&>(c).value();
            }
            template<typename T>
            bool container<T>::operator==(T v) const {
                return this->value() == v;
            }

            //operator !=
            template<typename T>
            bool container<T>::operator!=(const container<T> &c) const {
                return !(*this == c);
            }
            template<typename T>
            bool container<T>::operator!=(T v) const {
                return !(this->value() == v);
            }

            //operator <
            template<typename T>
            bool container<T>::operator<(const container<T> &c) const {
                return this->value() < const_cast<container<T>&>(c).value();
            }
            template<typename T>
            bool container<T>::operator<(T v) const {
                return this->value() < v;
            }

            //operator >
            template<typename T>
            bool container<T>::operator>(const container<T> &c) const {
                return const_cast<container<T>&>(c) < *this;
            }
            template<typename T>
            bool container<T>::operator>(T v) const {
                return v < this->value();
            }
            
            //operator <=
            template<typename T>
            bool container<T>::operator<=(const container<T> &c) const {
                return !(*this > c);
            }
            template<typename T>
            bool container<T>::operator<=(T v) const {
                return !(this->value() > v);
            }      
            
            //operator >=
            template<typename T>
            bool container<T>::operator>=(const container<T> &c) const {
                return !(*this < c);
            }
            template<typename T>
            bool container<T>::operator>=(T v) const {
                return !(this->value() < v);
            }
            
        }
    }
}
