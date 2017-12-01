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

/* This implements the kvtree_err.h interface, but for serial jobs,
 * like the SCR utilities. */

#include "kvtree_err.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* variable length args */
#include <stdarg.h>
#include <errno.h>

/* version info */
#include "kvtree.h"

/*
=========================================
Error and Debug Messages
=========================================
*/

/* print error message to stdout */
void kvtree_err(const char *fmt, ...)
{
  /* get my hostname */
  char hostname[256];
  if (gethostname(hostname, sizeof(hostname)) != 0) {
    /* TODO: error! */
  }

  va_list argp;
  fprintf(stdout, "SCR %s ERROR: %s: ", KVTREE_VERSION, hostname);
  va_start(argp, fmt);
  vfprintf(stdout, fmt, argp);
  va_end(argp);
  fprintf(stdout, "\n");
}

/* print warning message to stdout */
void kvtree_warn(const char *fmt, ...)
{
  /* get my hostname */
  char hostname[256];
  if (gethostname(hostname, sizeof(hostname)) != 0) {
    /* TODO: error! */
  }

  va_list argp;
  fprintf(stdout, "SCR %s WARNING: %s: ", KVTREE_VERSION, hostname);
  va_start(argp, fmt);
  vfprintf(stdout, fmt, argp);
  va_end(argp);
  fprintf(stdout, "\n");
}

/* print message to stdout if kvtree_debug is set and it is >= level */
void kvtree_dbg(int level, const char *fmt, ...)
{
  /* get my hostname */
  char hostname[256];
  if (gethostname(hostname, sizeof(hostname)) != 0) {
    /* TODO: error! */
  }

  va_list argp;
  /*
  if (level == 0 || (kvtree_debug > 0 && kvtree_debug >= level)) {
  */
    fprintf(stdout, "SCR %s: %s: ", KVTREE_VERSION, hostname);
    va_start(argp, fmt);
    vfprintf(stdout, fmt, argp);
    va_end(argp);
    fprintf(stdout, "\n");
  /*
  }
  */
}

/* print abort message and kill run */
void kvtree_abort(int rc, const char *fmt, ...)
{
  /* get my hostname */
  char hostname[256];
  if (gethostname(hostname, sizeof(hostname)) != 0) {
    /* TODO: error! */
  }

  va_list argp;
  fprintf(stderr, "SCR %s ABORT: %s: ", KVTREE_VERSION, hostname);
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  fprintf(stderr, "\n");

  exit(rc);
}
