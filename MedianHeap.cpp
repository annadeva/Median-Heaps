/*
    Name:    Anna Devadas
    UserId:  UY38419
    Course:  CMSC341, Sec 01
    Project: Project 4
    File:    MedianHeap.cpp
*/

#ifndef _MEDIANHEAP_CPP_
#define _MEDIANHEAP_CPP_

#include <iostream>
#include <stdexcept>
#include "MedianHeap.h"

using namespace std;

//**************************** Heap Class *********************************

// heap class constructor
// dynamically creates array of specified capacity and assigns member variables
template <typename T>
Heap<T>::Heap(int cap, bool (*cmp) (const T&, const T&)) {
    m_heap = new T[cap];    // creates array for heap
    m_heapCap = cap;
    m_heapSize = 0;
    compare = cmp;
}

// heap class copy constructor
template <typename T>
Heap<T>::Heap(const Heap<T>& other) {
    // initializes member variables to same values as other
    m_heapCap = other.m_heapCap;
    m_heapSize = other.m_heapSize;
    compare = other.compare;

    // allocates memory for new heap
    m_heap = new T[m_heapCap];
    // copies values from other heap to new heap
    for (int i=1; i <= m_heapSize; i++){
        m_heap[i] = other.m_heap[i];
    }
}

// heap class destructor
// deletes dynamically allocated array 
template <typename T>
Heap<T>::~Heap() {
    m_heapCap = 0;
    m_heapSize = 0;
    delete[] m_heap;
    m_heap = NULL;
}

// heap class overloaded assignment operator
template <typename T>
const Heap<T>& Heap<T>::operator=(const Heap<T>& rhs) {
    // checks first for self-assignment, if true returns object
    if(this == &rhs){
        return *this;
    }

    delete[] m_heap;
    // initializes member variables to same values as rhs
    m_heapCap = rhs.m_heapCap;
    m_heapSize = rhs.m_heapSize;
    compare = rhs.compare;

    // allocates memory for lhs heap
    m_heap = new T[m_heapCap];
    // copies values from rhs heap to lhs heap
    for (int i=1; i <= m_heapSize; i++){
        m_heap[i] = rhs.m_heap[i];
    }

    return *this;
}

// inserts passed item at the last position in the heap it is in 
// the right position and bubbles up if it's not
template <typename T>
void Heap<T>::insert(const T& item) {
    // if heap is full throw error
    if(m_heapSize == m_heapCap){
        throw out_of_range("Could not insert item. Heap is full.");
    }
    // increase size of heap and add item to index
    else {
        m_heapSize++;
        m_heap[m_heapSize] = item;
        // call bubbleUp to check if item is in correct position
        bubbleUp(m_heapSize);
    }
}

// checks if inserted item is in correct position and if not, bubbles up
template <typename T>
void Heap<T>::bubbleUp(int pos) {
    int pIndex; // index of parent node
    // if not the root of the heap
    if(pos != 1){
        // find parent index
        pIndex = parent(pos);
        // if node violates heap condition, swap with parent node
        if(compare(m_heap[pos], m_heap[pIndex])){
            swap(pos, pIndex);
            bubbleUp(pIndex);
        }
    }
}

// removes item from heap at the specified position
template <typename T> 
void Heap<T>::deleteH(int pos) {
    // if position specified is at end of array
    if(pos == m_heapSize){
        m_heapSize--;
    }
    // if position specified is the root of heap
    else if(pos == 1){
        // root set equal to last item in heap
        m_heap[pos] = m_heap[m_heapSize];
        // heap size decremented then trickleDown is called
        m_heapSize--;
        trickleDown(pos);
    }
    // other position specified
    else {
        m_heap[pos] = m_heap[m_heapSize];
        m_heapSize--;
        // if violates heap condition with parent, bubbleUp
        if(compare(m_heap[pos], m_heap[parent(pos)]) ) {
            bubbleUp(pos);
        }
        // else heck trickle down
        else{ trickleDown(pos); }
    }
}

// checks if item is in correct position, and if not trickles down
template <typename T> 
void Heap<T>::trickleDown(int pos) {
    // determine indices of children
    int l = left(pos);
    int r = right(pos);
    int x = pos;    // will hold swap index

    // if left child is in violation with item and condition
    if(l <= m_heapSize && compare(m_heap[l], m_heap[pos])){
        x = l;
    }
    // if right child is either greater than or less than left
    if(r <= m_heapSize && compare(m_heap[r], m_heap[x])) {
        x = r;
    }
    // if swap index was changed from pos, swap
    if (x != pos){
        swap(pos, x);
        trickleDown(x);
    }
}

// swaps the two passed in items with one another
template <typename T>
void Heap<T>::swap(int pos1, int pos2) {
    T temp; // creates temporary variable to hold item 1
    temp = m_heap[pos1];
    // switches the positions of items
    m_heap[pos1] = m_heap[pos2];
    m_heap[pos2] = temp;
}

//********************** MedianHeap Class *********************************

// constructor for MedianHeap class
// must create a MedianHeap object capable of holding cap items
template <typename T>
MedianHeap<T>::MedianHeap( bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), int cap) {
    // assign capacity
    m_capacity = cap;
    // create two new Heap objects
    maxHeap = new Heap<T>((cap/2) + 2, gt);
    minHeap = new Heap<T>((cap/2) + 2, lt);

    less = lt;
    greater = gt;
}

// MedianHeap class copy constructor
// creates a deep copy of the passed in MedianHeap object
template <typename T>
MedianHeap<T>::MedianHeap(const MedianHeap<T>& otherH) {
    // intializes member variables with same values as otherH
    m_capacity = otherH.m_capacity;
    m_max = otherH.m_max;
    m_min = otherH.m_min;

    // creates new max and min heap objects using Heap copy constructor
    maxHeap = new Heap<T>(otherH.maxHeap);
    minHeap = new Heap<T>(otherH.minHeap);

    less = otherH.less;
    greater = otherH.greater;
}

// MedianHeap class destructor
// deallocates any dynamically allocated memory
template <typename T>
MedianHeap<T>::~MedianHeap() {
    delete maxHeap;
    maxHeap = NULL;
    delete minHeap;
    minHeap = NULL;

    m_capacity = 0;
}

// MedianHeap class overloaded assignment operator
// deallocates memory of the host object and copies rhs into host
template <typename T>
const MedianHeap<T>& MedianHeap<T>::operator=(const MedianHeap<T>& rhs) {
    // checks first for self-assignment, if true returns object
    if(this == &rhs){
        return *this;
    }
   
    // changes member variables to the values of rhs
    m_capacity = rhs.m_capacity;
    m_max = rhs.m_max;
    m_min = rhs.m_min;

    // delete max and min heaps
    delete maxHeap;
    delete minHeap;
    // uses Heap copy constructor 
    maxHeap = new Heap<T>(rhs.maxHeap);
    minHeap = new Heap<T>(rhs.minHeap);

    less = rhs.less;
    greater = rhs.greater;
}

// returns the total number of items in the MedianHeap
template <typename T>
int MedianHeap<T>::size() {
    return (minHeap->m_heapSize + maxHeap->m_heapSize);
}

// returns the maximum number of items that can be stored in the MedianHeap
template <typename T>
int MedianHeap<T>::capacity() {
    return m_capacity;
}

template <typename T>
void MedianHeap<T>::insert(const T& item) {
    // if MedianHeap is full throw out of range error
    if(size() == capacity()){
        throw out_of_range("The MedianHeap is full. Cannot insert item.");
    }
    // if MedianHeap is empty
    if(size() == 0){
        // insert item in minHeap, set min and max equal to it
        minHeap->insert(item);
        m_min = item;
        m_max = item;
    }
    // if there is one item in MedianHeap
    else if(size() == 1){
        // if the item is less than median
        if(less(item, getMedian())){
            // insert into the max Heap and change min
            maxHeap->insert(item);
            m_min = item;
        }
        // if the item is greater than the median
        else{
            // insert the curr median into the maxHeap, and delete it from minHeap
            maxHeap->insert(minHeap->m_heap[1]);
            minHeap->deleteH(1);
            // insert item into minHeap and change max
            minHeap->insert(item);
            m_max = item;
        }
    }
    // more than one item in MedianHeap
    else {
        // if max heap size is greater than min heap size by more than one
        if(maxHeapSize() > minHeapSize() + 1) {
            // insert root of maxHeap into minHeap
            minHeap->insert(maxHeap->m_heap[1]);
            // delete root from maxHeap
            maxHeap->deleteH(1);
        }
        // if min heap size is greater by more than one
        else if (minHeapSize() > maxHeapSize() + 1) {
            // insert root of minHeap into maxHeap
            maxHeap->insert(minHeap->m_heap[1]);
            // delete root from minHeap
            maxHeap->deleteH(1);
        }
        // if the item is less than median
        if(less(item, getMedian())){
            // insert into the maxHeap
            maxHeap->insert(item);
            // check if min needs to be changed
            if(less(item, m_min)) {m_min = item;}
        } 
        // if item is greater than median insert in min heap
        else {
            minHeap->insert(item);
            // check if max needs to be changed
            if(greater(item, m_max)) {m_max = item;}
        }
    }
}

// returns a copy of the median key object
template <typename T>
T MedianHeap<T>::getMedian() {
    T median;
    if(size() == 0){
        throw out_of_range("The MedianHeap is empty.");
    }
    if (minHeapSize() > maxHeapSize()){
        median = minHeap->m_heap[1];
    }
    else {
        median = maxHeap->m_heap[1];
    }
}

// returns a copy of the min key object
template <typename T>
T MedianHeap<T>::getMin() {
    return m_min;
}

// returns a copy of the max key object
template <typename T>
T MedianHeap<T>::getMax() {
    return m_max;
}

// looks for givenItem in MedianHeap and if found deletes item and returns true
// if unfound, MedianHeap is unchanged and returns false
template <typename T>
bool MedianHeap<T>::deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) ) {
    // if the MedianHeap is empty throw out of range error
    if(size() == 0) {
        throw out_of_range("The heap is empty, cannot remove item.");
    }
    // create found boolean and index tracker
    bool found = false;
    int i = 1;
    // runs while item remains unfound in maxHeap
    while(found == false && i <= maxHeapSize()){
        // if item in heap index is equal to givenItem
        if (equalTo(maxHeap->m_heap[i], givenItem)){
            // copy item into givenItem and then delete
            givenItem = maxHeap->m_heap[i];
            maxHeap->deleteH(i);
            found = true;

            // if deleted item was min find new min
            if (equalTo(givenItem, m_min)){
                findMin();
            }
            // if deleted item was max find new max
            else if (equalTo(givenItem, m_max)){
                findMax();
            }
            return found;
        }
        i++;
    }
    // if unfound in maxHeap look in minHeap
    i = 1;
    while(found == false && i <= minHeapSize()){
        // if item in heap index is equal to givenItem
        if (equalTo(minHeap->m_heap[i], givenItem)){
            // copy item into givenItem and then delete
            givenItem = minHeap->m_heap[i];
            minHeap->deleteH(i);
            found = true;

            // if deleted item was min find new min
            if (equalTo(givenItem, m_min)){
                findMin();
            }
            // if deleted item was max find new max
            else if (equalTo(givenItem, m_max)){
                findMax();
            }
            return found;
        }
        i++;   
    }
    // if unfound
    return false;
}

// called when min is deleted, finds the new min
template <typename T>
void MedianHeap<T>::findMin() {
    // create temp var set it equal to root of maxHeap
    T temp = maxHeap->m_heap[1];
    // iterate through maxHeap and find smallest value
    for(int i=1; i <= maxHeapSize(); i++){
        // if current item is less than temp, reassign temp
        if(less(maxHeap->m_heap[i], temp) ){
            temp = maxHeap->m_heap[i];
        }
    }
    m_min = temp; // assign min to tmep val
}

// called when max is deleted, finds the new max
template <typename T>
void MedianHeap<T>::findMax() {
    // create temp var set it equal to root of minHeap
    T temp = minHeap->m_heap[1];
    // iterate through minHeap and find largest value
    for(int i=1; i <= minHeapSize(); i++){
        // if current item is less than temp, reassign temp
        if(greater(minHeap->m_heap[i], temp) ){
            temp = minHeap->m_heap[i];
        }
    }
    m_max = temp; // assign max to tmep val
}

// prints out max and min heap data in proper format
template <typename T>
void MedianHeap<T>::dump() {
    cout << "... MedianHeap()::dump() ..." << endl;

    // prints max heap data
    cout << "------------Max Heap------------" << endl;
    cout << "size = " << maxHeap->m_heapSize << ", ";
    cout << "capacity = " << maxHeap->m_heapCap << endl;
    for (int i=1; i <= maxHeap->m_heapCap; i++){
        cout << "Heap[" << i << "] = " << maxHeap->m_heap[i] << endl;
    }

    // prints min heap data
    cout << "------------Min Heap------------" << endl;
    cout << "size = " << minHeap->m_heapSize << ", ";
    cout << "capacity = " << minHeap->m_heapCap << endl;
    for (int i=1; i <= minHeap->m_heapCap; i++){
        cout << "Heap[" << i << "] = " << minHeap->m_heap[i] << endl;
    }

    cout << "--------------------------------" << endl;
}

// returns the number of items in the max heap
template <typename T>
int MedianHeap<T>::maxHeapSize() {
    return maxHeap->m_heapSize;
}

// returns the number of items in the min heap
template <typename T>
int MedianHeap<T>::minHeapSize() {
    return minHeap->m_heapSize;
}

// returns a copy of the item in position pos in the max heap 
template <typename T>
T MedianHeap<T>::locateInMaxHeap(int pos) {
    // if pos is invalid, throw error
    if(pos < 1 || pos > maxHeapSize()){
        throw out_of_range("Position specified is invalid or out of range.");
    }
    // create copy of item in pos, and return
    T item;
    item = maxHeap->m_heap[pos];

    return item;
}

// returns a copy of the item in position pos in the min heap
template <typename T>
T MedianHeap<T>::locateInMinHeap(int pos) {
    // if pos is invalid, throw error
    if(pos < 1 || pos > minHeapSize()){
        throw out_of_range("Position specified is invalid or out of range.");
    }
    // create copy of item in pos, and return
    T item;
    item = minHeap->m_heap[pos];

    return item;
}

#endif