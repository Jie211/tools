#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cuda_runtime.h>
#include <helper_cuda.h>
#include <helper_functions.h>

// texture<int2, cudaTextureType1D, cudaReadModeElementType> texture_;
#define SIZE 10

extern void __host__ 
bxcreate(const double *a,
         double *b,
         double *x,
         const int n_data);

extern void __host__ 
CSR(const double *a,
    double *val,
    int *col,
    int *ptr,
    const int n_data,
    const int m);

extern __host__ void
display_mat(const double *a, 
            const int n_data);

extern __host__ void
display_vec(const double *a, 
            const int n_data);

extern __host__ void
list_vec_i(const int *a, 
            const int n_data);

extern __host__ void
list_vec_d(const double *a, 
            const int n_data);

extern __host__ void
CSR(const double *a,
    double *val,
    int *col,
    int *ptr,
    const int n_data,
    const int m);

extern __host__ void
cgm_CRS(double *val,
    const int *col,
    const int *ptr,
    double *bvec,
    double *xvec,
    const int ndata,
    const double eps,
    const int i_max,
    const int m);

extern __host__ void
cgm_JAD(double *val,
    const int *col,
    const int *perm, 
    const int *nz, 
    const int *jd,
    double *bvec,
    double *xvec,
    const int ndata,
    const double eps,
    const int i_max,
    const int m, 
    const int max_nz);


extern __global__ void
spmv_crs_kernel(const int ndata,
                const int *ptr,
                const int *col,
                const double *val,
                const double *x,
                double *y);

extern __global__ void
spmv_crs_kernel_readonly(const int ndata,
                        const int *ptr,
                        const int *col,
                        const double *val,
                        const double *__restrict__ x,
                        double *y);

extern __global__ void
spmv_crs_kernel_warpshuffle(const int ndata,
                        const int *ptr,
                        const int *col,
                        const double *val,
                        const double *__restrict__ x,
                        double *y);

extern __global__ void
spmv_crs_kernel_basic(const int num_rows, 
                        const int *ptr, 
                        const int *indices, 
                        const double *data, 
                        const double *x, 
                        double *y);
extern __global__ void
spmv_jad_kernel_basic(const int num_rows, 
                        const int *perm, 
                        const int *nz, 
                        const int *jd,
                        const double *mat, 
                        const int *col,
                        const double *vec, 
                        double *y);

extern __host__ void 
usage(const char *a,const char *b,const char *x, int *ndata, int *m);

extern __host__ void 
getmatrix(const char *file,double *a,int ndata,int m);

extern __host__ void
getvector(const char *file,double *v,int ndata);

extern __host__ void
gethead(const char *file1, const char *file2, const char *file3, int *N, int *NZ);

extern __host__ void
getdata(const char *file1, const char *file2, const char *file3,int *col,int *ptr,double *val,double *b,double *x, int N, int NZ);

__device__ __inline__ double shfl_xor(double value, int const lane)
{
 return  __hiloint2double(__shfl_xor(__double2hiint(value), lane), __shfl_xor(__double2loint(value), lane)); 
}
extern __host__ void
gethead_jad(const char *file1, const char *file2, const char *file3, const char *file4,int *N, int *NZ);

extern __host__ void 
getmaxnz_jad(const char *file1, int *MAXNZ);

extern __host__ void 
getdata_jad(const char *file1, const char *file2, const char *file3,const char *file4, int *col,int *perm,int *nz, int *jd, double *val,double *b,double *x, int N, int NZ, int MAXNZ);

