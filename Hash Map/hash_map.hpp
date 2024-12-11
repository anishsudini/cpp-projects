#include "hash_map.h"

template <typename K, typename V>
hash_map<K, V>::hash_map(size_t capacity, float upper_load_factor, float lower_load_factor) : _size(0), _capacity(capacity), _upper_load_factor(upper_load_factor), _lower_load_factor(lower_load_factor) {
    adjust_capacity();
    _head = new hash_list<K, V>[_capacity];
}


template <typename K, typename V>
hash_map<K, V>::hash_map(const hash_map &other) : _size(other._size), _capacity(other._capacity), _upper_load_factor(other._upper_load_factor), _lower_load_factor(other._lower_load_factor) {
    _head = new hash_list<K, V>[_capacity];
    for (size_t i = 0; i < _capacity; ++i) {
        _head[i] = other._head[i];
    }
}


template <typename K, typename V>
hash_map<K, V> &hash_map<K, V>::operator=(const hash_map &other) {
    if(this == &other) {                              
        return *this;
    }

    hash_map<K, V> temp(other);                               

    std::swap(_size, temp._size);                      
    std::swap(_capacity, temp._capacity);               
    std::swap(_head, temp._head);                       

    return *this; 
}


template <typename K, typename V>
void hash_map<K, V>::insert(K key, V value) {
    size_t hash = _hash(key) % _capacity;
    if (!_head[hash].get_value(key).has_value()) {
        _size++; 
    }            
    _head[hash].insert(key, value);
    if (_size > _upper_load_factor * _capacity) {
        rehash(true); 
    }
}


template <typename K, typename V>
std::optional<V> hash_map<K, V>::get_value(K key) const {
    size_t hash = _hash(key) % _capacity;
    return _head[hash].get_value(key);                  
}


template <typename K, typename V>
bool hash_map<K, V>::remove(K key) {
    size_t hash = _hash(key) % _capacity;
    if(_head[hash].remove(key)) {
        _size--;
        if(_size < _lower_load_factor * _capacity) {
            rehash(false);  
        }
        return true;
    }
    return false;
}


template <typename K, typename V>
size_t hash_map<K, V>::get_size() const { return _size; }


template <typename K, typename V>
size_t hash_map<K, V>::get_capacity() const { return _capacity; }


template <typename K, typename V>
void hash_map<K, V>::get_all_keys(K *keys) {
    size_t index = 0;
    for(size_t i = 0; i < _capacity; ++i) {
        _head[i].reset_iter();
        while(!_head[i].iter_at_end()) {
            std::optional<std::pair<const int*, float*>> pair = _head[i].get_iter_value();
            if(pair.has_value()) {
                keys[index++] = *(pair->first);
            }
            _head[i].increment_iter();
        }
    }
}


template <typename K, typename V>
void hash_map<K,V>::get_all_sorted_keys(K *keys) {
    get_all_keys(keys);
    for (size_t i = 0; i < _size; ++i) {
        for (size_t j = i + 1; j < _size; ++j) {
            if (keys[i] > keys[j]) {
                std::swap(keys[i], keys[j]);
            }
        }
    }
}


template <typename K, typename V>
void hash_map<K, V>::get_bucket_sizes(size_t *buckets) {
    for(size_t i = 0; i < _capacity; ++i) {
        buckets[i] = _head[i].get_size();
    }
}


template <typename K, typename V>
hash_map<K, V>::~hash_map() { delete[] _head; }


template <typename K, typename V>
void hash_map<K,V>::rehash(bool increase_capacity) {
    size_t prev_capacity = _capacity;
    if(increase_capacity) { //increase capacity
        for(size_t i = 0; i < sizeof(_capacities) / sizeof(_capacities[0]); ++i) {
            if(_capacities[i] > _capacity) {
                _capacity = _capacities[i];
                break;
            }
        }
    } else { //decrease capacity
        for(size_t i = sizeof(_capacities) / sizeof(_capacities[0]); i-- > 0;) {
            if(_capacities[i] < _capacity) { 
                _capacity = _capacities[i];
                break;
            }
        }
    }

    hash_list<K, V>* new_head = new hash_list<K,V>[_capacity];

    for(size_t i = 0; i < prev_capacity; ++i) {
        _head[i].reset_iter();
        while(!_head[i].iter_at_end()) {
            auto key_val_pair = _head[i].get_iter_value();
            if(key_val_pair.has_value()) {
                size_t hash = _hash(*(key_val_pair->first)) % _capacity;
                new_head[hash].insert(*(key_val_pair->first), *(key_val_pair->second));
            }
            _head[i].increment_iter();
        }
    }

    delete[] _head;

    _head = new_head;
}

template <typename K, typename V>
void hash_map<K, V>::adjust_capacity() {
    for (size_t i = sizeof(_capacities) / sizeof(_capacities[0]); i-- > 0;) {
        if (_capacities[i] <= _capacity) {
            _capacity = _capacities[i];
            return;
        }
    }

    _capacity = _capacities[0];
}