
Kvtree Binary File Format
+++++++++++++++++++++++++


This section documents the binary format used when serializing a kvtree.

Packed kvtree 
-------------

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
-----------
 
A kvtree can be serialized and stored as a binary file. This section
documents the file format for an `kvtree` object. All integers are
stored in network byte order (big-endian format). A kvtree file consists
of the following sequence of bytes

============== ============== ============================================================
Field Name     Datatype       Description
-------------- -------------- ------------------------------------------------------------
Magic Number   uint32_t       Unique integer to help distinguish an SCR file from other types of files 0x951fc3f5 (host byte order)
File Type      uint16_t       Integer field describing what type of SCR file this file is 1 -> file is an `kvtree` file
File Version   uint16_t       Integer field that together with File Type defines the file format 1 -> `kvtree` file is stored in version 1 format
File Size      uint64_t       Size of this file in bytes, from first byte of the header to the last byte in the file.
Flags          uint32_t       Bit flags for file.
Data           PACKED kvtree  Packed kvtree data 
CRC32          uint32_t       CRC32 of file, accounts for first byte of header to last byte of Data.  (Only exists if SCR FILE FLAGS CRC32 bit is set in Flags.)
============== ============== ============================================================
