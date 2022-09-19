#include <stdio.h>
#include <stdlib.h>

/* Purpose: Creates a 2D matrix given specified dimensions.
 * int size: Dimensions of desired square matrix.
 * return: 2D array structure for square matrix.
 */
double **create_matrix(int size){
	double **matrix = (double **)malloc(size * sizeof(double *));
	for (int i = 0; i < size; i++)
		matrix[i] = (double *)malloc(size * sizeof(double));
	return (matrix);
}

/* Purpose: Frees memory previously allocated for matrix array.
 * int size: Dimensions of square matrix.
 * double **mat: Matrix to be freed.
 * return: Nothing.
 */
void free_matrix(int size, double **matrix){
	for (int i = 0; i < size; i++)
		free(matrix[i]);
	free(matrix);
}

/* Purpose: Print the contents of matrix array to stdout.
 * int size: Dimensions of square matrix.
 * double **mat: Contents of matrix passed from stdin.
 * return: Nothing.
 */
void print_matrix(int size, double **mat){
	printf("Elements:\n");
	for (int row = 0; row < size; row++){
		for (int col = 0; col < size; col++){
			printf("%lf ", mat[row][col]);
		}
		printf("\n");
	}
}

/* Purpose: Swaps specified rows in a square matrix. As a result, the sign of the
 * determinant is flipped.
 * int size: Dimensions of square matrix.
 * double **mat: Contents of matrix passed from stdin.
 * int row1: Row which needs to be swapped.
 * int row2: Row which will be swapped with row1.
 * return: -1, scalar used to flip the sign of the determinant after each swap.
 */
int swap_rows(int size, double **mat, int row1, int row2){
	double temp_mat[size]; /* store row1 here */
	for (int col = 0; col < size; col++){
		temp_mat[col] = mat[row1][col];
		mat[row1][col] = mat[row2][col];
		mat[row2][col] = temp_mat[col];
	}
	return (-1);
}

/* Purpose: Finds the determinant of a given square matrix.
 * int size: Dimensions of square matrix.
 * double **mat: Contents of matrix passed from stdin.
 * int det_scalar: Represents the sign of the determinant.
 * double to_add: Acts as a scalar for a row which will be added to transform another row.
 * double determinant: Resulting determinant of specified matrix.
 * return: determinant.
 */
double calc_determinant(int size, double **mat)
{
	int det_scalar = 1;
	double to_add = 0, determinant = 1;

	/* Check if diagonal elements are all nonzero
	 * If not, then swap with a row from below in the matrix */
	for (int i = 0; i < size; i++){
		if (mat[i][i] == 0){
			for (int row = i + 1; row < size; row++){
				if (mat[row][i] != 0){
					det_scalar *= swap_rows(size, mat, row, i);
					break;
				}
			}
			if (mat[i][i] == 0)
				determinant = 0;
		}
		/* If any diagonal elements are zero at this point,
		 * do not continue, the determinant is zero */
		if (determinant == 0)
			break;
		/* Transform into upper triangluar matrix */
		for (int j = i + 1; j < size; j++){
			to_add = mat[j][i] / mat[i][i];
			for (int k = 0; k < size; k++){
				mat[j][k] = mat[j][k] - to_add * mat[i][k];
			}
		}
	}
	/* Multiply all elements on diagonal for determinant */
	for (int i = 0; i < size; i++)
		determinant *= mat[i][i];
	return (determinant * det_scalar);
}

/* Purpose: Calculates the determinant of a matrix of dimension mat_size with contents
 * held in mat_array. The resulting determinant will be printed to stdout.
 * int mat_size: Dimensions of square matrix.
 * double **mat_array: Contents of matrix passed from stdin.
 * double element: Placeholder for each element of the matrix.
 * return: 0 on success, -1 on failure.
 */
int main(){
	int mat_size = 0;
	/* Read matrix dimensions */
	scanf("%d", &mat_size);
	if (mat_size < 1){
		printf("Matrix must have dimensions greater than 0.");
		return (-1);
	}
	// printf("\nMatrix size: %d\n", mat_size);
	double **mat_array = create_matrix(mat_size);

	/* Ingest contents of matrix from stdin */
	double element = 0;
	for (int row = 0; row < mat_size; row++){
		for (int col = 0; col < mat_size; col++){
			if (scanf("%lf", &element) == 1){
				mat_array[row][col] = element;
			}
		}
	}

	/* Calculate determinant, print to stdout, then free allocated memory */
	printf("%lf\n", calc_determinant(mat_size, mat_array));
	free_matrix(mat_size, mat_array);
	return 0;
}