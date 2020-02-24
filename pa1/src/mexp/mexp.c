#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void printArray(int *arr[], int size) {
  int i, j;
  // iterate through array
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      // print element
      printf("%d", arr[i][j]);
      // if not last column, print space after
      if (j != size - 1) {
        printf(" ");
      }
    }
    printf("\n");
  }
}

int multHelper(int *arr1[], int* arr2[], int size, int r, int c) {
  int sum = 0;
  int i;
  // perform matrix multiplication, return result
  // multiply elements and sum them up to get result
  for (i = 0; i < size; i++) sum += arr1[r][i] * arr2[i][c];
  return sum;
}

void exponentiate(int *ans[], int *arr[], int size, int exponent) {
  int r, c;
  // if exponent is 0, set elements to 0 or 1
  if (exponent == 0) {
    for (r = 0; r < size; r++) {
      for (c = 0; c < size; c++) {
        ans[r][c] = (r == c) ? 1 : 0;
      }
    }
    return;
  }
  // if exponent is 1, return
  if (exponent == 1) return;
  // otherwise, allocate tmporary memory
  int *tmp = (int *)malloc(size * size * sizeof(int));
  for (r = 0; r < size; r++) {
    for (c = 0; c < size; c++) {
      // get matrix mult value of element, store in tmp array
      *(tmp + size*r + c) = multHelper(ans, arr, size, r, c);
    }
  }
  // update result array
  for (r = 0; r < size; r++) {
    for (c = 0; c < size; c++) {
      ans[r][c] = *(tmp + size * r + c);
    }
  }
  // decrement exponent
  exponent--;
  // if not 0, recurse
  if (exponent > 1) exponentiate(ans, arr, size, exponent);
}

int main(int argc, char **argv) {
  // check valid args
  if (argc - 1 != 1) {
    printf("Invalid number of arguments");
    return EXIT_SUCCESS;
  }
  // get file
  FILE *file = fopen(argv[1], "r");

  // scan file
  int size, i;
  int f = fscanf(file, "%d", &size);
  if (f <= 0) {
    return EXIT_SUCCESS;
  }
  // allocate array memory
  int **arr = malloc(size * sizeof(int *));
  for (i = 0; i < size; i++) {
    arr[i] = malloc(size * sizeof(int));
  }
  // populate array
  int r, c;
  for (r = 0; r < size; r++) {
    for (c = 0; c < size; c++) {
      fscanf(file, "%d", &(arr[r][c]));
    }
  }

  int exponent;
  fscanf(file, "%d", &exponent);

  int **ans = malloc(size * sizeof(int *));
  for (i = 0; i < size; i++) {
    ans[i] = malloc(size * sizeof(int));
  }
  for (r = 0; r < size; r++) {
    for (c = 0; c < size; c++) {
      ans[r][c] = arr[r][c];
    }
  }

  exponentiate(ans, arr, size, exponent);
  printArray(ans, size);

  // free up memory
  for (r = 0; r < size; r++) {
    free(arr[r]);
    free(ans[r]);
  }
  // close file
  fclose(file);
  return EXIT_SUCCESS;
}