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
        Node* node;
        explicit ConstIterator(Node* node);
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
        if (other.Head == nullptr) {
            Head = nullptr;
            return;
        }
        Delete();
        // Copy the first node
        Head = new Node(other.Head->data);
        Node* current = Head;
        Node* otherCurrent = other.Head->next;

        // Copy the remaining nodes
        while (otherCurrent) {
            current->next = new Node(otherCurrent->data);
            current = current->next;
            otherCurrent = otherCurrent->next;
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
        if (this != &other) {
            SortedList<T> temp(other);
            //using already existing swap function for any template;
            copyFrom(temp);
        }
        return *this;
    }

    template <class T>
    SortedList<T>::~SortedList() {
        Delete();
    }

    template <class T>
    void SortedList<T>::insert(const T& data) {
        Node* nodeToInsert = new Node(data);

        if (!Head) {
            Head = nodeToInsert;
        } else if(nodeToInsert->data > Head->data){
            nodeToInsert->next = Head;
            Head = nodeToInsert;
        } else {
            Node* currentNode = Head;
            while (currentNode->next && currentNode->next->data > data) {
                currentNode = currentNode->next;
            }
            nodeToInsert->next = currentNode->next;
            currentNode->next = nodeToInsert;
        }
    }

    template <class T>
    void SortedList<T>::remove(const SortedList::ConstIterator &it) {
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
        return ConstIterator(Head);
    }

    template <class T>
    typename SortedList<T>::ConstIterator SortedList<T>::begin() {
        return ConstIterator(Head);
    }

    template <class T>
    typename SortedList<T>::ConstIterator SortedList<T>::end() const {
        return ConstIterator(nullptr);
    }

    template <class T>
    typename SortedList<T>::ConstIterator SortedList<T>::end() {
        return ConstIterator(nullptr);
    }

    template <typename T>
    SortedList<T>::ConstIterator::ConstIterator(Node* node) : node(node) {}

    template <typename T>
    const T& SortedList<T>::ConstIterator::operator*() const {
        if (node == nullptr) {
            throw std::range_error("Dereferencing end iterator");
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
        return node != other.node;
    }

}