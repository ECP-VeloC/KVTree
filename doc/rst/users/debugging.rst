Kvtree Debugging 
----------------

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
