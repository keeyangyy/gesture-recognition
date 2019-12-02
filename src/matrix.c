/*
 * matrix.c
 *
 *  Created on: 2018年7月21日
 *      Author: wjy
 */

#include "matrix.h"
#include <math.h>

/*
create a matrix with 0
*/
Matrix_t create_mat(uint16_t row, uint16_t column)
{
    Matrix_t mat;
    if (row <= 0||column<=0)
    {
        printf("error, in create_mat: row <= 0||column<=0\n");
        exit(1);
    }
    if (row > 0 && column > 0)
    {
        mat.row = row;
        mat.column = column;
        mat.data = (float **)malloc(row*sizeof(float *));//先指针的指针
        if (mat.data == NULL)
        {
            printf("error, in create_mat: mat.data==NULL");
            exit(1);
        }
        uint16_t i;
        for (i = 0; i < row; i++)
        {
            *(mat.data + i) = (float *)malloc(column*sizeof(float));//再分配每行的指针
            if (mat.data[i] == NULL)
            {
              printf("error, in create_mat: mat.data==NULL");
              exit(1);
            }
        }
        clear_mat(&mat);
    }
    return mat;
}

/*
set all datas in matrix to zero
*/
void clear_mat(Matrix_t* mat)
{
    uint16_t i, j;
    for (i = 0; i < mat->row; i++)
    {
        for (j = 0; j < mat->column; j++)
        {
            mat->data[i][j] = 0;
        }
    }
}


/*
free a matrix
*/
void free_mat(Matrix_t *mat)
{
    uint16_t i;
    for (i = 0; i < mat->row; i++)
        free(mat->data[i]);/*释放行*/
    free(mat->data);/*释放头指针*/
}

/*
set datas to the matrix
*/
void set_mat_data(Matrix_t* mat,const float *data)
{
    uint16_t i, j;
    for (i = 0; i < mat->row; i++)
    {
        for (j = 0; j < mat->column; j++)
        {
            mat->data[i][j] = data[i*mat->column+j];
        }
    }
}

/*
mat=mat1+mat2
*/
Matrix_t add_mat(const Matrix_t* mat1, const Matrix_t* mat2)
{

    if (mat1->row != mat2->row)
    {
        printf("error, in add_mat: mat1->row != mat2->row\n");
        exit(1);
    }
    if (mat1->column != mat2->column)
    {
        printf("error, in add_mat: mat1->column != mat2->column\n");
        exit(1);
    }
    Matrix_t mat;
    uint16_t i, j;
    mat = create_mat(mat1->row, mat1->column);
    for (i = 0; i < mat1->row; i++)
    {
        for (j = 0; j < mat1->column; j++)
            mat.data[i][j] = mat1->data[i][j] + mat2->data[i][j];
    }
    return mat;
}

/*
transpose the matrix, mat=mat'
*/
Matrix_t transpose_mat(const Matrix_t* mat)//mat'
{
    Matrix_t mat_T;
    mat_T = create_mat(mat->column, mat->row);
    uint16_t i, j;
    for (i = 0; i < mat->column; i++)
    {
        for (j = 0; j < mat->row; j++)
        {
            mat_T.data[i][j] = mat->data[j][i];
        }
    }
    return mat_T;
}

/*
mat=mat1*mat2
*/
Matrix_t mult_mat(const Matrix_t *mat1, const Matrix_t* mat2)
{
    Matrix_t mat;
    if (mat1->column != mat2->row)
    {
        printf("error,In mult_mat: mat1->column != mat2->row\n");
        exit(1);
    }
    else
    {
        mat = create_mat(mat1->row, mat2->column);
        clear_mat(&mat);
        uint16_t i, j;
        for (i = 0; i < mat1->row; i++)
        {
            for (j = 0; j < mat2->column; j++)
            {
                uint16_t m;
                for (m = 0; m < mat1->column; m++)
                {
                    mat.data[i][j] += mat1->data[i][m] * mat2->data[m][j];
                }
            }
        }
    }
    return mat;
}

/*
get matrix's derterminent value
*/
float det_mat(Matrix_t *m)
{
    uint16_t i, j, n, max_row;
    char swap_f;
    float max, k;
    float det=1;
    swap_f = 0;
    if (m->column != m->row)
    {
        printf("error:In det_mat (m->column != m->row)\n");
        exit(1);
    }
    Matrix_t mat = copy_mat(m);
    for (i = 0; i < mat.row-1; i++)
    {
        max = fabs(mat.data[i][i]);
        max_row = i;
        for (j = i + 1; j < mat.row; j++)
        {
            if (max < fabs(mat.data[j][i]))
            {
                max = fabs(mat.data[j][i]);
                max_row = j;
            }
        }
        if (i != max_row)
        {
            swap_row_mat(&mat, i, max_row);
            swap_f++;
        }
        for (j = i + 1; j < mat.row; j++)
        {
            k = -mat.data[j][i]/mat.data[i][i];
            for (n= 0; n < mat.column; n++)
            {
                mat.data[j][n] = mat.data[i][n] * k + mat.data[j][n];
            }
        }
    }
    if (swap_f%2==1)swap_f = -1;
    else swap_f = 1;
    det = 1;
    for (i = 0; i < mat.column; i++)
        det *= mat.data[i][i];
    det *= swap_f;
    //show_mat("in det_mat: mat_upper",&mat);
    free_mat(&mat);
    return det;
}

/*
get inverse matrix
use main column element of Gauss-Jordan algrithm: A|I  --->  I|A^(-1)
*/
Matrix_t inverse_mat(Matrix_t* m)
{
    if (det_mat(m) == 0)
    {
        printf("error: In inverse_mat(det_mat(mat) == 0)\n");
        exit(1);
    }

    Matrix_t mat = copy_mat(m);
    Matrix_t inv_mat = MatEye(m->row);

    int i, j, n,max_row;
    int swap_f = 0;
    float max,k;
    for (i = 0; i < mat.row - 1; i++)
    {
        max = fabs(mat.data[i][i]);
        max_row = i;
        for (j = i + 1; j < mat.row; j++)
        {
            if (max < fabs(mat.data[j][i]))
            {
                max = fabs(mat.data[j][i]);
                max_row = j;
            }
        }
        if (i != max_row)
        {
            swap_row_mat(&mat, i, max_row);
            swap_row_mat(&inv_mat, i, max_row);
            swap_f++;
        }
        for (j = i + 1; j < mat.row; j++)
        {
            k = -mat.data[j][i] / mat.data[i][i];
            for (n = 0; n < mat.column; n++)
            {
                mat.data[j][n] = mat.data[i][n] * k + mat.data[j][n];
                inv_mat.data[j][n] = inv_mat.data[i][n] * k + inv_mat.data[j][n];
            }
        }
    }

    for (i = 0; i < mat.row; i++)
    {
        k = 1/mat.data[i][i];
        scale_row_mat(&mat,i, k);
        scale_row_mat(&inv_mat, i, k);
    }
    for (i = mat.row-1; i>0; i--)
    {
        for (j = i - 1; j >=0; j--)
        {
            k = -mat.data[j][i] / mat.data[i][i];
            for (n = 0; n < mat.column; n++)
            {
                mat.data[j][n] = mat.data[j][n] + k*mat.data[i][n];
                inv_mat.data[j][n] = inv_mat.data[j][n] + k*inv_mat.data[i][n];
            }
        }

    }
    //show_mat("in inverse_mat: mat", &mat);
    //show_mat("in inverse_mat: inv_mat", &inv_mat);
    free_mat(&mat);
    return inv_mat;
}

Matrix_t copy_mat(Matrix_t* src)
{
	int row, column;
	Matrix_t dst;
	dst = create_mat(src->row, src->column);

	if( src->row != dst.row || src->column != dst.column)
	{
		printf("err check, unmathed matrix for MatCopy\n");
		return dst;
	}

	for(row = 0 ; row < src->row ; row++)
	{
		for(column = 0 ; column < src->column ; column++)
			dst.data[row][column] = src->data[row][column];
	}
	return dst;
}

Matrix_t MatEye(u16 i)
{
	Matrix_t mat = create_mat(i, i);

	MatZeros(mat);
	for(i = 0 ; i < min(mat.row, mat.column) ; i++)
		mat.data[i][i] = 1.0f;

	return mat;
}

Matrix_t MatZeros(Matrix_t mat)
{
	int row,column;

	for(row = 0 ; row < mat.row ; row++)
		for(column = 0 ; column < mat.column ; column++)
			mat.data[row][column] = 0.0f;

	return mat;
}

void MatDump(const Matrix_t mat)
{
	int row,column;

	printf("Mat %dx%d:\n", mat.row, mat.column);
	for(row = 0 ; row < mat.row ; row++){
		for(column = 0 ; column < mat.column ; column++){
			printf("%.4f\t", mat.data[row][column]);
		}
		printf("\n");
	}
}

void swap_row_mat(Matrix_t* mat,u16 i, u16 j)
{
	float tmp;
	u16 column;
	for(column = 0 ; column < mat->column ; column++)
	{
		tmp = mat->data[i][column];
		mat->data[i][column] = mat->data[j][column];
		mat->data[j][column] = tmp;
	}
}

void scale_row_mat(Matrix_t* mat, u16 i, float k)
{
	u16 n;
    for (n = 0; n < mat->column; n++)
    {
        mat->data[i][n] = mat->data[i][n] * k;
    }
}
