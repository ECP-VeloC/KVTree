/*
 * This test spawns off a bunch of processes that all try to read and write the
 * same kvtree file over and over.  It is actually two tests - first, it tests
 * that there is file corruption if the kvtree file is written with no locking.
 * Second, it verifies that there is no corruption if locking is used.
 */

#include "kvtree.h"
#include "test_kvtree_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/* get_nprocs() */
#if defined(__APPLE__)
#include <sys/sysctl.h>
#else
#include <sys/sysinfo.h>
#endif

#define PROCESSES 8
#define ITERATIONS 1000
#define TESTFILE "/dev/shm/test_kvtree_write_locking"

/*
 * This is our process function that writes the file and then tries to read it.
 * Return NULL on success, non-NULL otherwise.
 */
int process_func(int use_locking)
{
  kvtree *kvtree;
  int rc;
  int iterations = ITERATIONS;

  srand(1);

  while (iterations--) {
    kvtree = kvtree_new();

    kvtree_util_set_int64(kvtree, "DATA",  rand());

    if (use_locking) {
      rc = kvtree_write_with_lock(TESTFILE, kvtree);
    } else {
      rc = kvtree_write_file(TESTFILE, kvtree);
    }
    kvtree_delete(&kvtree);

    if (rc != KVTREE_SUCCESS) {
      /* Couldn't write file */
      return 1;
    }

    kvtree = kvtree_new();
    if ((kvtree_read_with_lock(TESTFILE, kvtree) != KVTREE_SUCCESS)) {
      /* Couldn't read file */
      kvtree_delete(&kvtree);
      return 1;
    }
    kvtree_delete(&kvtree);
  }

  return 0;    /* success */
}

/*
 * Spawn off a bunch of processes th
 */
int spawn_processes(int use_locking)
{
  int rc;
  int i;
  pid_t pids[PROCESSES] = {0};
  int status;
  pid_t child_pid;
  int failures = 0;

  /* Wait for the threads to finish */
  for (i = 0; i < PROCESSES; i++) {
    child_pid = fork();
    if (child_pid == 0) {
      /* We're a child. */
      process_func(use_locking);

      /* Try to write our kvtree and pass our return code to exit() */
      rc = process_func(use_locking);
      exit(rc);
      break;
    } else {
      pids[i] = child_pid;
      /* We're the original process (parent).  Keep spawning children */
    }
  }

  for (i = 0; i < PROCESSES; i++) {
    waitpid(pids[i], &status, 0);
    status = WEXITSTATUS(status);
    if (status != 0) {
      failures++;
    }
  }
  return failures;
}

int main(int argc, char** argv)
{
  int cpus;

#if defined(__APPLE__)
    int count;
    size_t size = sizeof(count);
    if (sysctlbyname("hw.ncpu", &count, &size, NULL, 0)) {
        cpus = 1;
    } else {
        cpus = count;
    }
#else
    cpus = get_nprocs();
#endif


  if (cpus <= 1) {
    printf("We need more than 1 CPU to run this test.  Skipping.\n");
    return TEST_PASS;
  }
  printf("Testing that non-locking writes will corrupt the kvtree file.\n");
  printf("NOTE: You will see KVTree errors if this is working correctly:\n");
  printf("Testing with %d CPUs\n", cpus);
  fflush(stdout);

  if (spawn_processes(0) != 0) {
    printf("Non-locking test correctly failed\n");
  } else {
    printf("Non-locking test should have failed, but didn't\n");
    return (1);
  }
  fflush(stdout);

  printf("Testing that locking writes don't corrupt the kvtree file\n");
  fflush(stdout);
  if (spawn_processes(1) == 0) {
    printf("Locking test correctly passed\n");
  } else {
    printf("Locking test failed\n");
    return (1);
  }

  return TEST_PASS;
}
