# Handle System

## Introduction

This code demonstrates a simple implementation of a data handle system for an operating system. The purpose is to disassociate data from pointers to that data, which helps decouple code and prevent issues in situations where pointers may not work such as in network applications.

When desigining this system, here are my following order of priorities:
1. The system is able to handle an arbitrary number of handles (within the confines of the size of basic C variables)
   - We do not know how many handles the system may need, and if we run out of handles then the system is effectively useless. Thus we must support any number of handles.
2. The system works with any data type
   - If there is a data type that is not supported then the handle system also becomes useless. Thus, we must support any data type to ensure it is always applicable
3. Fast time to access the data bound to a handle with regard to large numbers of handles allocated at any one time
   - Accessing data bound to a handle (read or write) will likely be the most common operation done on handles and thus should be as fast as possible
4. Fast time to bind and free data attached to a handle
   - Binding and re-binding handles should be the second most commmon operation (if you allocate/free you have to do this anyways) and thus these are second highest priority for runtime
5. Fast time to allocate and free handles from being used in the system
   - Allocation and freeing of handles is most likely the least common operation used, as these will usually only appear in startup and shutdown sequences. These are lowest priority operations.
6. Robust API against potential misuse
   - The system should be robust to illegal operations such as using an illegal handle to access data, and the system should continue functioning normally and notify the user if possible
7. Minimize memory usage of the system
   - The system should minimize the amount of data needed for bookkeeping in order to maximize memory available to the rest of the system

## The Handle System Data Structure

The data structure used for the Handle System is a cross between a C++ style vector and a linked list. It functions as follows:

Similar to a linked list, a node of the data structure contains a data field as well as a "pointer" to the next node in the structure (Note that the `data` field is actually a void pointer to handle arbitrary data types, and the `next` field is simply the next free handle).
```
struct handle_node {
    HANDLE next_free_handle; // HANDLE type is really an unsigned int
    void * data;
};
```

However in memory, the data more closely resembles a C++ vector, which consists of an array of size `n`. This array size is stored and used to expand the array when needed. Each slot in the array consists of a `handle_node` initialized to the following, where `HANDLE_NULL` is the maximum storable `HANDLE` value and used as a sentinel.

```
FIRST      +------+------+------+------+-----+-------------+
+---+      | 1    | 2    | 3    | 4    | ... | HANDLE_NULL |
| 0 |      | NULL | NULL | NULL | NULL | ... | NULL        |
+---+      +------+------+------+------+-----+-------------+
              0       1      2       3              n-1
```

Also similar to a linked list however, we separately store a pointer to the first available handle in the list.

#### Allocation
To allocate a new handle to be used, we use the value stored in FIRST (`handle_first_available` in the code). That slot is allocated away, and we use the `next` field in that slot to update FIRST for the next allocation.

For simplicity, we remove the data fields in the following diagram:

```
        Allocated Slot 0
+---+       +---+---+---+---+-----+-------------+
| 1 |       |///| 2 | 3 | 4 | ... | HANDLE_NULL |
+---+       +---+---+---+---+-----+-------------+

              0   1   2   3         n-1

        Allocate Slot 1
+---+       +---+---+---+---+-----+-------------+
| 2 |       |///|///| 3 | 4 | ... | HANDLE_NULL |
+---+       +---+---+---+---+-----+-------------+

              0   1   2   3         n-1
```

#### Deallocation
When a handle is freed and available for reuse, we set its `next` pointer to FIRST and then update FIRST to point to the newly freed node.

```
Free item 0
+---+       +---+---+---+---+-----+-------------+
| 0 |       | 2 |///| 3 | 4 | ... | HANDLE_NULL |
+---+       +---+---+---+---+-----+-------------+

              0   1   2   3         n-1

Free item 1
+---+       +---+---+---+---+-----+-------------+
| 2 |       | 2 | 0 | 3 | 4 | ... | HANDLE_NULL |
+---+       +---+---+---+---+-----+-------------+

              0   1   2   3         n-1
```

#### Resizing
Naturally the above array is of a fixed length. To accomodate arbitrary numbers of handles, once we have allocated the last slot in the array (so `n` handles are in use at once and FIRST points to `HANDLE_NULL`) we need to allocate more. Then similar to a C++ vector, we allocate a new array of twice the size, copy over the data from the existing array and the free it. We then have sufficient space to allocate more handles (assuming we don't run out of memory).

### Why this structure?

This style of combined vector and linked list enables the system to best match up to my order of priorities outlined in the introduction. 

- Due to array resizing, it can handle an arbitrary number of handles
- Since each node uses a void pointer to store data, any data type can be stored with a handle
- Since the data in memory is stored in an array structure, access time is O(1), which is significantly better than alternatives such as a traditional linked list (O(n)) or a binary tree (O(log n)).
- Since the lookup time for any handle is O(1), consequently so is the time to bind or release data from a corresponding handle.
- Time to free a handle from use is also O(1), which matches the best case from other structures such as a linked list.
- Time to allocate a new handle is amortized O(1), as in it averages to O(1) but could potentially take much longer if resizing is needed. This is worse than something like a linked list (which is always O(1)), but since this is lower on the priority list this type of behaviour is preferred due to the increase in access time for a linked list.
-  A struct was chosen for `handle_node` as that allows the system to more robust to failure if illegal handles are used. A `union` can be used instead to save on memory usage in the data structure, however that was deemed a lower priority.
-  Generally speaking there will be a moderate amount of wasted space from this data structure due to unallocated handles still requiring space in the data structure (compare to a standard linked list where no extra nodes needed to be pre-allocated). However as space was deemed the lowest priority this sacrifice was made for faster runtimes.

Essentially, this structure combines the best parts of a linked list and a vector for best overall performance.

## Usage and Testing

The handle API is implemented in `handle.h` and `handle.c`. Prior to using it you must call  `handle_init()` and before exiting the program you must call `handle_cleanup()` in order to allocate and deallocate the handle data structure.

The usage of each function is documented in `handle.h` itself. These are
- `handle_alloc` : allocate new handle
- `handle_free` : free handle for reuse
- `handle_bind` : bind data to handle
- `handle_reset` : reset so no data attached to a handle
- `handle_get` : get data attached to a handle

An example of how to use the handle system to implement OS file opening is demonstrated in `my_os.h` and `my_os.c`. These functions use the handle API to create file handles, read data from them, and then free those handles when done.

Some basic tests of the system are present in `main.c`. To build and run:

```
make && ./main
```

