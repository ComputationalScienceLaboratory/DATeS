/*        Generated by TAPENADE     (INRIA, Tropics team)
    Tapenade 3.10 (r5363) -  9 Sep 2014 09:54
*/
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>
#include "swe_parameters.h"
#include "rok_instrumentation.h"

void swe_fun_d_init();
void swe_fun_d_cleanup();
void swe_fun_d(double t, double X[], double Xd[], double Yd[]);
void swe_fun_main_d(int M, int N, double X[], double Xd[], double dx, double dy, double Yd[]);

// timing data
double fund_time;
double fund_count;

#define Xdim (NX+2)
#define Ydim (NY+2)

//double tx2d[Xdim][Ydim], tu2d[Xdim][Ydim], tv2d[Xdim][Ydim];
//double tx2dd[Xdim][Ydim], tu2dd[Xdim][Ydim], tv2dd[Xdim][Ydim];
double Hx[Xdim-1][Ydim-1],Ux[Xdim-1][Ydim-1],Vx[Xdim-1][Ydim-1];
double Hxd[Xdim-1][Ydim-1], Uxd[Xdim-1][Ydim-1], Vxd[Xdim-1][Ydim-1];
double Hy[Xdim-1][Ydim-1], Uy[Xdim-1][Ydim-1], Vy[Xdim-1][Ydim-1];
double Hyd[Xdim-1][Ydim-1], Uyd[Xdim-1][Ydim-1], Vyd[Xdim-1][Ydim-1];


void swe_fun_d_init() {
   fund_count = 0;
   fund_time = 0.0;
}

void swe_fun_d_cleanup() {
   rok_record_time(fund_time/fund_count, TIMER_SWE_FUN_D);
}

/*  Hint: ISIZE1OFY should be the size of dimension 1 of array Y*/

/*
  Differentiation of swe_fun_main in forward (tangent) mode:
   variations   of useful results: Y[_:_]
   with respect to varying inputs: X[_:_]
   Plus diff mem management of: Hx:in Hx[0:512+2-1-1]:in Hy:in
                Hy[0:512+2-1-1]:in Ux:in Ux[0:512+2-1-1]:in Uy:in
                Uy[0:512+2-1-1]:in Vx:in Vx[0:512+2-1-1]:in Vy:in
                Vy[0:512+2-1-1]:in X:in Y:in
*/
void swe_fun_main_d(int M, int N, double X[], double Xd[], double dx, double 
        dy, double Yd[]) {
    int i, j;
//    int ii2;
//    int ii1;
    //   int l;
    //   l = M * N;
    // Initialize derivatives
#pragma omp parallel private(i, j) default(shared)
{
    #pragma omp for schedule(static)
    for (i = 0; i < M-1; ++i)
        for (j = 0; j < N-1; ++j) {
            Hxd[i][j] = 0.0;
            Hx[i][j] = 0.0;
        }
    #pragma omp for schedule(static)
    for (i = 0; i < M-1; ++i)
        for (j = 0; j < N-1; ++j) {
            Hyd[i][j] = 0.0;
            Hy[i][j] = 0.0;
        }
    #pragma omp for schedule(static)
    for (i = 0; i < M-1; ++i)
        for (j = 0; j < N-1; ++j) {
            Uxd[i][j] = 0.0;
            Ux[i][j] = 0.0;
        }
    #pragma omp for schedule(static)
    for (i = 0; i < M-1; ++i)
        for (j = 0; j < N-1; ++j) {
            Uyd[i][j] = 0.0;
            Uy[i][j] = 0.0;
        }
    #pragma omp for schedule(static)
    for (i = 0; i < M-1; ++i)
        for (j = 0; j < N-1; ++j) {
            Vxd[i][j] = 0.0;
            Vx[i][j] = 0.0;
        }
    #pragma omp for schedule(static)
    for (i = 0; i < M-1; ++i)
        for (j = 0; j < N-1; ++j) {
            Vyd[i][j] = 0.0;
            Vy[i][j] = 0.0;
        }
}
//    for (ii1 = 0; ii1 < 513; ++ii1)
//        for (ii2 = 0; ii2 < 513; ++ii2)
//            Hxd[ii1][ii2] = 0.0;
    /*     for (i = 0; i < M; i++) {
      for(j = 0; j < N; j++) {
         int p = i * M + j;
         tx2d[i][j] = X[p];
         tu2d[i][j] = X[l+p];
         tv2d[i][j] = X[2*l+p];
      }
   } */
    // Unpack current state
    //#define x2d(V, I, J) (tx2d[(I)][(J)])
    //#define u2d(V, I, J) (tu2d[(I)][(J)])
    //#define v2d(V, I, J) (tv2d[(I)][(J)])
    // Compute x-direction finite differences
#pragma omp parallel private(i, j) default(shared)
{
    #pragma omp for schedule(static)
    for (i = 0; i < M-1; ++i)
        for (j = 0; j < N-2; ++j) {
            Hxd[i][j] = (Xd[(i+1)*M+(j+1)]+Xd[i*M+(j+1)])/2.0;
            Hx[i][j] = (X[(i+1)*M+(j+1)]+X[i*M+(j+1)])/2.0;
        }
//    for (ii1 = 0; ii1 < 513; ++ii1)
//        for (ii2 = 0; ii2 < 513; ++ii2)
//            Uxd[ii1][ii2] = 0.0;
    #pragma omp for schedule(static)
    for (i = 0; i < M-1; ++i)
        for (j = 0; j < N-2; ++j) {
            Uxd[i][j] = (Xd[M*N+(i+1)*M+(j+1)]+Xd[M*N+i*M+(j+1)])/2.0;
            Ux[i][j] = (X[M*N+(i+1)*M+(j+1)]+X[M*N+i*M+(j+1)])/2.0;
        }
//    for (ii1 = 0; ii1 < 513; ++ii1)
//        for (ii2 = 0; ii2 < 513; ++ii2)
//            Vxd[ii1][ii2] = 0.0;
    #pragma omp for schedule(static)
    for (i = 0; i < M-1; ++i)
        for (j = 0; j < N-2; ++j) {
            Vxd[i][j] = (Xd[2*M*N+(i+1)*M+(j+1)]+Xd[2*M*N+i*M+(j+1)])/2.0;
            Vx[i][j] = (X[2*M*N+(i+1)*M+(j+1)]+X[2*M*N+i*M+(j+1)])/2.0;
        }
//    for (ii1 = 0; ii1 < 513; ++ii1)
//        for (ii2 = 0; ii2 < 513; ++ii2)
//            Hyd[ii1][ii2] = 0.0;
    // Compute y-direction finite differences
    #pragma omp for schedule(static)
    for (i = 0; i < M-2; ++i)
        for (j = 0; j < N-1; ++j) {
            Hyd[i][j] = (Xd[(i+1)*M+(j+1)]+Xd[(i+1)*M+j])/2.0;
            Hy[i][j] = (X[(i+1)*M+(j+1)]+X[(i+1)*M+j])/2.0;
        }
//    for (ii1 = 0; ii1 < 513; ++ii1)
//        for (ii2 = 0; ii2 < 513; ++ii2)
//            Uyd[ii1][ii2] = 0.0;
    #pragma omp for schedule(static)
    for (i = 0; i < M-2; ++i)
        for (j = 0; j < N-1; ++j) {
            Uyd[i][j] = (Xd[M*N+(i+1)*M+(j+1)]+Xd[M*N+(i+1)*M+j])/2.0;
            Uy[i][j] = (X[M*N+(i+1)*M+(j+1)]+X[M*N+(i+1)*M+j])/2.0;
        }
//    for (ii1 = 0; ii1 < 513; ++ii1)
//        for (ii2 = 0; ii2 < 513; ++ii2)
//            Vyd[ii1][ii2] = 0.0;
    #pragma omp for schedule(static)
    for (i = 0; i < M-2; ++i)
        for (j = 0; j < N-1; ++j) {
            Vyd[i][j] = (Xd[2*M*N+(i+1)*M+(j+1)]+Xd[2*M*N+(i+1)*M+j])/2.0;
            Vy[i][j] = (X[2*M*N+(i+1)*M+(j+1)]+X[2*M*N+(i+1)*M+j])/2.0;
        }
}
//    for (ii1 = 0; ii1 < ISIZE1OFY; ++ii1)
//        Yd[ii1] = 0.0;
    // Evaluate shallow water equations in 2D
#pragma omp parallel private(i, j) default(shared)
{
    #pragma omp for schedule(static)
    for (i = 1; i < M-1; ++i)
        for (j = 1; j < N-1; ++j) {
            Yd[i*M + j] = -((Uxd[i][j-1]-Uxd[i-1][j-1])/dx) - (Vyd[i-1][j]-Vyd
                [i-1][j-1])/dy;
//            Y[i*M + j] = -(1.0/dx)*(Ux[i][j-1]-Ux[i-1][j-1]) - 1.0/dy*(Vy[i-1]
//                [j]-Vy[i-1][j-1]);
        }
    #pragma omp for schedule(static)
    for (i = 1; i < M-1; ++i)
        for (j = 1; j < N-1; ++j) {
            Yd[M*N + i*M + j] = -((((Uxd[i][j-1]*Ux[i][j-1]+Ux[i][j-1]*Uxd[i][
                j-1])*Hx[i][j-1]-Ux[i][j-1]*Ux[i][j-1]*Hxd[i][j-1])/(Hx[i][j-1
                ]*Hx[i][j-1])+g*(Hxd[i][j-1]*Hx[i][j-1]+Hx[i][j-1]*Hxd[i][j
                -1])/2.0-((Uxd[i-1][j-1]*Ux[i-1][j-1]+Ux[i-1][j-1]*Uxd[i-1][j-
                1])*Hx[i-1][j-1]-Ux[i-1][j-1]*Ux[i-1][j-1]*Hxd[i-1][j-1])/(Hx[
                i-1][j-1]*Hx[i-1][j-1])-g*(Hxd[i-1][j-1]*Hx[i-1][j-1]+Hx[i-
                1][j-1]*Hxd[i-1][j-1])/2.0)/dx) - (((Vyd[i-1][j]*Uy[i-1][j]+Vy
                [i-1][j]*Uyd[i-1][j])*Hy[i-1][j]-Vy[i-1][j]*Uy[i-1][j]*Hyd[i-1
                ][j])/(Hy[i-1][j]*Hy[i-1][j])-((Vyd[i-1][j-1]*Uy[i-1][j-1]+Vy[
                i-1][j-1]*Uyd[i-1][j-1])*Hy[i-1][j-1]-Vy[i-1][j-1]*Uy[i-1][j-1
                ]*Hyd[i-1][j-1])/(Hy[i-1][j-1]*Hy[i-1][j-1]))/dy;
//            Y[M*N + i*M + j] = -(1.0/dx)*(Ux[i][j-1]*Ux[i][j-1]/Hx[i][j-1]+
//                g/2.0*Hx[i][j-1]*Hx[i][j-1]-(Ux[i-1][j-1]*Ux[i-1][j-1]/Hx[i
//                -1][j-1]+g/2.0*Hx[i-1][j-1]*Hx[i-1][j-1])) - 1.0/dy*(Vy[i-1
//                ][j]*Uy[i-1][j]/Hy[i-1][j]-Vy[i-1][j-1]*Uy[i-1][j-1]/Hy[i-1][j
//                -1]);
        }
    #pragma omp for schedule(static)
    for (i = 1; i < M-1; ++i)
        for (j = 1; j < N-1; ++j) {
            Yd[2*M*N + i*M + j] = -((((Uxd[i][j-1]*Vx[i][j-1]+Ux[i][j-1]*Vxd[i
                ][j-1])*Hx[i][j-1]-Ux[i][j-1]*Vx[i][j-1]*Hxd[i][j-1])/(Hx[i][j
                -1]*Hx[i][j-1])-((Uxd[i-1][j-1]*Vx[i-1][j-1]+Ux[i-1][j-1]*Vxd[
                i-1][j-1])*Hx[i-1][j-1]-Ux[i-1][j-1]*Vx[i-1][j-1]*Hxd[i-1][j-1
                ])/(Hx[i-1][j-1]*Hx[i-1][j-1]))/dx) - (((Vyd[i-1][j]*Vy[i-1][j
                ]+Vy[i-1][j]*Vyd[i-1][j])*Hy[i-1][j]-Vy[i-1][j]*Vy[i-1][j]*Hyd
                [i-1][j])/(Hy[i-1][j]*Hy[i-1][j])+g*(Hyd[i-1][j]*Hy[i-1][j]
                +Hy[i-1][j]*Hyd[i-1][j])/2.0-((Vyd[i-1][j-1]*Vy[i-1][j-1]+Vy[i
                -1][j-1]*Vyd[i-1][j-1])*Hy[i-1][j-1]-Vy[i-1][j-1]*Vy[i-1][j-1]
                *Hyd[i-1][j-1])/(Hy[i-1][j-1]*Hy[i-1][j-1])-g*(Hyd[i-1][j-1
                ]*Hy[i-1][j-1]+Hy[i-1][j-1]*Hyd[i-1][j-1])/2.0)/dy;
//            Y[2*M*N + i*M + j] = -(1.0/dx)*(Ux[i][j-1]*Vx[i][j-1]/Hx[i][j-1]-
//                Ux[i-1][j-1]*Vx[i-1][j-1]/Hx[i-1][j-1]) - 1.0/dy*(Vy[i-1][j]*
//                Vy[i-1][j]/Hy[i-1][j]+g/2.0*Hy[i-1][j]*Hy[i-1][j]-(Vy[i-1][
//                j-1]*Vy[i-1][j-1]/Hy[i-1][j-1]+g/2.0*Hy[i-1][j-1]*Hy[i-1][j
//                -1]));
        }
}
    // Fill in boundary conditions
#pragma omp parallel private(i, j) default(shared)
{
    #pragma omp for schedule(static)
    for (i = 0; i < M; ++i) {
        Yd[i*M + 0] = Yd[i*M + 1];
//        Y[i*M + 0] = Y[i*M + 1];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < M; ++i) {
        Yd[M*N + i*M + 0] = Yd[M*N + i*M + 1];
//        Y[M*N + i*M + 0] = Y[M*N + i*M + 1];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < M; ++i) {
        Yd[2*M*N + i*M + 0] = -Yd[2*M*N+i*M+1];
//        Y[2*M*N + i*M + 0] = -Y[2*M*N+i*M+1];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < M; ++i) {
        Yd[i*M + (N - 1)] = Yd[i*M + (N - 2)];
//        Y[i*M + (N - 1)] = Y[i*M + (N - 2)];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < M; ++i) {
        Yd[M*N + i*M + (N - 1)] = Yd[M*N + i*M + (N - 2)];
//        Y[M*N + i*M + (N - 1)] = Y[M*N + i*M + (N - 2)];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < M; ++i) {
        Yd[2*M*N + i*M + (N - 1)] = -Yd[2*M*N+i*M+(N-2)];
//        Y[2*M*N + i*M + (N - 1)] = -Y[2*M*N+i*M+(N-2)];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < N; ++i) {
        Yd[0*M + i] = Yd[1*M + i];
//        Y[0*M + i] = Y[1*M + i];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < N; ++i) {
        Yd[M*N + 0*M + i] = -Yd[M*N+1*M+i];
//        Y[M*N + 0*M + i] = -Y[M*N+1*M+i];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < N; ++i) {
        Yd[2*M*N + 0*M + i] = Yd[2*M*N + 1*M + i];
//        Y[2*M*N + 0*M + i] = Y[2*M*N + 1*M + i];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < N; ++i) {
        Yd[(M-1)*M + i] = Yd[(M-2)*M + i];
//        Y[(M-1)*M + i] = Y[(M-2)*M + i];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < N; ++i) {
        Yd[M*N + (M-1)*M + i] = -Yd[M*N+(M-2)*M+i];
//        Y[M*N + (M-1)*M + i] = -Y[M*N+(M-2)*M+i];
    }
    #pragma omp for schedule(static)
    for (i = 0; i < N; ++i) {
        Yd[2*M*N + (M-1)*M + i] = Yd[2*M*N + (M-2)*M + i];
//        Y[2*M*N + (M-1)*M + i] = Y[2*M*N + (M-2)*M + i];
    }
}
    /*    for(i=0;i<M;i++)
   {
         for(j=0;j<N;j++)
         {
               int p = i*M+j;
               Y[p]=tx2d[i][j];
               Y[l+p]=tu2d[i][j];
               Y[2*l+p]=tv2d[i][j];
         }
   } */
    // Repack
    return;
}

/*
  Differentiation of swe_fun in forward (tangent) mode:
   variations   of useful results: Y[_:_]
   with respect to varying inputs: X[_:_]
   RW status of diff variables: Hx[0:512+2-1-1][0:512+2-1-1]:(loc)
                Hy[0:512+2-1-1][0:512+2-1-1]:(loc) Ux[0:512+2-1-1][0:512+2-1-1]:(loc)
                Uy[0:512+2-1-1][0:512+2-1-1]:(loc) Vx[0:512+2-1-1][0:512+2-1-1]:(loc)
                Vy[0:512+2-1-1][0:512+2-1-1]:(loc) X[_:_]:in Y[_:_]:out
   Plus diff mem management of: tv2d:in-out tv2d[0:512+2-1]:in-out
                tx2d:in-out tx2d[0:512+2-1]:in-out tu2d:in-out
                tu2d[0:512+2-1]:in-out Hx:in-out Hx[0:512+2-1-1]:in-out
                Hy:in-out Hy[0:512+2-1-1]:in-out Ux:in-out Ux[0:512+2-1-1]:in-out
                Uy:in-out Uy[0:512+2-1-1]:in-out Vx:in-out Vx[0:512+2-1-1]:in-out
                Vy:in-out Vy[0:512+2-1-1]:in-out X:in Y:in-out
*/
void swe_fun_d(double t, double X[], double Xd[], double Yd[]) {
    struct timeval t1, t2;
    gettimeofday(&t1, (void *)0);
    swe_fun_main_d(NX + 2, NY + 2, X, Xd, DX, DY, Yd);
    gettimeofday(&t2, (void *)0);
    fund_time += 1000.0*(t2.tv_sec-t1.tv_sec) + 1.0/1000.0*(t2.tv_usec-t1.tv_usec);
    fund_count++;
}
