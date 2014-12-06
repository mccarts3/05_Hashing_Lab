//DO NOT CHANGE THIS FILE
//Author: Bo Brinkman
//Date: 2013/07/24
#include "USet.h"

/*
* Note: Just above your template declaration when you use this class, you
* must define method called "hash" that takes a Key as input, and returns
* an unsigned long (which is the hash value)
*
* For example, you might do:
* unsigned long hash(char c){ return 10*((unsigned long)c)%backingArraySize; }
* HashTable<char,int> mySillyTable;
*
* If you don't define an appropriate hash function, the class won't compile.
*/

template <class Key, class T>
class HashTable : public USet <Key, T> {
private:
	class HashRecord {
	public:
		Key k;
		T x;

		//If the slot in the hash table is totally empty, set this to true.
		bool isNull;

		//If the slot used to have something in it, but doesn't now, set
		// isDel to true, and isNull to false. isNull is only for slots
		// that have never been used
		bool isDel;

		HashRecord() { isNull = true; isDel = false; };
	};

public:
	//See USet.h for documentation of these methods
	virtual unsigned long size();
	virtual void add(Key k, T x);
	virtual void remove(Key k);
	virtual T find(Key k);
	virtual bool keyExists(Key k);

	//Initialize all private member variables.
	HashTable();
	//Delete any dynamically allocated memory.
	virtual ~HashTable();

private:
	//A pointer to the array that holds the hash table data
	HashRecord* backingArray;

	//Whenever numItems + numRemoved >= backingArraySize/2, call
	// grow(). grow() should make a new backing array that is twice the
	// size of the old one, similar to what we did in the ArrayQueue
	// lab.
	//Note: You cannot just loop through the old array and copy it to the
	// new one! Since the backing array size has changed, each item will likely
	// map to a different slot in the array. You may just want to use add()
	// after initializing the new array.
	void grow();

	//This helper method should take a key, and return the index for that
	// item within the hash table. If the item already exists, return the
	// index of the existing item. If the item doesn't exist, return the index
	// where it OUGHT to be. This function can then be used as a helper method in
	// your other methods.
	unsigned long calcIndex(Key k);

	unsigned long numItems; //Number of items in the hash table
	unsigned long primeCounter; //Keeps track of which prime # backingArraySize is

	//Note: Ordinarily, these OUGHT to be private. In this case I have
	// made them public for easy of testing.
public:
	unsigned long numRemoved; //Number of slots that have been removed but not re-used. Those that have isDel == true
	unsigned long backingArraySize;
};

/***************************
* Previously HashTable.ipp *
***************************/

//You will need this so you can make a string to throw in
//remove
#include <string>

template <class Key, class T>
HashTable<Key,T>::HashTable(){
	numItems=0;
	numRemoved=0;
	primeCounter=0;
	backingArraySize = hashPrimes[primeCounter];
	backingArray = new HashRecord[backingArraySize];
}

template <class Key, class T>
HashTable<Key,T>::~HashTable() {
    delete[] backingArray;
}

template <class Key, class T>
unsigned long HashTable<Key,T>::calcIndex(Key k){	
	//Use the hash function and then linear probe until a space is allocated
    //for use by a new key
    unsigned long index = hash(k)%backingArraySize;
    unsigned long indexStart = index;
    
    do {
        if(backingArray[index].isNull || backingArray[index].k == k) {
            return index;
        }
        index = (++index)%backingArraySize;
    } while(index != indexStart);
    
    return indexStart;
}

template <class Key, class T>
void HashTable<Key,T>::add(Key k, T x){
    if((numItems+numRemoved) >= backingArraySize/2) {
		grow();
	}
    
    unsigned long index = calcIndex(k);

    
	if(backingArray[index].isDel || backingArray[index].isNull) {
        backingArray[index].k = k;
        backingArray[index].x = x;
        numItems++;
    }
    else if(numItems < backingArraySize) {
        while(!backingArray[index].isDel && !backingArray[index].isNull) {
            index = (++index)%backingArraySize;
            if(backingArray[index].isDel) {
                backingArray[index].k = k;
                backingArray[index].x = x;
                numRemoved--;
            }
            else if(backingArray[index].isNull) {
                backingArray[index].k = k;
                backingArray[index].x = x;
            }
            else if(backingArray[index].k == k) {
                backingArray[index].x = x;
            }
        }
        numItems++;
    }
}

template <class Key, class T>
void HashTable<Key,T>::remove(Key k){
	unsigned long index = calcIndex(k);
    
    if(numItems == 0) {
        throw std::string("In remove(), attempted to remove from an empty hash table.");
    }
    if(keyExists(k)) {
        backingArray[index].isDel = true;
        numItems--;
        numRemoved++;
    }
}

template <class Key, class T>
T HashTable<Key,T>::find(Key k){
	//T dummy;
	//return dummy;
    
    if(keyExists(k)) {
        return backingArray[calcIndex(k)].x;
    }
    else {
        throw std::string("In find(), Key parameter does not exist in hash table.");
    }
}

template <class Key, class T>
bool HashTable<Key,T>::keyExists(Key k){
	unsigned long index = calcIndex(k);
    
    if(backingArray[index].k == k && !backingArray[index].isDel &&
       backingArray[index].isNull) {
        return true;
    }
    else if(!backingArray[index].isDel || !backingArray[index].isNull) {
        while(!backingArray[index].isDel || !backingArray[index].isNull) {
            ++index;
            if(backingArray[index].k == k) { return true; }
        }
    }
    
    return false;
}

template <class Key, class T>
unsigned long HashTable<Key,T>::size(){
	return numItems;
}

template <class Key, class T>
void HashTable<Key,T>::grow(){
    int currSize = backingArraySize;
    backingArraySize = hashPrimes[++primeCounter];
	HashRecord* tempArray = backingArray;
    backingArray = new HashRecord[backingArraySize];
    
    for(int index = 0; index < currSize; index++) {
        if(!tempArray[index].isNull && !tempArray[index].isDel) {
            add(tempArray[index].k, tempArray[index].x);
        }
    }

	delete[] tempArray;
}

