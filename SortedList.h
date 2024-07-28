#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {

    template <typename T>
    class SortedList {
    private:
        struct Node {
            T data;
            Node* next;
            explicit Node(const T& data, Node* next = nullptr) : data(data), next(next) {}
        };

        Node* Head;

        void Delete();
        void copyFrom(const SortedList& other);

    public:
        SortedList();
        SortedList(const SortedList& other);
        SortedList& operator=(const SortedList& other);
        ~SortedList();

        class ConstIterator;

        ConstIterator begin();
        ConstIterator end();
        ConstIterator begin() const;
        ConstIterator end() const;
        void insert(const T& data);
        void remove(const ConstIterator& it);
        int length() const;
        template<typename Predicate>
        SortedList<T> filter(Predicate predicate) const;
        template<typename Operation>
        SortedList<T> apply(Operation op) const;
    };

    template <class T>
    class SortedList<T>::ConstIterator {
        const SortedList<T>* list;
        Node* node;
        ConstIterator(const SortedList<T>* list, Node* node);
        friend class SortedList;

    public:
        ConstIterator(const ConstIterator& other) = default;
        ConstIterator& operator=(const ConstIterator& other) = default;
        ~ConstIterator() = default;

        const T& operator*() const;
        ConstIterator& operator++();
        bool operator!=(const ConstIterator& other) const;
    };

    template <class T>
    void SortedList<T>::Delete() {
        Node* current = Head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        Head = nullptr;
    }

    template <class T>
    void SortedList<T>::copyFrom(const SortedList& other) {
        Node* currentHead = other.Head;
        while (currentHead) {
            insert(currentHead->data);
            currentHead = currentHead->next;
        }
    }

    template <class T>
    SortedList<T>::SortedList() : Head(nullptr) {}

    template <class T>
    SortedList<T>::SortedList(const SortedList<T>& other) : Head(nullptr) {
        copyFrom(other);
    }

    template <class T>
    SortedList<T>& SortedList<T>::operator=(const SortedList& other) {
        if (this == &other) {
            return *this;
        }
        Delete();
        copyFrom(other);
        return *this;
    }

    template <class T>
    SortedList<T>::~SortedList() {
        Delete();
    }

    template <class T>
    void SortedList<T>::insert(const T& data) {
        Node* nodeToInsert = new Node(data);
        Node* currentNode = Head;
        if (!Head || nodeToInsert->data > Head->data) {
            nodeToInsert->next = currentNode;
            Head = nodeToInsert;
        } else {
            while (currentNode->next && currentNode->next->data > data) {
                currentNode = currentNode->next;
            }
            nodeToInsert->next = currentNode->next;
            currentNode->next = nodeToInsert;
        }
    }

    template <class T>
    void SortedList<T>::remove(const ConstIterator& it) {
        if (it.list != this) {
            throw std::invalid_argument("Invalid iterator");
        }
        if(it.node == nullptr){
            return;
        }
        Node* current = Head;
        Node* prev = nullptr;

        while (current && current != it.node) {
            prev = current;
            current = current->next;
        }

        if (current == it.node) {
            if (prev) {
                prev->next = current->next;
            } else {
                Head = current->next;
            }
            delete current;
        } else {
            throw std::invalid_argument("Iterator does not point to a valid node");
        }
    }

    template <class T>
    int SortedList<T>::length() const {
        int count = 0;
        Node* current = Head;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }

    template <class T>
    template <typename Predicate>
    SortedList<T> SortedList<T>::filter(Predicate predicate) const {
        SortedList<T> result;
        Node* current = Head;
        while (current) {
            if (predicate(current->data)) {
                result.insert(current->data);
            }
            current = current->next;
        }
        return result;
    }

    template <class T>
    template <typename Operation>
    SortedList<T> SortedList<T>::apply(Operation op) const {
        SortedList<T> result;
        Node* current = Head;
        while (current) {
            T data = op(current->data);
            result.insert(data);
            current = current->next;
        }
        return result;
    }

    template <class T>
    typename SortedList<T>::ConstIterator SortedList<T>::begin() const {
        return ConstIterator(this, Head);
    }

    template <class T>
    typename SortedList<T>::ConstIterator SortedList<T>::begin() {
        return ConstIterator(this, Head);
    }

    template <class T>
    typename SortedList<T>::ConstIterator SortedList<T>::end() const {
        return ConstIterator(this, nullptr);
    }

    template <class T>
    typename SortedList<T>::ConstIterator SortedList<T>::end() {
        return ConstIterator(this, nullptr);
    }

    template <typename T>
    SortedList<T>::ConstIterator::ConstIterator(const SortedList<T>* list, Node* node) : list(list), node(node) {}

    template <typename T>
    const T& SortedList<T>::ConstIterator::operator*() const {
        if (node == nullptr) {
            throw std::out_of_range("Dereferencing end iterator");
        } else {
        return node->data;
        }
    }

    template <typename T>
    typename SortedList<T>::ConstIterator& SortedList<T>::ConstIterator::operator++() {
        if (node == nullptr) {
            throw std::out_of_range("Incrementing end iterator");
        } else {
            node = node->next;
            return *this;
        }
    }

    template <typename T>
    bool SortedList<T>::ConstIterator::operator!=(const ConstIterator& other) const {
        if (list != other.list) {
            throw std::invalid_argument("Comparing iterators from different lists");
        }
        return node != other.node;
    }

}