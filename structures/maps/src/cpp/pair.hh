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
                    pair(const K& k, const V& v);    
                    pair(const pair<K,V> &);

                    K& key();
                    const K& key() const;
                    V& value();
                    const V& value() const;
                    std::string to_string() const;
                    
                    pair& operator=(pair &&); //move assignment operator
                    pair& operator=(const pair &); //copy assignment operator

                    bool operator==(const pair<K,V>&) const;
                    //bool operator==(K, V); TODO: implement when/if I make sets. i.e. operator=({K,V})
                    bool operator!=(const pair<K,V>&) const;
                    //bool operator!=(T);
                    bool operator<(const pair<K,V>&) const;
                    //bool operator<(T);
                    bool operator>(const pair<K,V>&) const;
                    //bool operator>(T);
                    bool operator<=(const pair<K,V>&) const;
                    //bool operator<=(T);
                    bool operator>=(const pair<K,V>&) const;
                    //bool operator>=(T);

            };

            template<typename K, typename V>
            pair<K,V>::pair(const K& k, const V& v) : k(k), v(v) {}

            template<typename K, typename V>
            pair<K,V>::pair(const pair<K,V> &p) {
                k = p.k;
                v = p.v; 
            }
             
            template<typename K, typename V>
            K& pair<K,V>::key() {
                return k.value();
            }

            template<typename K, typename V>
            const K& pair<K,V>::key() const {
                return k.value();
            }
             
            template<typename K, typename V>
            V& pair<K,V>::value() {
                return v.value();
            }
           
            template<typename K, typename V>
            const V& pair<K,V>::value() const {
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

            template<typename K, typename V>
            pair<K,V>& pair<K,V>::operator=(pair &&p) {
                if (&p != this) {
                    k = p.k;
                    v = p.v;
                }
                return *this;
            }

            template<typename K, typename V>
            pair<K,V>& pair<K,V>::operator=(const pair &p) {
                if (&p != this) {
                    k = p.k;
                    v = p.v;
                }
                return *this;
            }

            //operator ==
            template<typename K, typename V>
            bool pair<K,V>::operator==(const pair<K,V> &p) const {
                return this->key() == const_cast<pair<K,V>&>(p).key() && this->value() == const_cast<pair<K,V>&>(p).value();
            }

            //operator !=
            template<typename K, typename V>
            bool pair<K,V>::operator!=(const pair<K,V> &p) const {
                return !(*this == p);
            }   

            //operator <
            template<typename K, typename V>
            bool pair<K,V>::operator<(const pair<K,V> &p) const {
                return this->value() < const_cast<pair<K,V>&>(p).value();
            }

            //operator >
            template<typename K, typename V>
            bool pair<K,V>::operator>(const pair<K,V> &p) const {
                return const_cast<pair<K,V>&>(p) < *this;
            }
            
            //operator <=
            template<typename K, typename V>
            bool pair<K,V>::operator<=(const pair<K,V> &p) const {
                return !(*this > p);
            }
            
            //operator >=
            template<typename K, typename V>
            bool pair<K,V>::operator>=(const pair<K,V> &p) const {
                return !(*this < p);
            }

        }
    }
}
