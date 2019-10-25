#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

double** allocate(int rows, int cols);
void multiply(double** result, double** matrix1, double** matrix2, int rows1, int cols2, int same);
void inverse(double** inv, double** id, int rows);
void print_matrix(double** matrix_result, int rows, int cols);
void free_matrix(double** matrix, int rows);


double** allocate(int rows, int cols) {
	double** matrix = malloc(rows* sizeof(double*));
	for(int i = 0; i < rows; i++) {
		matrix[i] = malloc(cols * sizeof(double));
	}
	
	return matrix;
}


void identity(double** result, int cols) {
	for(int i = 0; i < cols; i++) {
		for(int j = 0; j < cols; j++) {
			if(i == j) {
				result[i][j] = 1;
			} else {
				result[i][j] = 0;
			}
		}
	}	
}

void multiply(double** result, double** matrix1, double** matrix2, int rows1, int cols2, int same) {
	for(int i = 0; i < rows1; i++) {
		for(int j = 0; j < cols2; j++) {
			double sum = 0;
			for(int k = 0; k < same; k++) {
				sum+= (matrix1[i][k] * matrix2[k][j]);
			}
			result[i][j] = sum;
		}
	}
}

void inverse(double** matrix, double** id, int length) {
	
	for(int i = 0; i < length; i++) {
		for(int j = i; j < length; j++) {
			if(i == j) {
				if(matrix[j][i] != 1) {
					double pivot = matrix[j][i];
					for(int cols = 0; cols < length; cols++) {
						matrix[j][cols] = matrix[j][cols]/pivot;
						id[j][cols] = id[j][cols]/pivot;
					}
				}
			} else {
				if(matrix[j][i] != 0) {
					double num = matrix[j][i];
					for(int cols = 0; cols < length; cols++) {
						matrix[j][cols] = matrix[j][cols] - (num * (matrix[i][cols]));
						id[j][cols] = id[j][cols] - (num * (id[i][cols]));
					}
				}
			}
		}
	}

	for(int i = length-1; i >= 0; i--) {
		for(int j = i; j>= 0; j--) {
			if(i == j) {
				if(matrix[j][i] != 1) {
					double pivot = matrix[j][i];
					for(int cols = length-1; cols >= 0; cols--) {
						matrix[j][cols] = (matrix[j][cols])/pivot;
						id[j][cols] = (id[j][cols])/pivot;
					}
				}
			} else {
				if(matrix[j][i] != 0) {
					double num = matrix[j][i];
					for(int cols = length-1; cols >= 0; cols--) {
						matrix[j][cols] = matrix[j][cols] - (num * (matrix[i][cols]));
						id[j][cols] = id[j][cols] - (num * (id[i][cols]));
					}
				}
			}
		}
	}

	for(int i = 0; i < length; i++) {
		for(int j = 0; j < length; j++) {
			matrix[i][j] = id[i][j];
		}
	}
}

void print_matrix(double** matrix_result, int rows, int cols) {

	for(int i = 0; i < rows; i++) {
		printf("\n");
		

		for(int j = 0; j < cols; j++) {
			printf("%lf	", matrix_result[i][j]);
		}
	}
	printf("\n");
}

void free_matrix(double** matrix, int rows) {
	for (int i = 0; i < rows; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

int main(int argc, char** argv) {
	
	if (argc != 3) {
		printf("insufficient arguments\n");
		exit(0);
	}

	FILE* fp1 = fopen(argv[1], "r");
	FILE* fp2 = fopen(argv[2], "r");

	if(fp1 == NULL || fp2 == NULL) {
		printf("error\n");
		exit(0);
	}

	int check1 = 0;
	int check2 = 0;
	if(fscanf(fp1, "%d\n", &check1) == EOF) {
		exit(0);
	}
	if(fscanf(fp2, "%d\n", &check2) == EOF) {
		exit(0);
	}

	int cols = check1;
	int rows;
	fscanf(fp1, "%d\n", &rows);

	double** matrix_original = allocate(rows, cols);
	double* prices = malloc(rows * sizeof(double));

	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols+1; j++) {
			double num;
			fscanf(fp1, "%lf", &num);
			char c;
			fscanf(fp1, "%c", &c);
			
			if (j == cols) {
				prices[i] = num;
			} else {
				matrix_original[i][j] = num;
			}

		}
	}
	fclose(fp1);	

	double** matrix_attributes = allocate(check2, cols);	
	for(int i = 0; i < check2; i++) {
		for(int j = 0; j < cols; j++) {
			double num;
			fscanf(fp2, "%lf", &num);
			char c;
			fscanf(fp2, "%c", &c);

			matrix_attributes[i][j] = num;
		}
	}

	//initialization
	int colsX = cols+1;
	double** X = allocate(rows, colsX);

	for(int i = 0; i < rows; i++) {
		X[i][0] = 1;
		for(int j = 0; j < cols; j++) {
			X[i][j+1] = matrix_original[i][j];
		}
	}
	
	double** Xt = allocate(colsX, rows);
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < colsX; j++) {
			Xt[j][i] = X[i][j];
		}
	}

	double** XtX = allocate(colsX, colsX);
	multiply(XtX, Xt, X, colsX, colsX, rows);

	double** id = allocate(colsX, colsX);
	for(int i = 0; i < colsX; i++) {
		for(int j = 0; j < colsX; j++) {
			if(j == i) {
				id[i][j] = 1;
			} else{
				id[i][j] = 0;
			}
		}
	}

	double** inv = allocate(colsX, colsX);
	for(int i = 0; i < colsX; i++) {
		for(int j = 0; j < colsX; j++) {
			inv[i][j] = XtX[i][j];
		}
	}
	inverse(inv, id, colsX);

	double** invXt = allocate(colsX, rows);
	multiply(invXt, inv, Xt, colsX, rows, colsX);

	double** Y = allocate(rows, 1);
	for(int i = 0; i < rows; i++) {
		Y[i][0] = prices[i];
	}
	
	double** W = allocate(colsX, 1);
	multiply(W, invXt, Y, colsX, 1, rows);

	for(int i = 0; i < check2; i++) {
		double value = W[0][0];
		for(int j = 0; j < cols; j++) {
			value += W[j+1][0] * matrix_attributes[i][j];
		}
		printf("%0.0lf\n", value);
	}

	//free
	free_matrix(matrix_original, rows);
	free_matrix(X, rows);
	free_matrix(Xt, colsX);
	free_matrix(XtX, colsX);
	free_matrix(inv, colsX);
	free_matrix(id, colsX);
	free_matrix(invXt, colsX);
	free(prices);
	free_matrix(Y, rows);
	free_matrix(W, colsX);
	free_matrix(matrix_attributes, check2);


	return 0;

}	
