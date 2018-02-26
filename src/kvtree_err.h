/*
* Copyright (c) 2018, Lawrence Livermore National Security, LLC.
* Produced at the Lawrence Livermore National Laboratory.
*
* Copyright (c) 2018, UChicago Argonne LLC,
*   operator of Argonne National Laboratory
*
* LLNL-CODE-745961
* All rights reserved.
*
* This is the license for KVTree.
* For details, see https://github.com/LLNL/KVTree
* Please read the LICENSE file for full license text.
*/

#ifndef KVTREE_ERR_H
#define KVTREE_ERR_H

/*
=========================================
Error and Debug Messages
=========================================
*/

/* print error message to stdout */
void kvtree_err(const char *fmt, ...);

/* print warning message to stdout */
void kvtree_warn(const char *fmt, ...);

/* print message to stdout if kvtree_debug is set and it is >= level */
void kvtree_dbg(int level, const char *fmt, ...);

/* print abort message and kill run */
void kvtree_abort(int rc, const char *fmt, ...);

#endif
