/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */


template <class T>
Deque<T>::Deque(){
    n1 = 0;
    n2 = -1;
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    n2++;
    data.push_back(newItem);
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    T item = data[n1];
    n1++;
   
    if (n1 >= (n2 - n1) + 1) {
        vector<T> tempArray;
        for (int i1 = n1 ; i1 <= n2; i1++) {
            tempArray.push_back(data[i1]);
        }
        data.clear();
        for (unsigned int i1 = 0 ; i1 < tempArray.size(); i1++) {
            data.push_back(tempArray[i1]);
        }
        n1 = 0;
        n2 = data.size() - 1;
    }
    
    return item;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    T item = data[n2];
    data.pop_back();
    n2--;
    
    if (n1 >= (n2 - n1) + 1) {
        vector<T> tempArray;
        for (int i1 = n1 ; i1 <= n2; i1++) {
            tempArray.push_back(data[i1]);
        }
        data.clear();
        for (unsigned int i1 = 0 ; i1 < tempArray.size(); i1++) {
            data.push_back(tempArray[i1]);
        }
        n1 = 0;
        n2 = data.size() - 1;
    }
    
    return item;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data[n1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
   return data[n2];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return n2 < n1;
}
