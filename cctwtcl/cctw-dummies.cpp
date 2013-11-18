
#include <stdbool.h>
#include <stdlib.h>

#include "cctw-dummies.h"

#define UNUSED __attribute__((unused))

static bool initialized;

static unsigned int seedp;

static void initialize(void)
{
  seedp = 4;
  rand_r(&seedp);
}

static int random_output_count(void)
{
  if (!initialized) initialize();

  int r = rand_r(&seedp);
  int count = r % 6;
  if (count == 6) count = 20;
  return count;
}

output_entry** allocate_output_entries(int count)
{
  output_entry** result =
    (output_entry**) malloc(count*sizeof(output_entry*));
  return result;
}

bool read_hdf_fragment(UNUSED const char* filename,
                       void** output, int* output_length)
{
  int count = 1024;
  int length = count * sizeof(double);
  *output = calloc(count, sizeof(double));
  *output_length = length;
  return true;
}

bool unpack_fragment(void* fragment, UNUSED void* fragment_length,
                     double** intensity, int* intensity_count,
                     double** weight, int* weight_count)
{
  double* p = (double*) fragment;
  *intensity = p + 0;
  *weight = p + 512;
  *intensity_count = 512;
  *weight_count = 512;
  return true;
}

static void random_indices(int max_i, int max_j, int max_k,
                           int* i, int* j, int* k);

bool forward_transform(int max_i, int max_j, int max_k,
                       UNUSED int i, UNUSED int j, UNUSED int k,
                       UNUSED double* intensity, int UNUSED intensity_count,
                       UNUSED double* weight, UNUSED int weight_count,
                       output_entry** output, int* output_count)
{
  int count = random_output_count();
  for (int c = 0; c < count; c++)
  {
    output_entry* entry = (output_entry*) malloc(sizeof(output_entry));
    random_indices(max_i, max_j, max_k,
                   &entry->i, &entry->j, &entry->k);
    entry->intensity_count = 10;
    entry->intensity =
        (double*) calloc(entry->intensity_count, sizeof(double));
    entry->weight_count = 10;
    entry->weight =
        (double*) calloc(entry->weight_count, sizeof(double));
    output[c] = entry;
  }
  *output_count = count;
  return true;
}

static void random_indices(int max_i, int max_j, int max_k,
                           int* i, int* j, int* k)
{
  int r;
  r = rand_r(&seedp);
  *i = r % max_i;
  r = rand_r(&seedp);
  *j = r % max_j;
  r = rand_r(&seedp);
  *k = r % max_k;
}

bool serialize_output_entries(output_entry* output)
{
  void* buffer = malloc(5 * sizeof(int) +
                        output->intensity_count * sizeof(double) +
                        output->weight_count * sizeof(double));
  if (!buffer) return false;
  return true;
}

void free_output_entries(output_entry** outputs, int output_count)
{
  for (int i = 0; i < output_count; i++)
  {
    free(outputs[i]->intensity);
    free(outputs[i]->weight);
    free(outputs[i]);
  }
  free(outputs);
}
