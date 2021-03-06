//**********************************************************************
//* This file is a part of the CANUPO project, a set of programs for   *
//* classifying automatically 3D point clouds according to the local   *
//* multi-scale dimensionality at each point.                          *
//*                                                                    *
//* Author & Copyright: Nicolas Brodu <nicolas.brodu@numerimoire.net>  *
//*                                                                    *
//* This project is free software; you can redistribute it and/or      *
//* modify it under the terms of the GNU Lesser General Public         *
//* License as published by the Free Software Foundation; either       *
//* version 2.1 of the License, or (at your option) any later version. *
//*                                                                    *
//* This library is distributed in the hope that it will be useful,    *
//* but WITHOUT ANY WARRANTY; without even the implied warranty of     *
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *
//* Lesser General Public License for more details.                    *
//*                                                                    *
//* You should have received a copy of the GNU Lesser General Public   *
//* License along with this library; if not, write to the Free         *
//* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,    *
//* MA  02110-1301  USA                                                *
//*                                                                    *
//**********************************************************************/
/*
This is just a wrapper around LAPACK's dgesvd and sgesvd function
*/
#ifndef CANUPO_LSQ_H
#define CANUPO_LSQ_H

#include <iostream>
#include <algorithm>

#include <stdlib.h>

extern "C" {
    // least square solving from lapack
    void dgelsd_ (const int* m, const int* n, const int* nrhs, double* A, const int* lda, double* B, const int* ldb, double* S, const double* rcond, int* rank, double* work, const int* lwork, int* iwork, int* info);
    void sgelsd_ (const int* m, const int* n, const int* nrhs, float* A, const int* lda, float* B, const int* ldb, float* S, const float* rcond, int* rank, float* work, const int* lwork, int* iwork, int* info);
}

// Our own somewhat simplified wrapper. See below for a simpler one where leadingA is nrows by default
void leastSquares(double* A, int leadingA, int nrows, int ncols, double* B, int nrhs) {
#ifndef LAPACK_IS_THREAD_SAFE
#pragma omp critical
{
#endif
    int info = 0;
    int ldb = std::max(nrows,ncols);
    double *sval = new double[std::min(nrows,ncols)];
    double rcond = -1;
    int rank;
    // first, workspace query
    int lwork = -1;
    double tmpwork;
    int tmpiwork;
    dgelsd_(&nrows, &ncols, &nrhs, A, &leadingA, B, &ldb, sval, &rcond, &rank, &tmpwork, &lwork, &tmpiwork, &info);
    if (info) {
        std::cerr << "Could not retrieve the work array size for lapack" << std::endl;
        exit(1);
    }
    lwork = (int)tmpwork;
    double* work = new double[lwork];
    // doc says read size of the int array in tmpiwork, but debugging says it is irrelevant
    // => doc says min required sizes and formula for work is > formula for iwork in all cases: reuse lwork
    int* iwork = new int[lwork]; 
    dgelsd_(&nrows, &ncols, &nrhs, A, &leadingA, B, &ldb, sval, &rcond, &rank, work, &lwork, iwork, &info);
    if (info) {
        std::cerr << "Error in dgelsd: " << info << std::endl;
        exit(1);
    }
    delete [] iwork;
    delete [] work;
    delete [] sval;
#ifndef LAPACK_IS_THREAD_SAFE
}
#endif
}

inline void leastSquares(double* A, int nrows, int ncols, double* B, int nrhs) {
    leastSquares(A,nrows,nrows,ncols,B,nrhs);
}

void leastSquares(float* A, int leadingA, int nrows, int ncols, float* B, int nrhs) {
#ifndef LAPACK_IS_THREAD_SAFE
#pragma omp critical
{
#endif
    int info = 0;
    int ldb = std::max(nrows,ncols);
    float *sval = new float[std::min(nrows,ncols)];
    float rcond = -1;
    int rank;
    // first, workspace query
    int lwork = -1;
    float tmpwork;
    int tmpiwork;
    sgelsd_(&nrows, &ncols, &nrhs, A, &leadingA, B, &ldb, sval, &rcond, &rank, &tmpwork, &lwork, &tmpiwork, &info);
    if (info) {
        std::cerr << "Could not retrieve the work array size for lapack" << std::endl;
        exit(1);
    }
    lwork = (int)tmpwork;
    float* work = new float[lwork];
    // doc says read size of the int array in tmpiwork, but debugging says it is irrelevant
    // => doc says min required sizes and formula for work is > formula for iwork in all cases: reuse lwork
    int* iwork = new int[lwork]; 
    sgelsd_(&nrows, &ncols, &nrhs, A, &leadingA, B, &ldb, sval, &rcond, &rank, work, &lwork, iwork, &info);
    if (info) {
        std::cerr << "Error in sgelsd: " << info << std::endl;
        exit(1);
    }
    delete [] iwork;
    delete [] work;
    delete [] sval;
#ifndef LAPACK_IS_THREAD_SAFE
}
#endif
}
void leastSquares(float* A, int nrows, int ncols, float* B, int nrhs) {
    leastSquares(A,nrows,nrows,ncols,B,nrhs);
}

#endif
