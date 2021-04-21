#include <iostream>
using namespace std;
void init_array1(int** arr,int size){
    *arr = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        //注意此处*arr只是数字的首地址
        *(*arr + i) = 0;
    }
}

void init_array2(int* arr,int size){
    arr = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        //注意此处*arr只是数字的首地址
        *(arr + i) = 0;
    }
}

int main() {
    int* arr;
    int size = 10;
    //init_array1(&arr,size);
    init_array2(arr,size);
    for (int i = 0; i < size; ++i) {
        printf("%d,",*(arr + i));
    }
    printf("\n");
    return 0;
}
