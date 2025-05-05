#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

void bubbleSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int> &arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k++] = L[i++];
    }
    while (j < n2)
    {
        arr[k++] = R[j++];
    }
}

void mergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void parallelBubbleSort(vector<int> &arr)
{
    int n = arr.size();

    for (int i = 0; i < n; i++)
    {
        int start = i % 2;
        #pragma omp parallel for default(none) shared(arr, n, start)
        for (int j = start; j < n - 1; j += 2)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void parallelMergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        #pragma omp parallel sections
        {
                #pragma omp section
                parallelMergeSort(arr, l, m);
                #pragma omp section
                parallelMergeSort(arr, m + 1, r);
        }
        merge(arr,l,m,r);
    }
}
int main()
{
    omp_set_num_threads(4);
    const int size=1000;
    vector<int> arr(size),arr_cpy(size);

    for(int i=0; i<size; i++){
        arr[i]=rand() %1000;
        arr_cpy[i]=arr[i];
    }

    // sequential bubble sort
    double start=omp_get_wtime();
    bubbleSort(arr);
    double stop=omp_get_wtime();
    double seq_bblsrt_tm=stop-start;

    arr=arr_cpy;

    // parallel bubble sort
    start=omp_get_wtime();
    parallelBubbleSort(arr);
    stop=omp_get_wtime();
    double par_bblsrt_tm=stop-start;

    arr=arr_cpy;

    // sequential merge sort
    start=omp_get_wtime();
    mergeSort(arr,0,size-1);
    stop=omp_get_wtime();
    double seq_mrgsrt_tm=stop-start;

    arr=arr_cpy;

    // parallel merge sort
    double par_mrgsrt_tm;
    #pragma omp parallel
    {
        #pragma omp single
        {
            start=omp_get_wtime();
            parallelMergeSort(arr,0,size-1);
            stop=omp_get_wtime();
            par_mrgsrt_tm=stop-start;
        }
    }

    arr=arr_cpy;

    cout<<"Total number of threads : "<<omp_get_num_threads()<<endl<<endl;

    cout<<"Sequential bubble sort time : "<<seq_bblsrt_tm*1e6<<" microseconds"<<endl<<endl;
    cout<<"Parallel bubble sort time : "<<par_bblsrt_tm*1e6<<" microseconds"<<endl<<endl;
    cout<<"Sequential merge sort time : "<<seq_mrgsrt_tm*1e6<<" microseconds"<<endl<<endl;
    cout<<"Parallel merge sort time : "<<par_mrgsrt_tm*1e6<<" microseconds"<<endl<<endl;

    cout<<"Speed up for bubble sort : "<<seq_bblsrt_tm/par_bblsrt_tm<<endl<<endl;
    cout<<"Speed up for merge sort : "<<seq_mrgsrt_tm/par_mrgsrt_tm<<endl<<endl;

    return 0;
}