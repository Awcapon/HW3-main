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
            Node(const T& data, Node* next = nullptr) : data(data), next(next) {}
        };

        Node* Head;

        void Delete();
        void copyFrom(const SortedList& other);

        /* (1) Type T must be able to compare elements , as in have "<,>,<=,>=,==" implemented the write way.
         * also if printing is requested, T must have an implementation of "<<(ostream)"
         * in order to have an output;
         *
         * (2) Using a non-Const Iterator that it's operator * returns a reference of T, might lead
         * to changes in the sortings of the sorted list which is unwanted!
         * this may lead to also the ability to delete elements, where iterators that
         * point to it may point to Null now and that could cause errors!
         *
         * (3) Filter here is a function that receives a function variable (Predict),
         * say it recieves a function Dividable_by_p which returns a boolean, true if dividable
         * and false otherwise. loop through the elements of sortedList , send them to
         * the received function and which ever returns true is added to new list.
         * when all is done, we return the new list which is sorted and all elements
         * are dividable by p
         */
    public:
        SortedList();
        SortedList(const SortedList& other);
        SortedList& operator=(const SortedList& other);
        bool operator==(const SortedList& other);
        ~SortedList();

        class ConstIterator;

        ConstIterator begin();
        ConstIterator end();
        ConstIterator begin() const;
        ConstIterator end() const;
        void insert(const T& data);
        void remove(const T& data);
        int Length() const;
        template<typename Predicate>
        SortedList<T> filter(Predicate predicate) const;
        template<typename Operation>
        SortedList<T> apply(Operation op);
        /**
         *
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. SortedList() - creates an empty list.
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~SortedList() - destructor
         *
         * iterator:
         * 5. class ConstIterator;
         * 6. begin method
         * 7. end method
         *
         * functions:
         * 8. insert - inserts a new element to the list
         * 9. remove - removes an element from the list
         * 10. length - returns the number of elements in the list
         * 11. filter - returns a new list with elements that satisfy a given condition
         * 12. apply - returns a new list with elements that were modified by an operation
         */

    };

    template <class T>
    void SortedList<T> ::  Delete(){
        Node* Current = Head;
        while(Current){
            Node *Next = Current->next;
            delete Current;
            Current = Next;
        }
    }

    template <class T>
    void SortedList<T>::copyFrom(const SortedList& other) {
        Node* CurrentHead = other.Head;
        while(CurrentHead) {
            insert(CurrentHead->data);
            CurrentHead = CurrentHead->next;
        }
    }

    template <class T>
    SortedList<T>::SortedList() : Head(nullptr) {}

    template <class T>
    SortedList<T>::SortedList(const SortedList<T> &other) {
        Head = nullptr;
        copyFrom(other);
    }

    template <class T>
    bool SortedList<T> :: operator==(const SortedList& other){
        const Node *originalNode = this->Head, *otherNode = other.Head;
        if (this->Length() != other.Length())
            return false;
        while(originalNode && otherNode){
            if (!(originalNode->data == otherNode->data)){
                return false;
            }
            originalNode = originalNode->next;
            otherNode = otherNode->next;
        }
        if(!originalNode && !otherNode) {
                return true;
            }
        return false;
    }

    template <class T>
    SortedList<T>& SortedList<T> :: operator=(const SortedList& other){
        if(*this == other){
            return *this;
        } else {
            Delete();
            copyFrom(other);
            return *this;
        }
    }

    template<class T>
    SortedList<T> :: ~SortedList(){
        Delete();
    }

    template<class T>
    void SortedList<T> :: insert(const T& data){
        Node* nodeToInsert = new Node(data);
        if(!Head || nodeToInsert->data > Head->data){
            nodeToInsert->next = Head;
            Head = nodeToInsert;
        } else{
            Node *currentNode = Head;
            while(currentNode->next && currentNode->next->data > data){
                currentNode = currentNode->next;
            }
            nodeToInsert->next = currentNode->next;
            currentNode->next = nodeToInsert;
        }
    }

    template<class T>
    void SortedList<T> :: remove(const T& data){
        Node* currentNode = Head, previousNode = nullptr;

        while(currentNode && currentNode->data != data){
            previousNode = currentNode;
            currentNode = currentNode->next;
        }
        if(currentNode){
            if(previousNode){
                previousNode.next = currentNode->next;
            }  else {
                Head = currentNode->next;
            }
            delete currentNode;
        }
    }

    template<class T>
    int SortedList<T> :: Length() const{
        Node * tempNode = Head;
        int count = 0;
        while(tempNode){
            count++;
            tempNode = tempNode->next;
        }
        return count;
    }

    template<typename T>
    template<typename Predicate>
    SortedList<T> SortedList<T> :: filter(Predicate predicate) const{
        SortedList<T> updatedList;
        Node* currentNode = Head;
        while(currentNode){
            if(predicate(currentNode->data)){
                updatedList.insert(currentNode->data);
            }
            currentNode = currentNode->next;
        }
        return updatedList;
    }

    template<typename T>
    template<typename Operation>
    SortedList<T> SortedList<T> :: apply(Operation op){
        SortedList<T> updatedNode;
        Node* currentNode = Head;
        while(currentNode){
            T data = op(currentNode->data);
            updatedNode.insert(data);
            currentNode = currentNode->next;
        }
        return updatedNode;
    }

    template<class T>
    typename SortedList<T> :: ConstIterator SortedList<T> :: begin() const{
        return ConstIterator(Head);
    }

    template<class T>
    typename SortedList<T> :: ConstIterator SortedList<T> :: begin() {
        return ConstIterator(Head);
    }

    template<class T>
    typename SortedList<T> :: ConstIterator SortedList<T> :: end() const{
        return ConstIterator(nullptr);
    }

    template<class T>
    typename SortedList<T> :: ConstIterator SortedList<T> :: end() {
        return ConstIterator(nullptr);
    }

    template <class T>
    class SortedList<T>::ConstIterator {
            Node* node;
            ConstIterator(Node* node);
        public:
            ConstIterator() = default;
            ConstIterator(const ConstIterator& other) = default;
            ConstIterator& operator=(const ConstIterator& other) = default;
            ~ConstIterator()=default;

            const T& operator*() const;
            ConstIterator& operator++();
            bool operator!=(const ConstIterator& other) const;

            friend class SortedList;
    /**
     * the class should support the following public interface:
     * if needed, use =defualt / =delete
     *
     * constructors and destructor:
     * 1. a ctor(or ctors) your implementation needs
     * 2. copy constructor
     * 3. operator= - assignment operator
     * 4. ~ConstIterator() - destructor
     *
     * operators:
     * 5. operator* - returns the element the iterator points to
     * 6. operator++ - advances the iterator to the next element
     * 7. operator!= - returns true if the iterator points to a different element
     *
     */
    };
    template<typename T>
    SortedList<T> :: ConstIterator :: ConstIterator(Node* node) : node(node){}

    template<typename T>
    const T& SortedList<T>::ConstIterator::operator*() const {
        if(node == nullptr){
            throw std::range_error("Dereferencing end iterator");
        }
        return node->data;
    }

    template<typename T>
    typename SortedList<T>::ConstIterator& SortedList<T> :: ConstIterator :: operator++(){
        if(node == nullptr){
            throw std::range_error("Adding to end iterator");
        }
        node = node->next;
        return *this;
    }

    template<typename T>
    bool SortedList<T> :: ConstIterator :: operator!=(const ConstIterator& other) const{
        return node != other.node;
    }
}