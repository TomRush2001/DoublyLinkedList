#ifndef LIST_H
#define LIST_H

#include <algorithm>
using namespace std;

template <typename Object>
class List
{
private:
    // The basic doubly linked list node.
    // Nested inside of List, can be public
    // because the Node is itself private
    struct Node
    {
        Object  data;
        Node* prev;
        Node* next;

        Node(const Object& d = Object{ }, Node* p = nullptr, Node* n = nullptr)
            : data{ d }, prev{ p }, next{ n } { }

        Node(Object&& d, Node* p = nullptr, Node* n = nullptr)
            : data{ std::move(d) }, prev{ p }, next{ n } { }
    };

public:
    class const_iterator
    {
    public:

        // Public constructor for const_iterator.
        const_iterator() : current{ nullptr }
        { }

        // Return the object stored at the current position.
        // For const_iterator, this is an accessor with a
        // const reference return type.
        const Object& operator* () const
        {
            return retrieve();
        }

        const_iterator& operator++ ()
        {
            current = current->next;
            return *this;
        }

        const_iterator operator++ (int)
        {
            const_iterator old = *this;
            ++(*this);
            return old;
        }

        const_iterator& operator-- ()
        {
            current = current->prev;
            return *this;
        }

        const_iterator operator-- (int)
        {
            const_iterator old = *this;
            --(*this);
            return old;
        }

        bool operator== (const const_iterator& rhs) const
        {
            return current == rhs.current;
        }

        bool operator!= (const const_iterator& rhs) const
        {
            return !(*this == rhs);
        }

    protected:
        Node* current;

        // Protected helper in const_iterator that returns the object
        // stored at the current position. Can be called by all
        // three versions of operator* without any type conversions.
        Object& retrieve() const
        {
            return current->data;
        }

        // Protected constructor for const_iterator.
        // Expects a pointer that represents the current position.
        const_iterator(Node* p) : current{ p }
        { }

        friend class List<Object>;
    };

    class iterator : public const_iterator
    {
    public:

        // Public constructor for iterator.
        // Calls the base-class constructor.
        // Must be provided because the private constructor
        // is written; otherwise zero-parameter constructor
        // would be disabled.
        iterator()
        { }

        Object& operator* ()
        {
            return const_iterator::retrieve();
        }

        // Return the object stored at the current position.
        // For iterator, there is an accessor with a
        // const reference return type and a mutator with
        // a reference return type. The accessor is shown first.
        const Object& operator* () const
        {
            return const_iterator::operator*();
        }

        iterator& operator++ ()
        {
            this->current = this->current->next;
            return *this;
        }

        iterator operator++ (int)
        {
            iterator old = *this;
            ++(*this);
            return old;
        }

        iterator& operator-- ()
        {
            this->current = this->current->prev;
            return *this;
        }

        iterator operator-- (int)
        {
            iterator old = *this;
            --(*this);
            return old;
        }

    protected:
        // Protected constructor for iterator.
        // Expects the current position.
        iterator(Node* p) : const_iterator{ p }
        { }

        friend class List<Object>;
    };

public:
    List()
    {
        init();
    }

    ~List()
    {
        clear();
        delete head;
        delete tail;
    }


    /*      ONLY TOUCH CODE AFTER THIS LINE      */

    // Return mutable iterator representing beginning of list.
    iterator begin()
    {
        return iterator(head);
    }

    // Return constant iterator representing beginning of list.
    const_iterator begin() const
    {
        return const_iterator(head);
    }

    // Return iterator representing endmarker of list.
    // Mutator version is first, then accessor version.
    iterator end()
    {
        return iterator(tail);
    }

    const_iterator end() const
    {
        return const_iterator(tail);
    }

    // Return number of elements currently in the list.
    int size() const
    {
        return count;
    }

    // Return true if the list is empty, false otherwise.
    bool empty() const
    {
        if (size() == 0) 
        {
            return true;
        }
        else 
        {
            return false;
        }
    }
    

    // Removes all elements from the list
    void clear()
    {
        tail = NULL;
        Node* temp = new Node();

        while (head != NULL) 
        {
            temp = head;
            head = head->next;
            free(temp);
            count = count - 1;
        }
    }

    /* front, back, push_front, push_back, pop_front, and pop_back
       are the basic double-ended queue operations. */
    

    //Returns the value stored in the first element of the list
    const Object& front() const
    {
        const_iterator beginning = begin();
        return *beginning;
    }

    //Returns the value stored in the last element of the list
    const Object& back() const
    {
        const_iterator ending = end();
        return *ending;
    }

    //Inserts an object at the front of the list
    void push_front(const Object& x)
    {
        Node* h = head;
        Node* beginning = new Node{ x, nullptr, head };

        if (head == nullptr) 
        {
            head = beginning;
        }
        else 
        {
            head->prev = beginning;
            head = beginning; 
        }
        if (tail == nullptr && count == 0)
        {
            tail = beginning;
        }
        else if(tail == nullptr)
        {
            tail = h->next;
        }
        count = count + 1;

    }

    //Inserts an object at the back of the list
    void push_back(const Object& x)
    {
        Node* ending = new Node();

        ending->data = x;

        ending->next = NULL;
        ending->prev = NULL;

        if (head == NULL) 
        {
            head = ending;
            tail = ending; 
            count = count + 1;
        }
        else if(head != NULL && tail == NULL)
        {
            tail = ending;
            head->next = tail;
            tail->prev = head;
            count = count + 1;
        }
        else 
        {
            tail->next = ending;
            ending->prev = tail;
            tail = ending;
            count = count + 1;
        }
        
    }

    //Removes the first element in the list
    void pop_front()
    {
        if (head != NULL) 
        {
            Node* temp = new Node();
            
            temp = head;
            head = head->next;

            if (head != NULL) 
            {
               head->prev = NULL;
            }
            
            count = count - 1;
        }
    }

    //Removes the last element in the list
    void pop_back()
    {
        if (tail != NULL) 
        {
            Node* temp = new Node();
            temp = tail;
            tail = tail->prev;
            
            if (tail != NULL) 
            {
                tail->next = NULL;
            }
        }
        
        count = count - 1; 
    }

    // Insert x before itr.
    iterator insert(iterator itr, const Object& x)
    {
        Node* node = new Node();
        Node* temp = itr.current->prev;
        node->data = x;
        node->next = NULL;
        node->prev = NULL;

        itr.current->prev = node;
        node->prev = temp;
        node->next = itr.current;
        temp->next = node;
        itr--;

        return itr;
    }

    // Erase item at itr.
    iterator remove(iterator itr)
    {
        itr.current->prev->next = itr.current->next;
        itr.current->next->prev = itr.current->prev;
        itr.current->next = NULL;
        itr.current->prev = NULL;
        return itr;
    }

    // Return the index of the node containing the matching value
    // Return -1 if no matching value
    int find(const Object& x)
    {
        int index = 0;
        iterator start = begin();
        while (start.current != NULL) 
        {
            if (*start == x) 
            {
                return index;
            }
            else 
            {
                index = index + 1;
                start++;
            }
        }
        return -1;
    }

private:
    Node* head;
    Node* tail;
    int count = 0;

    void init()
    {
    }
};

#endif
