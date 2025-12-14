#pragma once

#include <iostream>
#include <string>

template <typename Key, typename Value>
class Node {
public:
    Key key;
    Value value;
    Node* next;

    Node(const Key& k, const Value& v)
        : key(k)
        , value(v)
        , next(nullptr)
    {
    }
};

template <typename Key, typename Value>
class List {
private:
    Node<Key, Value>* head;
    int size;

public:
    List()
        : head(nullptr)
        , size(0)
    {
    }

    ~List()
    {
        clear();
    }

    List& operator=(const List& other)
    {
        if (this != &other) {
            clear();
            Node<Key, Value>* current = other.head;
            while (current != nullptr) {
                add(current->key, current->value);
                current = current->next;
            }
        }
        return *this;
    }

    void add(const Key& k, const Value& v)
    {
        Node<Key, Value>* newNode = new Node<Key, Value>(k, v);
        newNode->next = head;
        head = newNode;
        size++;
    }

    Value* find(const Key& k)
    {
        Node<Key, Value>* current = head;
        while (current != nullptr) {
            if (current->key == k) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    bool remove(const Key& k)
    {
        Node<Key, Value>** current = &head;
        while (*current != nullptr) {
            if ((*current)->key == k) {
                Node<Key, Value>* toDelete = *current;
                *current = (*current)->next;
                delete toDelete;
                size--;
                return true;
            }
            current = &((*current)->next);
        }
        return false;
    }

    void clear()
    {
        while (head != nullptr) {
            Node<Key, Value>* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

    int getSize() const
    {
        return size;
    }

    Node<Key, Value>* getHead() const
    {
        return head;
    }
};

template <typename Key, typename Value>
class Map {
private:
    List<Key, Value>* data;
    int cap;
    int size;

    int calcHash(unsigned int key) const
    {
        return key % cap;
    }

    int calcHash(const std::string& key) const
    {
        unsigned long h = 5381;
        for (char c : key) {
            h = ((h << 5) + h) + c;
        }
        return h % cap;
    }

    template <typename K>
    int hash(const K& key) const
    {
        return calcHash(key);
    }

    void rehash()
    {
        int oldCapacity = cap;
        List<Key, Value>* oldData = data;

        cap *= 2;
        data = new List<Key, Value>[cap];
        size = 0;

        for (int i = 0; i < oldCapacity; i++) {
            Node<Key, Value>* current = oldData[i].getHead();
            while (current != nullptr) {
                add(current->key, current->value);
                current = current->next;
            }
        }

        delete[] oldData;
    }

public:
    Map()
        : cap(10)
        , size(0)
    {
        data = new List<Key, Value>[cap];
    }

    ~Map()
    {
        delete[] data;
    }

    Map(const Map& other)
        : cap(other.cap)
        , size(other.size)
    {
        data = new List<Key, Value>[cap];
        for (int i = 0; i < cap; i++) {
            data[i] = other.data[i];
        }
    }

    Map& operator=(const Map& other)
    {
        if (this != &other) {
            delete[] data;
            cap = other.cap;
            size = other.size;
            data = new List<Key, Value>[cap];
            for (int i = 0; i < cap; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    void add(const Key& key, const Value& value)
    {
        int index = hash(key);

        Value* existing = data[index].find(key);
        if (existing != nullptr) {
            *existing = value;
            return;
        }

        data[index].add(key, value);
        size++;

        if (1.0 * size / cap > 0.7) {
            rehash();
        }
    }

    Value* find(const Key& key)
    {
        int index = hash(key);
        return data[index].find(key);
    }

    bool remove(const Key& key)
    {
        int index = hash(key);
        if (data[index].remove(key)) {
            size--;
            return true;
        }
        return false;
    }

    friend std::ostream& operator<<(std::ostream& os, const Map& map)
    {
        for (int i = 0; i < map.cap; i++) {
            Node<Key, Value>* current = map.data[i].getHead();
            while (current != nullptr) {
                os << "Key: " << current->key << ", Value: " << current->value << std::endl;
                current = current->next;
            }
        }
        return os;
    }
};
