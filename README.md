05_Hashing_Lab
==============

Implement a hash table in C++

Requirements
------------

1. `keyExists`, `find`, `remove`, and `size` should all be O(1)
2. `add` should be reasonably fast. Use linear probing to find an open slot in the hash table. This will be O(1), on average, except when `grow` is called.
3. `grow` should be O(n)
4. Do not leak memory


Reading
=======
"Open Data Structures," Chapter 5, except for 5.2.3. http://opendatastructures.org/ods-cpp/5_Hash_Tables.html

Questions
=========

#### 1. Which of the above requirements work, and which do not? For each requirement, write a brief response.

1. SUCCESS - All of these methods run in constant time
2. SUCCESS - The method runs reasonably fast because of the load factor/linear probing
3. SUCCESS - grow() is O(n)
4. SUCCESS - As far as I can tell, memory is not leaked

#### 2. I decided to use two function (`keyExists` and `find`) to enable lookup of keys. Another option would have been to have `find` return a `T*`, which would be `NULL` if an item with matching key is not found. Which design do you think would be better? Explain your reasoning. You may notice that the designers of C++ made the same decision I did when they designed http://www.cplusplus.com/reference/unordered_map/unordered_map/
	- I think returning a T* variable in find() would slightly increase the runtime because you have to create a new object, and that would take up space.  C++ is all about efficiency, and so I think your decision to just look up the key and return if it exists is more efficient for our purposes since we don't need to immediately do the operations with a T* object.

#### 3. What is one question that confused you about this exercise, or one piece of advice you would share with students next semester?
How much does load factor affect the run time?
