#pragma once
#include "container.hh"

namespace prb17 {
    namespace utils {
        namespace structures {
            template<typename K, typename V>
            class pair {
                private:
                    container<K> k;
                    container<V> v;
                    
                public:
                    pair() = default;
                    pair(const K& k, const V& v) : k(k), v(v) {}    

                    K key() const;
                    V value() const;
                    std::string to_string() const;

                    bool operator==(const pair<K,V>&);
                    //bool operator==(K, V); TODO: implement when/if I make sets. i.e. operator=({K,V})
                    bool operator!=(const pair<K,V>&);
                    //bool operator!=(T);
                    bool operator<(const pair<K,V>&);
                    //bool operator<(T);
                    bool operator>(const pair<K,V>&);
                    //bool operator>(T);
                    bool operator<=(const pair<K,V>&);
                    //bool operator<=(T);
                    bool operator>=(const pair<K,V>&);
                    //bool operator>=(T);

            };
            
            template<typename K, typename V>
            K pair<K,V>::key() const {
                return k.value();
            }
            
            template<typename K, typename V>
            V pair<K,V>::value() const {
                return v.value();
            }

            template<typename K, typename V>
            std::string pair<K,V>::to_string() const {
                std::stringstream stream;
                stream << '{' << const_cast<pair<K,V>*>(this)->key() << ',' << const_cast<pair<K,V>*>(this)->value() << '}';
                return stream.str();
            }            

            template<typename K, typename V>
            inline std::ostream& operator<<(std::ostream &stream, const pair<K,V>& p) {
                return stream << p.to_string();
            }

            //operator ==
            template<typename K, typename V>
            bool pair<K,V>::operator==(const pair<K,V> &p) {
                return this->key() == const_cast<pair<K,V>&>(p).key() && this->value() == const_cast<pair<K,V>&>(p).value();
            }

            //operator !=
            template<typename K, typename V>
            bool pair<K,V>::operator!=(const pair<K,V> &p) {
                return !(*this == p);
            }   

            //operator <
            template<typename K, typename V>
            bool pair<K,V>::operator<(const pair<K,V> &p) {
                return this->value() < const_cast<pair<K,V>&>(p).value();
            }

            //operator >
            template<typename K, typename V>
            bool pair<K,V>::operator>(const pair<K,V> &p) {
                return const_cast<pair<K,V>&>(p) < *this;
            }
            
            //operator <=
            template<typename K, typename V>
            bool pair<K,V>::operator<=(const pair<K,V> &p) {
                return !(*this > p);
            }
            
            //operator >=
            template<typename K, typename V>
            bool pair<K,V>::operator>=(const pair<K,V> &p) {
                return !(*this < p);
            }

        }
    }
}
