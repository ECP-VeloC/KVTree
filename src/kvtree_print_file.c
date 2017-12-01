/*
 * Copyright (c) 2009, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 * Written by Adam Moody <moody20@llnl.gov>.
 * LLNL-CODE-411039.
 * All rights reserved.
 * This file was originally part of The Scalable Checkpoint / Restart (SCR) library.
 * For details, see https://sourceforge.net/projects/scalablecr/
 * Please also read this file: LICENSE.TXT.
*/

/* Utility to pretty print a hash file to the screen. */

#include "kvtree.h"
#include "kvtree_io.h"
#include "kvtree_err.h"
#include "kvtree_helpers.h"
#include "kvtree.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[])
{
  int rc = 0;

  /* check that we were given exactly one filename argument */
  if (argc != 2) {
    printf("Usage: kvtree_print_hash_file <hashfile>\n");
    return 1;
  }

  /* read in the filename */
  char* filename = strdup(argv[1]);
  if (filename != NULL) {
    /* get a hash to read in the file */
    kvtree* hash = kvtree_new();

    /* read in the file */
    if (kvtree_read_file(filename, hash) == KVTREE_SUCCESS) {
      /* we read the file, now print it out */
      kvtree_print(hash, 0);
    } else {
      kvtree_err("Could not read file %s @ %s:%d",
              filename, __FILE__, __LINE__
      );
      rc = 1;
    }

    /* free the hash object */
    kvtree_delete(&hash);

    /* free our strdup'd filename */
    kvtree_free(&filename);
  } else {
    kvtree_err("Could not copy filename from command line @ %s:%d",
            __FILE__, __LINE__
    );
    rc = 1;
  }

  return rc;
}
