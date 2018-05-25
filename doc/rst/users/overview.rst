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
