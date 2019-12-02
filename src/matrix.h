/*
 * matrix.h
 *
 *  Created on: 2018年7月21日
 *      Author: wjy
 */

#ifndef SRC_MATRIX_H_
#define SRC_MATRIX_H_

#include <xil_types.h>
#include <math.h>

#define min(a, b) ((a) > (b) ? (b) : (a))

typedef struct
{
    u16 row;
    u16 column;
    float **data;
}Matrix_t;

Matrix_t create_mat(uint16_t row, uint16_t column);
void clear_mat(Matrix_t* mat);
void free_mat(Matrix_t *mat);
void set_mat_data(Matrix_t* mat,const float *data);
Matrix_t add_mat(const Matrix_t* mat1, const Matrix_t* mat2);
Matrix_t transpose_mat(const Matrix_t* mat);//mat'
Matrix_t mult_mat(const Matrix_t *mat1, const Matrix_t* mat2);
float det_mat(Matrix_t *m);
Matrix_t inverse_mat(Matrix_t* m);
Matrix_t copy_mat(Matrix_t* src);
Matrix_t MatEye(u16 i);
Matrix_t MatZeros(Matrix_t mat);
void MatDump(const Matrix_t mat);
void swap_row_mat(Matrix_t* mat,u16 i, u16 j);
void scale_row_mat(Matrix_t* mat, u16 i, float k);

#endif /* SRC_MATRIX_H_ */
