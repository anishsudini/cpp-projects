#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <optional>
#include <stddef.h>
#include <stdlib.h>

#include "hash_list.h"

template <typename K, typename V>
class hash_map
{

public:
    /**
     * @brief Construct a new hash map object
     */
    hash_map(size_t capacity,
             float upper_load_factor,
             float lower_load_factor);

    /**
     * @brief Construct a new hash map object
     *
     * @param other
     *  The map to create a copy of
     */
    hash_map(const hash_map &other);

    /**
     * @brief Constructs a new hash map from other
     *
     * @param other
     *  The map to create a copy of
     * @return hash_map&
     *  Returns a reference to the newly constructed hash map. Ensures that
     *  a = b = c works
     */
    hash_map &operator=(const hash_map &other);

    /**
     * @brief Inserts the key/value pair into the map. If the specified key already exists
     * in the map, updates the associated value, otherwise inserts a new key value pair.
     *
     * @param key
     *  The key to insert
     * @param value
     *  The value to insert
     */
    void insert(K key, V value);

    /**
     * @brief Returns an optional containing the value associated with the specified key.
     * If the key isn't in the map return an empty optional.
     *
     * @param key
     *  The key to search for
     * @return
     *  An empty optional (if the key isn't in the map), otherwise returns an optional
     *  containing the value associated with the specified key
     */
    std::optional<V> get_value(K key) const;

    /**
     * @brief Removes the key and corresponding value from the map and returns true.
     *
     * @param key
     *  The key to remove from the list
     * @return
     *  True if key was present
     *  False otherwise
     */
    bool remove(K key);

    /**
     * @brief Returns the number of key/value pairs in the map
     */
    size_t get_size() const;

    /**
     * @brief Returns the capacity of the map
     *
     * @return
     *  The capacity of the hash map
     */
    size_t get_capacity() const;

    /**
     * @brief Copies all the keys from the hash_map into the specified array
     *
     * @param keys
     *  A pointer to an array that has enough space to store all the keys
     *  in the hash_map.
     */
    void get_all_keys(K *keys);

    /**
     * @brief Copies all the keys from the hash_map into the specified array
     * and sorts the array by key value. 
     *
     * @param keys
     *  A pointer to an array that has enough space to store all the keys
     *  in the hash_map.
     */
    void get_all_sorted_keys(K *keys);

    /**
     * @brief Gets the number of elements in each hash_list pointed to by _head.
     *
     * @param buckets
     *  A pointer to an array that has at least _capacity elements
     */
    void get_bucket_sizes(size_t *buckets);

    /**
     * @brief Frees all memory associated with the map
     */
    ~hash_map();

private:
    /** A pointer to an array of hash_lists */
    hash_list<K, V> *_head;

    /** The number of key/value pairs in the map */
    size_t _size;

    /** The number of buckets in the hash map */
    size_t _capacity;

    /** The load factor that determines when we increase hash map capacity */
    float _upper_load_factor;

    /** The load factor that determines when we decrease hash map capacity */
    float _lower_load_factor;

    /** Built in hashing function for type K */
    std::hash<K> _hash;

    static size_t _capacities[];

    void rehash(bool increase_capacity);

    void adjust_capacity();
};

template <typename K, typename V>
size_t hash_map<K, V>::_capacities[] = {209, 1021, 2039}; //example capacities

#include "hash_map.hpp"

#endif