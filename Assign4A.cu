// %%writefile Assign4A.cu

#include<iostream>
#include<chrono>
#include<cuda_runtime.h>

using namespace std;
using namespace std::chrono;

__global__ void add(int * A, int * B, int * C, int size){
    int tid=blockIdx.x * blockDim.x + threadIdx.x;

    if(tid<size){
        C[tid]=A[tid]+B[tid];
    }
}

void initialize(int * vector,int size){
    for(int i=0; i<size; i++){
        cout<<"Enter element "<<i+1<<" for vector : "; cin>>vector[i];
    }
}

void print(int * vector, int size){
    for(int i=0; i<size; i++){
        cout<<vector[i]<<" ";
    }
    cout<<endl;
}

void addSequential(int * A, int * B, int * C, int size){
    for(int i=0; i<size; i++){
        C[i]=A[i]+B[i];
    }
}

int main(){

    int N;
    cout<<"Enter the size of the vectors : "; cin>>N;

    int* A,*B,*C;

    int vsize=N;

    size_t vectorBytes=vsize*sizeof(int);

    A=new int[vsize];
    B=new int[vsize];
    C=new int[vsize];

    initialize(A,vsize);
    initialize(B,vsize);

    cout<<"Vector A : "; 
    print(A,vsize);
    cout<<"Vector B : ";
    print(B,vsize);

    int *X,*Y,*Z;
    cudaMalloc(&X,vectorBytes);
    cudaMalloc(&Y,vectorBytes);
    cudaMalloc(&Z,vectorBytes);

    cudaMemcpy(X,A,vectorBytes,cudaMemcpyHostToDevice);
    cudaMemcpy(Y,B,vectorBytes,cudaMemcpyHostToDevice);

    int threadsPerBlock=256;
    int blocksPerGrid=(N+threadsPerBlock-1)/threadsPerBlock;

    //seq add
    auto start=high_resolution_clock::now();
    addSequential(A,B,C,N);
    auto stop=high_resolution_clock::now();
    auto duration=duration_cast<microseconds>(stop-start);

    cout<<"Sequential addition : ";
    print(C,N);

    cout<<"Sequential addition time : "<<duration.count()<<" microseconds"<<endl;

    //parallel add
    start=high_resolution_clock::now();
    add<<<blocksPerGrid,threadsPerBlock>>>(X,Y,Z,N);
    cudaMemcpy(C,Z,vectorBytes,cudaMemcpyDeviceToHost);
    stop=high_resolution_clock::now();
    duration=duration_cast<microseconds>(stop-start);

    cout<<"Parallel addition : ";
    print(C,N);

    cout<<"Parallel addition time : "<<duration.count()<<" microseconds"<<endl;

    delete[] A;
    delete[] B;
    delete[] C;

    cudaFree(X);
    cudaFree(Y);
    cudaFree(Z);
    return 0;
}

// !nvcc Assign4A.cu -o Assign4A
// ! ./Assign4A