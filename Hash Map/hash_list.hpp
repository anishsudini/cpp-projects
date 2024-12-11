#include "hash_list.h"

template <typename K, typename V>
hash_list<K, V>::hash_list() : size(0), head(nullptr), iter_ptr(nullptr) {}


template <typename K, typename V>
void hash_list<K, V>::insert(K key, V value) {
    node<K, V>* curr = head;
    while(curr != nullptr) {                    //traverse linked list
        if(curr->key == key) {                  //if key exists 
            curr->value = value;                //update its respective value
            return; 
        }
        curr = curr->next;                      //if key doesn't match -> move to next node
    }
                                                //if key doesn't exist or nullptr
    node<K, V>* newNode = new node<K, V>;                 //create newNode
    newNode->key = key;                         //set key
    newNode->value = value;                     //set value
    newNode->next = head;                       //set next node pointer as the head
    head = newNode;                             //insert new node at beginning

    size++;                                     //increment size of linked list
}


template <typename K, typename V>
std::optional<V> hash_list<K, V>::get_value(K key) const {
    node<K, V>* curr = head;
    while(curr != nullptr) {                    //traverse linked list
        if(curr->key == key) {                  //if key exists 
            return curr->value;                 //return value at current node
        }
        curr = curr->next;                      //if key doesn't match -> set curr to next node
    }
    return std::nullopt; 
}


template <typename K, typename V>
bool hash_list<K, V>::remove(K key) {               
    node<K, V>* curr = head; 
    node<K, V>* prev = nullptr;                       //keep track of previous node
    while(curr != nullptr) {                    //traverse linked list
        if(curr->key == key) {                  //if key exists
            if(prev == nullptr) {               //checks if head needs to be removed 
                head = curr->next;
            } 
            else {                              //else some node, not the head
                prev->next = curr->next;        //set prev next node to curr next node to skip over curr
            }
            delete curr;                        //delete the node
            size--;                             //decrement size of linked list
            return true;                        //return true
        }
        prev = curr;                            //if key doesn't match -> set prev node to the curr node
        curr = curr->next;                      //if key doesn't match -> set curr to next node
    }
    return false; 
}


template <typename K, typename V>
size_t hash_list<K, V>::get_size() const { return size; }


template <typename K, typename V>
hash_list<K, V>::~hash_list() {
    node<K, V>* curr = head;
    while(curr != nullptr) {
        node<K, V>* next = curr->next;
        delete curr;
        curr = next;
    }
    size = 0;
    head = nullptr;
}


template <typename K, typename V>
hash_list<K, V>::hash_list(const hash_list &other) : size(0), head(nullptr) {                    
    if(other.head == nullptr) {                 //check if 'other' list is empty
        return;
    }

    head = new node<K, V>;                          //insert the first node
    head->key = other.head->key;
    head->value = other.head->value;
    head->next = nullptr;

    size++;                                     //update size

    node<K, V>* curr = head;                          //track first node in new list
    node<K, V>* curr_other = other.head->next;        //move to next node in 'other' list

    while(curr_other != nullptr) {              //loop till end of 'other' list
        node<K, V>* new_node = new node<K, V>;            //create new node
        new_node->key = curr_other->key;              
        new_node->value = curr_other->value;
        new_node->next = nullptr;

        curr->next = new_node;                  //link new node to new list

        curr = new_node;                        //move pointer in new list to the next new node

        size++;                                 //update size

        curr_other = curr_other->next;          //move to next node in other list
    }                                           
}                                               


template <typename K, typename V>
hash_list<K, V> &hash_list<K, V>::operator=(const hash_list &other) { 
    if(this == &other) {                        //check for self-assignment
        return *this;
    }

    hash_list<K, V> temp(other);                //call copy constructor to store deep copy of 'other' in temp

    std::swap(head, temp.head);                 //swap head pointer between current object & temp
    std::swap(size, temp.size);                 //swap size between current object & temp

    return *this; 
}


template <typename K, typename V>
void hash_list<K, V>::reset_iter() { iter_ptr = head; } 


template <typename K, typename V>
void hash_list<K, V>::increment_iter() {
    if(iter_ptr != nullptr) {                   //check if at end of list
        iter_ptr = iter_ptr->next;              //move to next node
    }
}


template <typename K, typename V>
std::optional<std::pair<const K *, V *>> hash_list<K, V>::get_iter_value() {
    if(iter_ptr == nullptr) {                   //check if at end of list/NULL 
        return std::nullopt;                    //return empty optional if iter_ptr is at end of list/NULL  
    }

    return std::make_optional(std::make_pair(&iter_ptr->key, &iter_ptr->value)); //create a pair w/ key and value pointers
}


template <typename K, typename V>
bool hash_list<K, V>::iter_at_end() { return iter_ptr == nullptr; }