%%writefile Assign4B.cu

#include<iostream>
#include<chrono>
#include<cuda_runtime.h>

using namespace std;
using namespace std::chrono;


__global__ void mulPar(int * A, int * B, int * C, int M,int N, int K){
    int row=blockIdx.y*blockDim.y + threadIdx.y;
    int col=blockIdx.x*blockDim.x+ threadIdx.x;

    if(row<M && col<K){
        int sum=0;
        for(int i=0; i<N; i++){
            sum+=A[row*N+i]+B[i*K+col];
        }
        C[row*K+col]=sum;
    }

}
void initialize(int * matrix, int rows, int cols){
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            cout<<"Enter the element at "<<i<<" "<<j<<" : "; cin>>matrix[i*cols+j];
        }
    }
}

void print(int * matrix, int rows, int cols){
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
          cout<<matrix[i*cols+j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void mulSeq(int * A, int * B, int * C, int M,int N, int K){
    for(int i=0; i<M; i++){
        for(int j=0; j<K; j++){
            int sum=0;
            for(int k=0; k<N; k++){
                sum+=A[i*N+k]*B[k*K+j];
            }
            C[i*K+j]=sum;
        }
    }
}


int main(){
    int M,N,K;
    cout<<"Enter the MxNxK : "; cin>>M>>N>>K;

    int * A,*B,*C;

    int matsize=M*K;
    size_t matBytes=matsize*sizeof(int);

    A=new int[M*N];
    B=new int[N*K];
    C=new int[M*K];

    initialize(A,M,N);
    initialize(B,N,K);

    print(A,M,N);
    print(B,N,K);

    int *X,*Y,*Z;

    cudaMalloc(&X,M*N*sizeof(int));
    cudaMalloc(&Y,N*K*sizeof(int));
    cudaMalloc(&Z,M*K*sizeof(int));

    cudaMemcpy(X,A,M*N*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(Y,B,N*K*sizeof(int),cudaMemcpyHostToDevice);

    int Threads=16;
    int Blocks=(M+Threads-1)/Threads;

    dim3 threads(Threads,Threads);
    dim3 blocks(Blocks,Blocks);


    // multi seq

    auto start=high_resolution_clock::now();
    mulSeq(A,B,C,M,N,K);
    auto stop=high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop-start);

    cout<<"Mul matrix : "<<endl;
    print(C,M,K);
    cout<<"Duration for seq : "<<duration.count()<<" microseconds"<<endl;


    // mul par
    start=high_resolution_clock::now();
    mulPar<<<blocks, threads>>>(X,Y,Z,M,N,K);
    cudaMemcpy(C,Z,matBytes,cudaMemcpyDeviceToHost);
     stop=high_resolution_clock::now();
     duration = duration_cast<microseconds>(stop-start);

    cout<<"Mul matrix : "<<endl;
    print(C,M,K);
    cout<<"Duration for par : "<<duration.count()<<" microseconds"<<endl;

    delete[] A;
    delete[] B;
    delete[] C;

    cudaFree(X);
    cudaFree(Y);
    cudaFree(Z);
    return 0;
}

// !nvcc filename -o fileobj 
// ! ./fileobj 