#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int minSequential(int arr[],int n){
    int minval=arr[0];
    for(int i=0; i<n; i++){
        if(arr[i]<minval) minval=arr[i];
    }
    return minval;
}

int maxSequential(int arr[],int n){
    int maxval=arr[0];
    for(int i=0; i<n; i++){
        if(arr[i]>maxval) maxval=arr[i];
    }
    return maxval;
}

int sumSequential(int arr[],int n){
    int sum=0;
    for(int i=0; i<n; i++){
        sum+=arr[i];
    }
    return sum;
}

double avgSequential(int arr[],int n){
    return (double)sumSequential(arr,n)/n; 
}

int minParallel(int arr[],int n){
    int minval=arr[0];
    #pragma omp parallel for reduction(min:minval)
    for(int i=0; i<n; i++){
        if(arr[i]<minval) minval=arr[i];
    }
    return minval;
}

int maxParallel(int arr[],int n){
    int maxval=arr[0];
    #pragma omp parallel for reduction(max:maxval)
    for(int i=0; i<n; i++){
        if(arr[i]>maxval) maxval=arr[i];
    }
    return maxval;
}

int sumParallel(int arr[],int n){
    int sum=0;
    #pragma omp parallel for reduction(+:sum)
    for(int i=0; i<n; i++){
        sum+=arr[i];
    }
    return sum;
}

double avgParallel(int arr[],int n){
    return (double)sumParallel(arr,n)/n; 
}

int main(){
    int n;
    cout<<"Enter the number of elements : "; cin>>n;
    int arr[n];
    cout<<"Enter the elements : ";
    for(int i=0; i<n; i++) cin>>arr[i];

    //min seq
    cout<<"Minimum value (sequential) is : ";
    double start=omp_get_wtime();
    cout<<minSequential(arr,n);
    double stop=omp_get_wtime();
    double duration=stop-start;
    cout<<"\nTime taken for min calculation (Sequential) : "<<duration*1e6<<" microseconds"<<endl<<endl;

    // max seq
    cout<<"Maximum value (sequential) is : ";
    start=omp_get_wtime();
    cout<<maxSequential(arr,n);
    stop=omp_get_wtime();
    duration=stop-start;
    cout<<"\nTime taken for max calculation (Sequential) : "<<duration*1e6<<" microseconds"<<endl<<endl;

    // sum seq
    cout<<"Sum of  values (sequential) is : ";
    start=omp_get_wtime();
    cout<<sumSequential(arr,n);
    stop=omp_get_wtime();
    duration=stop-start;
    cout<<"\nTime taken for Sum of values calculation (Sequential) : "<<duration*1e6<<" microseconds"<<endl<<endl;

    //avg seq
    cout<<"Average value (sequential) is : ";
    start=omp_get_wtime();
    cout<<avgSequential(arr,n);
    stop=omp_get_wtime();
    duration=stop-start;
    cout<<"\nTime taken for Average value calculation (Sequential) : "<<duration*1e6<<" microseconds"<<endl<<endl;

    //min par
    cout<<"Minimum value (parallel) is : ";
    start=omp_get_wtime();
    cout<<minParallel(arr,n);
    stop=omp_get_wtime();
    duration=stop-start;
    cout<<"\nTime taken for min calculation (parallel) : "<<duration*1e6<<" microseconds"<<endl<<endl;

    // max par
    cout<<"Maximum value (parallel) is : ";
    start=omp_get_wtime();
    cout<<maxParallel(arr,n);
    stop=omp_get_wtime();
    duration=stop-start;
    cout<<"\nTime taken for max calculation (parallel) : "<<duration*1e6<<" microseconds"<<endl<<endl;

    // sum par
    cout<<"Sum of  values (parallel) is : ";
    start=omp_get_wtime();
    cout<<sumParallel(arr,n);
    stop=omp_get_wtime();
    duration=stop-start;
    cout<<"\nTime taken for Sum of values calculation (parallel) : "<<duration*1e6<<" microseconds"<<endl<<endl;

    //avg par
    cout<<"Average value (parallel) is : ";
    start=omp_get_wtime();
    cout<<avgParallel(arr,n);
    stop=omp_get_wtime();
    duration=stop-start;
    cout<<"\nTime taken for Average value calculation (prallel) : "<<duration*1e6<<" microseconds"<<endl<<endl;

    return 0;
}