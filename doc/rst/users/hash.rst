kvtree 
====

Overview
--------

This document describes the 'kvtree` object. This data
structure contains an unordered list of elements, where each element
contains a key (a string) and a value (another kvtree). Each element in a
kvtree has a unique key. Using the key, one can get, set, and unset
elements in a kvtree. There are functions to iterate through the elements
of a kvtree. There are also functions to pack and unpack a kvtree into a
memory buffer, which enables one to transfer a kvtree through the network
or store the kvtree to a file.

Throughout the documentation and comments in the source code, a kvtree is
often displayed as a tree structure. The key belonging to a kvtree element
is shown as a parent node, and the elements in the kvtree belonging to
that element are displayed as children of that node. For example,
consider the following tree::
     +- RANK 
       +- 0 
       |  +- FILES
       |  |  +- 2
       |  +- FILE 
       |     +- foo_0.txt 
       |     |  +- SIZE 
       |     |  |  +- 1024 
       |     |  +- COMPLETE 
       |     |     +- 1 
       |     +- bar_0.txt 
       |        +- SIZE 
       |        |  +- 2048 
       |        +- COMPLETE 
       |           +- 1 
       +- 1 
          +- FILES 
          |  +- 1 
          +- FILE
             +- foo_1.txt 
                +- SIZE 
                |  +- 3072 
                +- COMPLETE 
                   +- 1 

The above example represents a kvtree that contains a single element with
key `RANK`. The kvtree associated with the `RANK` element contains two
elements with keys `0` and `1`. The kvtree associated with the `0` element
contains two elements with keys `FILES` and `FILE`. The `FILES` element,
in turn, contains a kvtree with a single element with a key `2`, which
finally contains a kvtree having no elements.

Often when displaying these trees, the guidelines are not shown and only
the indentation is used, like so::
     RANK
       0
         FILES
           2
         FILE
           foo_0.txt
             SIZE
               1024
             COMPLETE
               1
           bar_0.txt
               SIZE
                 2048
               COMPLETE
                 1
       1
         FILES
           1
         FILE
           foo_1.txt
             SIZE
               3072
             COMPLETE
               1

Common functions
----------------

This section lists the most common functions used when dealing with
kvtrees. For a full listing, see `kvtree.h`.

KVTree basics
++++++++++++

First, before using a kvtree, one must allocate a kvtree object.::

      kvtree* kvtree = kvtree_new();

And one must free the kvtree when done with it.::

      kvtree_delete(&kvtree);

Given a kvtree object, you may insert an element, specifying a key and
another kvtree as a value.::

      kvtree_set(kvtree, key, value_kvtree);

If an element already exists for the specified key, this function
deletes the value currently associated with the key and assigns the
specified kvtree as the new value. Thus it is not necessary to unset a key
before setting it -- setting a key simply overwrites the existing value.

You may also perform a lookup by specifying a key and the kvtree object to
be searched.::

      kvtree* value_kvtree = kvtree_get(kvtree, key);

If the kvtree has a key by that name, it returns a pointer to the kvtree
associated with the key. If the kvtree does not have an element with the
specified key, it returns NULL.

You can unset a key.::

      kvtree_unset(kvtree, key);

If a kvtree value is associated with the specified key, it is freed, and
then the element is deleted from the kvtree. It is OK to unset a key even
if it does not exist in the kvtree.

To clear a kvtree (unsets all elements).::

      kvtree_unset_all(kvtree);

To determine the number of keys in a kvtree.::

      int num_elements = kvtree_size(kvtree);

To simplify coding, most kvtree functions accept NULL as a valid input
kvtree parameter. It is interpreted as an empty kvtree. For example,::

  ---------------------------------------- -------------------------------
  `kvtree_delete(NULL);`                 does nothing
  `kvtree_set(NULL, key, value_kvtree);`   does nothing and returns NULL
  `kvtree_get(NULL, key);`               returns NULL
  `kvtree_unset(NULL, key);`             does nothing
  `kvtree_unset_all(NULL);`              does nothing
  `kvtree_size(NULL);`                   returns 0
  ---------------------------------------- -------------------------------

Accessing and iterating over kvtree elements
##########################################

At times, one needs to work with individual kvtree elements. To get a
pointer to the element associated with a key (instead of a pointer to
the kvtree belonging to that element).::

      kvtree_elem* elem = kvtree_elem_get(kvtree, key);

To get the key associated with an element.::

      char* key = kvtree_elem_key(elem);

To get the kvtree associated with an element.::

      kvtree* kvtree = kvtree_elem_kvtree(elem);

It's possible to iterate through the elements of a kvtree. First, you need
to get a pointer to the first element.::

      kvtree_elem* elem = kvtree_elem_first(kvtree);

This function returns NULL if the kvtree has no elements. Then, to advance
from one element to the next.::

      kvtree_elem* next_elem = kvtree_elem_next(elem);

This function returns NULL when the current element is the last element.
Below is some example code that iterates through the elements of kvtree
and prints the key for each element::

      kvtree_elem* elem;
      for (elem = kvtree_elem_first(kvtree);
           elem != NULL;
           elem = kvtree_elem_next(elem))
      {
        char* key = kvtree_elem_key(elem);
        printf("%s\n", key);
      }

Key/value convenience functions
###############################

Often, it's useful to store a kvtree using two keys which act like a
key/value pair. For example, a kvtree may contain an element with key
`RANK`, whose kvtree contains a set of elements with keys corresponding to
rank ids, where each rank id `0`, `1`, `2`, etc. has a kvtree, like so::

      RANK
        0
          <kvtree for rank 0>
        1
          <kvtree for rank 1>
        2
          <kvtree for rank 2>

This case comes up so frequently that there are special key/value (\_kv)
functions to make this operation easier. For example, to access the kvtree
for rank 0 in the above example, one may call::

      kvtree* rank_0_kvtree = kvtree_get_kv(kvtree, "RANK", "0");

This searches for the `RANK` element in the specified kvtree. If found, it
then searches for the `0` element in the kvtree of the `RANK` element. If
found, it returns the kvtree associated with the `0` element. If kvtree is
NULL, or if kvtree has no `RANK` element, or if the `RANK` kvtree has no `0`
element, this function returns NULL.

The following function behaves similarly to `kvtree_get_kv` -- it
returns the kvtree for rank 0 if it exists. It differs in that it creates
and inserts kvtreees and elements as needed such that an empty kvtree is
created for rank 0 if it does not already exist.::

      kvtree* rank_0_kvtree = kvtree_set_kv(kvtree, "RANK", "0");

This function creates a `RANK` element if it does not exist in the
specified kvtree, and it creates a `0` element in the `RANK` kvtree if it
does not exist. It returns the kvtree associated with the `0` element,
which will be an empty kvtree if the `0` element was created by the call.
This feature lets one string together multiple calls without requiring
lots of conditional code to check whether certain elements already
exist. For example, the following code is valid whether or not `kvtree`
has a `RANK` element.::

      kvtree* rank_kvtree = kvtree_set_kv(kvtree,      "RANK", "0");
      kvtree* ckpt_kvtree = kvtree_set_kv(rank_kvtree, "CKPT", "10");
      kvtree* file_kvtree = kvtree_set_kv(ckpt_kvtree, "FILE", "3");

Often, as in the case above, the *value* key is an integer. In order to
avoid requiring the caller to convert integers to strings, there are
functions to handle the value argument as an `int` type, e.g, the above
segment could be written as::

      kvtree* rank_kvtree = kvtree_set_kv_int(kvtree,      "RANK",  0);
      kvtree* ckpt_kvtree = kvtree_set_kv_int(rank_kvtree, "CKPT", 10);
      kvtree* file_kvtree = kvtree_set_kv_int(ckpt_kvtree, "FILE",  3);

It's also possible to unset key/value pairs.::

      kvtree_unset_kv(kvtree, "RANK", "0");

This call removes the `0` element from the `RANK` kvtree if one exists. If
this action causes the `RANK` kvtree to be empty, it also removes the
`RANK` element from the specified input kvtree.

In some cases, one wants to associate a single value with a given key.
When attempting to change the value in such cases, it is necessary to
first unset a key before setting the new value. Simply setting a new
value will insert another element under the key. For instance, consider
that one starts with the following kvtree::

      TIMESTEP
        20

If the goal is to modify this kvtree such that it changes to::

      TIMESTEP
        21

then one should do the following::

      kvtree_unset(kvtree, "TIMESTEP");
      kvtree_set_kv_int(kvtree, "TIMESTEP", 21);

Simply executing the set operation without first executing the unset
operation results in the following::

      TIMESTEP
        20
        21

Because it is common to have fields in a kvtree that should only hold one
value, there are several utility functions to set and get such fields
defined in `kvtree_util.h` and implemented in `kvtree_util.c`. For
instance, here are a few functions to set single-value fields::

      int kvtree_util_set_bytecount(kvtree* kvtree, const char* key, unsigned long count);
      int kvtree_util_set_crc32(kvtree* kvtree, const char* key, uLong crc);
      int kvtree_util_set_int64(kvtree* kvtree, const char* key, int64_t value);

These utility routines unset any existing value before setting the new
value. They also convert the input value into an appropriate string
representation. Similarly, there are corresponding get routines, such
as::

      int kvtree_util_get_bytecount(const kvtree* kvtree, const char* key, unsigned long* count);
      int kvtree_util_get_crc32(const kvtree* kvtree, const char* key, uLong* crc);
      int kvtree_util_get_int64(const kvtree* kvtree, const char* key, int64_T* value);

If a value is set for the specified key, and if the value can be
interpreted as the appropriate type for the output parameter, the get
routine returns `SUCCESS` and copies the value to the output
parameter. Otherwise, the routine does not return `SUCCESS` and does
not modify the output parameter.

For example, to set and get the timestep value from the example above,
one could do the following::

      kvtree_util_set_int64(kvtree, "TIMESTEP", 21);

      int64_t current_timestep = -1;
      if (kvtree_util_get_int64(kvtree, "TIMESTEP", &current_timestep) == SUCCESS) {
        /* TIMESTEP was set, and it's value is now in current_timestep */
      } else {
        /* TIMESTEP was not set, and current_timestep is still -1 */
      }

The difference between these utility functions and the key/value (`_kv`)
functions is that the key/value functions are used to set and get a kvtree
that is referenced by a key/value pair whereas the utility functions set
and get a scalar value that has no associated kvtree.

Specifying multiple keys with format functions
##############################################

One can set many keys in a single call using a printf-like statement.
This call converts variables like floats, doubles, and longs into
strings. It enables one to set multiple levels of keys in a single call,
and it enables one to specify the kvtree value to associate with the last
element.::

      kvtree_setf(kvtree, value_kvtree, "format", variables ...);

For example, if one had a kvtree like the following::

      RANK
        0
          CKPT
            10
              <current_kvtree>

One could overwrite the kvtree associated with the `10` element in a
single call like so.::

      kvtree_setf(kvtree, new_kvtree, "%s %d %s %d", "RANK", 0, "CKPT", 10);

Different keys are separated by single spaces in the format string. Only
a subset of the printf format strings are supported.

There is also a corresponding getf version.::

      kvtree* kvtree = kvtree_getf(kvtree, "%s %d %s %d", "RANK", 0, "CKPT", 10);

Sorting kvtree keys
#################

Generally, the keys in a kvtree are not ordered. However, one may order
the keys with the following sort routines.::

      kvtree_sort(kvtree, direction);
      kvtree_sort_int(kvtree, direction);

The first routine sorts keys by string, and the second sorts keys as
integer values. The direction variable may be either
`kvtree_SORT_ASCENDING` or `kvtree_SORT_DESCENDING`. The keys remain
in sorted order until new keys are added. The order is not kept between
packing and unpacking kvtreees.

Listing kvtree keys
#################

One may get a sorted list of all keys in a kvtree.::

      int num_keys;
      int* keys;
      kvtree_list_int(kvtree, &num_keys, &keys);
      ...
      if (keys != NULL)
        free(keys);

This routine returns the number of keys in the kvtree, and if there is one
or more keys, it allocates memory and returns the sorted list of keys.
The caller is responsible for freeing this memory. Currently, one may
only get a list of keys that can be represented as integers. There is no
such list routine for arbitrary key strings.

Packing and unpacking kvtreees
############################

A kvtree can be serialized into a memory buffer for network transfer or
storage in a file. To determine the size of a buffer needed to pack a
kvtree.::

      int num_bytes = kvtree_pack_size(kvtree);

To pack a kvtree into a buffer.::

      kvtree_pack(buf, kvtree);

To unpack a kvtree from a buffer into a given kvtree object.::

      kvtree* kvtree = kvtree_new();
      kvtree_unpack(buf, kvtree);

One must pass an empty kvtree to the unpack function.

kvtree files
##########

kvtreees may be serialized to a file and restored from a file. To write a
kvtree to a file.::

      kvtree_file_write(filename, kvtree);

This call creates the file if it does not exist, and it overwrites any
existing file.

To read a kvtree from a file (merges kvtree from file into given kvtree
object).::

      kvtree_file_read(filename, kvtree);

Many kvtree files are written and read by more than one process. In this
case, locks can be used to ensure that only one process has access to
the file at a time. A process blocks while waiting on the lock. The
following call blocks the calling process until it obtains a lock on the
file. Then it opens, reads, closes, and unlocks the file. This results
in an atomic read among processes using the file lock.::

      kvtree_read_with_lock(filename, kvtree)

To update a locked file, it is often necessary to execute a
read-modify-write operation. For this there are two functions. One
function locks, opens, and reads a file.::

      kvtree_lock_open_read(filename, &fd, kvtree)

The opened file descriptor is returned, and the contents of the file are
read (merged) in to the specified kvtree object. The second function
writes, closes, and unlocks the file.::

      kvtree_write_close_unlock(filename, &fd, kvtree)

One must pass the filename, the opened file descriptor, and the kvtree to
be written to the file.

Sending and receiving kvtreees
############################

There are several functions to exchange kvtreees between MPI processes.
While most kvtree functions are implemented in `kvtree.c`, the functions
dependent on MPI are implemented in `kvtree_mpi.c`. This is done so
that serial programs can use kvtreees without having to link to MPI.

To send a kvtree to another MPI process.::

      kvtree_send(kvtree, rank, comm)

This call executes a blocking send to transfer a copy of the specified
kvtree to the specified destination rank in the given MPI communicator.
Similarly, to receive a copy of a kvtree.::

      kvtree_recv(kvtree, rank, comm)

This call blocks until it receives a kvtree from the specified rank, and
then it unpacks the received kvtree into `kvtree` and returns.

There is also a function to simultaneously send and receive kvtreees,
which is useful to avoid worrying about ordering issues in cases where a
process must both send and receive a kvtree.::

      kvtree_sendrecv(kvtree_send, rank_send, kvtree_recv, rank_recv, comm)

The caller provides the kvtree to be sent and the rank it should be sent
to, along with a kvtree to unpack the received into and the rank it should
receive from, as well as, the communicator to be used.

A process may broadcast a kvtree to all ranks in a communicator.::

      kvtree_bcast(kvtree, root, comm)

As with MPI, all processes must specify the same root and communicator.
The root process specifies the kvtree to be broadcast, and each non-root
process provides a kvtree into which the broadcasted kvtree is unpacked.

Finally, there is a call used to issue a (sparse) global exchange of
kvtreees, which is similar to an `MPI_Alltoallv` call.::

      kvtree_exchange(kvtree_send, kvtree_recv, comm)

This is a collective call which enables any process in `comm` to send a
kvtree to any other process in `comm` (including itself). Furthermore, the
destination processes do not need to know from which processes they will
receive data in advance. As input, a process should provide an empty
kvtree for `kvtree_recv`, and it must structure `kvtree_send` in the following
manner.::

      rank_X
         kvtree_to_send_to_rank_X
     rank_Y
         kvtree_to_send_to_rank_Y

Upon return from the function, `kvtree_recv` will be filled in according
to the following format.::

     rank_A
         kvtree_received_from_rank_A
     rank_B
         kvtree_received_from_rank_B

For example, if `kvtree_send` was the following on rank 0 before the call::

      kvtree_send on rank 0:
      1
        FILES
          1
        FILE
          foo.txt
      2
        FILES
          1
        FILE
          bar.txt

Then after returning from the call, `kvtree_recv` would contain the
following on ranks 1 and 2::

      kvtree_recv on rank 1:
      0
        FILES
          1
        FILE
          foo.txt
      (... data from other ranks ...)

      kvtree_recv on rank 2:
      0
        FILES
          1
        FILE
          bar.txt
      (... data from other ranks ...)

The algorithm used to implement this function assumes the communication
is sparse, meaning that each process only sends to or receives from a
small number of other processes. It may also be used for gather or
scatter operations.

Debugging
---------

Newer versions of TotalView enable one to dive on kvtree variables and
inspect them in a variable window using a tree view. For example, when
diving on a kvtree object corresponding to the example kvtree in the
overview section, one would see an expanded tree in the variable view
window like so::

      +- RANK
         +- 0
         |  +- FILES = 2
         |  +- FILE
         |     +- foo_0.txt
         |     |  +- SIZE = 1024
         |     |  +- COMPLETE = 1
         |     +- bar_0.txt
         |        +- SIZE = 2048
         |        +- COMPLETE = 1
         +- 1
            +- FILES = 1
            +- FILE
               +- foo_1.txt
                  +- SIZE = 3072
                  +- COMPLETE = 1

When a kvtree of an element contains a single element whose own kvtree is
empty, this display condenses the line to display that entry as a key =
value pair.

If TotalView is not available, one may resort to printing a kvtree to
`stdout` using the following function. The number of spaces to indent
each level is specified in the second parameter.::

      kvtree_print(kvtree, indent);

To view the contents of a kvtree file, there is a utility called
`print_kvtree_file` which reads a file and prints the contents to the
screen.::

      print_kvtree_file  mykvtreefile.scr

Binary format
-------------

This section documents the binary format used when serializing a kvtree.

Packed kvtree 

A kvtree can be serialized into a memory buffer for network transfer or
storage in a file. When serialized, all integers are stored in network
byte order (big-endian format). Such a "packed" kvtree consists of the
following format:

Format of a PACKED kvtree
 
==========   ==========     ===============================================
Field Name   Datatype       Description
----------   ----------     -----------------------------------------------
Count        uint32_t       Number of elements in kvtree. A count of 0 means the kvtree is empty.
Elements     PACKED         Sequence of packed elements of length Count.
             ELEMENT   
==========   ==========     ===============================================
 
Format of a PACKED ELEMENT

==========   ============================   ===============================
Field Name   Datatype                       Description
----------   ----------------------------   -------------------------------
Key          NULL-terminated ASCII string   Key associated with element
kvtree         PACKED kvtree                    kvtree associated with element
==========   ============================   ===============================
 
File format
 
A kvtree can be serialized and stored as a binary file. This section
documents the file format for an `kvtree` object. All integers are
stored in network byte order (big-endian format). A kvtree file consists
of the following sequence of bytes

============== ============ ============================================================
Field Name     Datatype     Description
-------------- ------------ ------------------------------------------------------------
Magic Number   uint32_t     Unique integer to help distinguish an SCR file from other types of files 0x951fc3f5 (host byte order)
File Type      uint16_t     Integer field describing what type of SCR file this file is 1 -> file is an `kvtree` file
File Version   uint16_t     Integer field that together with File Type defines the file format 1 -> `kvtree` file is stored in version 1 format
File Size      uint64_t     Size of this file in bytes, from first byte of the header to the last byte in the file.
Flags          uint32_t     Bit flags for file.
Data           PACKED kvtree  Packed kvtree data 
CRC32          uint32_t     CRC32 of file, accounts for first byte of header to last byte of Data.  (Only exists if SCR FILE FLAGS CRC32 bit is set in Flags.)
============== ============ ============================================================
