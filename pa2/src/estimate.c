/* PA2 estimate */

#include <stdio.h>
#include <stdlib.h>

void print(double **matrix, int rows, int cols);
double** transpose(double **matrix, int rows, int cols);
double** invert(double **matrix, int rows, int cols);
double** make_val_one(double **matrix, int i, int j, int rows, int cols);
double** make_val_zero(double **matrix, int i, int j, int rows, int cols);
double** mmult(double **matrix1, double **matrix2, int rows1, int rows2, int cols1, int cols2);
double** get_weights(double **matrix, double **Y, int rows, int cols);
double** predict(double **matrix, double **weights, int rows, int cols);

int main(int argc, char ** argv) {
  int num_attribs, num_houses, num_houses_test;

	// get training data
  FILE *train = fopen(argv[1], "r");
  if (train == NULL) {
    printf("Cannot find training data file with that name\n");
    return 0;
  }

	// get testing data
  FILE *test = fopen(argv[2], "r");
  if (test == NULL) {
    printf("Cannot find test data file with that name\n");
    return 0;

  }
	
	// get number of attributes & houses
	fscanf(train, "%*s %d\n", &num_attribs);
  fscanf(train, "%d", &num_houses);

	// get number of houses in testing dataset
	fscanf(test, "%*s %*s %d\n", &num_houses_test);	
	
  int i, j;

	// allocate memory for training data matrix
  double **train_matrix = (double **) malloc(num_houses * sizeof(double *));
  for (i = 0; i < num_houses; i++) {
    *(train_matrix + i) = (double *) malloc((num_attribs + 1) * sizeof(double));
  }

	// allocate memory for testing data matrix
  double **test_matrix = (double **) malloc(num_houses_test * sizeof(double *));
  for (i = 0; i < num_houses_test; i++) {
    *(test_matrix + i) = (double *) malloc((num_attribs) * sizeof(double));
  }
	// allocate memory for Y matrix
  double **Y = (double **) malloc(num_houses * sizeof(double *));
  for (i = 0; i < num_houses; i++) {
    *(Y + i) = (double *) malloc(1 * sizeof(double));
  }
	// allocate memory for weights
  double **weights = (double **) malloc((num_attribs + 1) * sizeof(double *));
  for (i = 0; i <= num_attribs; i++) {
    *(weights + i) = (double *) malloc(1 * sizeof(double));
  }
	// allocate memory for Y_test matrix
  double **Y_test = (double **) malloc(num_houses_test * sizeof(double *));
  for (i = 0; i < num_houses_test; i++) {
    *(Y_test + i) = (double *) malloc(1 * sizeof(double));
  }

	// set first column of training matrix to 1
  for (i = 0; i < num_houses; i++) {
    train_matrix[i][0] = 1;
  }
	// build the rest of the training matrix
  for (i = 0; i < num_houses; i++) {
    for (j = 1; j <= num_attribs; j++) {
      fscanf(train, "%lf%*[,]", & train_matrix[i][j]);
    }
    fscanf(train, "%lf", & Y[i][0]);
    fscanf(train, "\n");
  }

	// build testing matrix
  for (i = 0; i < num_houses_test; i++) {
    for (j = 0; j < num_attribs; j++) {
      fscanf(test, "%lf%*[,]", & test_matrix[i][j]);
    }
    fscanf(test, "\n");
  }

	// get weights matrix
  weights = get_weights(train_matrix, Y, num_houses, num_attribs + 1);
	// use weights to predict Y_test
  Y_test = predict(test_matrix, weights, num_houses_test, num_attribs);

	// free memory
  free(train_matrix);
  free(test_matrix);
  free(Y_test);
  free(Y);
  free(weights);

  return 0;
}

double** predict(double **matrix, double **weights, int rows, int cols) {
  int i, r, c;
  double curr_val = 0;
	// allocate memory for prediction matrix
  double **res = (double ** ) malloc(rows * sizeof(double * ));
  for (i = 0; i < rows; ++i) {
    *(res + i) = (double *) malloc(1 * sizeof(double));
  }
	// iterate through rows
  for (r = 0; r < rows; ++r) {
		// get weight value
    curr_val = weights[0][0];
		// iterate through cols
		for (c = 0; c < cols; ++c) {
			// do matrix multiplication with weight to get Y'
      curr_val = curr_val + (matrix[r][c] * weights[c + 1][0]);
    }
		// update Y'
    res[r][0] = curr_val;
		// print prediction
    printf("%0.0lf\n", res[r][0]);
  }
  return res;
}

// returns a transposed version of inputted matrix
double** transpose(double **matrix, int row, int col) {
	// allocate memory for transpose matrix
  double **res = (double **) malloc(col * sizeof(double *));
  int i, j;
  for (i = 0; i < col; ++i) {
    *(res + i) = (double *) malloc(row * sizeof(double));
  }
	// iterate through matrix, switching rows and cols
  for (i = 0; i < col; i++) {
    for (j = 0; j < row; j++) {
      res[i][j] = matrix[j][i];
    }
  }
  return res;
}

void print(double **matrix, int rows, int cols) {
  int i, j;
	// iterate through and print current value
  for (i = 0; i < rows; ++i) {
    for (j = 0; j < cols; ++j) {
      printf("%lf\t", matrix[i][j]);
    }
    printf("\n");
  }
}

// helper method for inverting an input matrix
double** invert(double **matrix, int rows, int cols) {
	int i, r, c;
	// allocate memory for helper matrix (augmented input & identity)
	double **augmented = (double **) malloc(rows * sizeof(double *));
	// allocate space for input matrix & identity matrix
  for (i = 0; i < rows; i++) {
    *(augmented + i) = (double *) malloc((cols * 2) * sizeof(double));
  }
	// build input matrix
  for (r = 0; r < rows; ++r) {
    for (c = 0; c < cols; ++c) {
      augmented[r][c] = matrix[r][c];
    }
  }
	// build identity matrix
  for (r = 0; r < rows; ++r) {
    for (c = cols; c < (cols * 2); ++c) {
			// if diagonal, this is a 1
      if (c - cols == r) {
        augmented[r][c] = 1;
			// otherwise 0
      } else {
        augmented[r][c] = 0;
      }
    }
  }

  // iterate through input matrix (left matrix)
	// we want to get an upper triangular matrix
  for (r = 0; r < rows; ++r) {
    for (c = 0; c < cols; ++c) {
			// if this is a pivot element
      if (r == c) {
				// manipulate matrix until this element becomes a 1
        augmented = make_val_one(augmented, r, c, rows, cols * 2);
				// iterate through remaining elements in column, make them 0
        for (i = r + 1; i < rows; ++i) {
          augmented = make_val_zero(augmented, i, c, rows, cols * 2);
        }
      }
    }
  }

	// starting from the top-right, we want all non-diagonal elements to be zero
  for (r = rows - 1; r >= 0; --r) {
    for (c = cols - 1; c >= 0; --c) {
			// if this is not a diagonal element and not a zero
      if (r != c && augmented[r][c] != 0) {
				// make it a zero
        augmented = make_val_zero(augmented, r, c, rows, cols * 2);
      }
    }
  }

  // allocate memory for inverse matrix result
  double **res = (double **) malloc(rows * sizeof(double *));
  for (i = 0; i < rows; i++) {
    res[i] = (double *) malloc((cols) * sizeof(double));
  }
	// build inverse matrix from right section of augmented matrix
  for (r = 0; r < rows; ++r) {
    for (c = 0; c < cols; ++c) {
      res[r][c] = augmented[r][c + cols];
    }
  }
  return res;
}

// helper method for manipulating element into a one
double** make_val_one(double **matrix, int i, int j, int rows, int cols) {
	// check if target is already 1
	if (matrix[i][j] == 1)
		return matrix;
	// allocate memory for result matrix
	double **res = (double **) malloc(rows * sizeof(double *));
  int r, c;
  for (r = 0; r < rows; ++r) {
    *(res + r) = (double *) malloc((cols) * sizeof(double));
  }
	// build result matrix from input matrix
  for (r = 0; r < rows; ++r) {
    for (c = 0; c < cols; ++c) {
      res[r][c] = matrix[r][c];
    }
  }
  double val = res[i][j];
	// divide every value in row by scale factor
	for (c = 0; c < cols; ++c) res[i][c] /= val;
	return res;
}

// helper method for manipulating element into a zero
double** make_val_zero(double **matrix, int i, int j, int rows, int cols) {
	// check if already 0
  if (matrix[i][j] == 0) return matrix;
	int r, c;
	// allocate memory for res matrix
	double **res = (double **) malloc(rows * sizeof(double *));
	for (r = 0; r < rows; ++r) *(res + r) = (double *) malloc((cols) * sizeof(double));
	// build res matrix from input matrix
  for (r = 0; r < rows; ++r) {
    for (c = 0; c < cols; ++c) {
      res[r][c] = matrix[r][c];
    }
  }
	// get value of target element
  double val = res[i][j];
	// iterate through current row
  for (c = 0; c < cols; ++c) {
		// subtract scaled scaled pivot value from row
		// j = target column = pivot row, i = target row
		res[i][c] -= res[j][c] * val;
	}
	return res;
}

// helper method for doing matrix multiplication with two input matrices
double** mmult(double **matrix1, double **matrix2, int rows1, int rows2, int cols1, int cols2) {
	int r, c, i;
	double curr_val = 0;
	// allocate memory for product matrix
  double **res = (double ** ) malloc(rows1 * sizeof(double * ));
  for (i = 0; i < rows1; ++i) 
		*(res + i) = (double *) malloc((cols2) * sizeof(double));
	// iterate through matrix1
  for (r = 0; r < rows1; ++r) {
    for (c = 0; c < cols2; ++c) {
			// iterate through current matrix1 row & matrix2 column
      for (i = 0; i < rows2; ++i)
				// add product of two matrix indices to curr_val
				curr_val += matrix1[r][i] * matrix2[i][c];
			// update resulting matrix multiplication value
      res[r][c] = curr_val;
			// reset curr_val
      curr_val = 0;
    }
  }
  return res;
}

// returns weights given an input matrix and its associated outputs
double** get_weights(double **matrix, double **Y, int rows, int cols) {
  int i;
	// allocate memory for X transpose
  double **X_t = (double **) malloc(cols * sizeof(double *));
  for (i = 0; i < cols; ++i) {
    *(X_t + i) = (double *) malloc((rows) * sizeof(double));
  }
	// allocate memory for result matrix
  double ** res = (double ** ) malloc(cols * sizeof(double * ));
  for (i = 0; i < cols; ++i) {
    *(res + i) = (double *) malloc(1 * sizeof(double));
  }
	// get transpose matrix
  X_t = transpose(matrix, rows, cols);
	// multiply by X to get X_t_X
  res = mmult(X_t, matrix, cols, rows, rows, cols);
	// get inverse of X_t_X
  res = invert(res, cols, cols);
	// multiply by X_t
  res = mmult(res, X_t, cols, cols, cols, rows);
	// multiply by Y
  res = mmult(res, Y, cols, rows, rows, 1);
	// return result
  return res;
}