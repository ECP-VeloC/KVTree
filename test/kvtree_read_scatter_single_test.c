#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <kvtree.h>
#include <kvtree_util.h>
#include <mpi.h>

/*
 * Test reading back PREFIX* files into a single kvtree.  This verifies you
 * can read back a complete PREFIX* set of files.  Also verify that if one
 * of the PREFIX.*.* files is missing, that the function correctly fails.
 */

/* Create our PREFIX* files. */
static int create_files(char *prefix_dir, int ranks)
{
  int rc;
  kvtree *tree = kvtree_new();
  char name[10];
  char path[PATH_MAX];
  int i;

  /* Create a kvtree and stick some data in it */
  for (i = 0; i < ranks; i++) {
    sprintf(name, "%d", i);
    kvtree_util_set_str(tree, name, "test data");
  }

  snprintf(path, sizeof(path), "%s/PREFIX", prefix_dir);
  path[sizeof(path) - 1] = '\0';

  rc = kvtree_write_to_gather(path, tree, ranks);
  kvtree_delete(&tree);
  return rc;
}

int main(int argc, char** argv)
{
  int rc;
  char tmp[PATH_MAX];
  char *prefix_dir, *tmp_str = NULL;
  char prefix[PATH_MAX];
  int rank, ranks;

  if (argc != 2) {
    printf("USAGE:\n");
    printf("\n");
    printf("    %s TEMP_DIRECTORY\n", argv[0]);
    printf("\n");
    printf("TEMP_DIRECTORY: A temporary directory to store test files in.\n");
    exit(1);
  }

  prefix_dir = argv[1];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);

  /* Create our prefix files */
  rc = create_files(prefix_dir, ranks);
  if (rc != 0) {
    printf("Error creating files\n");
    return 1;
  }

  MPI_Barrier(MPI_COMM_WORLD);

  /* Only read back the file with rank 0 */
  if (rank != 0) {
    rc = 0;
    goto end;
  }

  /* Construct path to our main PREFIX file */
  snprintf(prefix, sizeof(prefix), "%s/PREFIX", prefix_dir);
  prefix[sizeof(prefix) - 1] = '\0';

  /* Read our PREFIX files */
  kvtree* kvtree = kvtree_new();
  rc = kvtree_read_scatter_single(prefix, kvtree);
  if (rc != KVTREE_SUCCESS) {
    printf("test_kvtree_read_scatter_single failed for %s\n", prefix);
    rc = 1;
    goto end;
  }

  /* Sanity test a value */
  kvtree_util_get_str(kvtree, "0", &tmp_str);
  if (strcmp(tmp_str, "test data") != 0) {
    printf("Got back wrong data (got %s, expected \"test data\")\n", tmp_str);
    rc = 1;
    goto end;
  }
  kvtree_print(kvtree, 4);
  kvtree_delete(&kvtree);

  /*
   * Remove PREFIX.0.0 to induce a failure.
   */
  snprintf(tmp, sizeof(tmp), "%s/PREFIX.0.0", prefix_dir);
  unlink(tmp);

  /* Attempt to call kvtree_read_scatter_single() - it should fail. */
  kvtree = kvtree_new();
  rc = kvtree_read_scatter_single(prefix, kvtree);
  if (rc == KVTREE_SUCCESS) {
    printf("kvtree_read_scatter_single() with missing file incorrectly succeeded\n");
    rc = 1;
  } else {
    /* success */
    rc = 0;
  }

end:
  kvtree_delete(&kvtree);

  if (rank == 0) {
    /* Remove our test files */
    snprintf(tmp, sizeof(tmp), "rm -f %s/PREFIX*", prefix_dir);
    system(tmp);
  }
  MPI_Finalize();

  return rc;
}
