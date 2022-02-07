#pragma once
template <typename Object>
class Queue
{
public:
    bool isEmpty() const
    {
        return theList.empty();
    }
    const Object& getFront() const
    {
        return theList.front();
    }
    void enqueue(const Object& x)
    {
        theList.push_back(x);
    }
    Object& dequeue()
    {
        Object& first = *theList.begin();
        theList.pop_front();
        return first;
    }
private:
    List<Object> theList;
};
