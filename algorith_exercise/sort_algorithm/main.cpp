#include <iostream>
#include <vector>
using namespace std;
//https://www.runoob.com/w3cnote/ten-sorting-algorithm.html
//https://zhuanlan.zhihu.com/p/41923298
void bubble_sort(vector<int>& num_vec)
{
    //比较相邻数据，如果前面大于后面，则交换两个数据
    for(int i=num_vec.size()-1;i>0;--i)
    {
        bool swap_flag=false;
        for(unsigned int j=0;j<i;++j)
        {
            if(num_vec[j]>num_vec[j+1])
            {
                swap(num_vec[j],num_vec[j+1]);
                swap_flag=true;
            }
        }
        if(!swap_flag)    break;
    }
}
//找出序列的最小值，与起始交换
void selection_sort(vector<int>& num_vec)
{
    for(unsigned int i=0;i<num_vec.size();++i)
    {
        int min_num=num_vec[i];
        int min_j=i;
        for(unsigned int j=i+1;j<num_vec.size();++j)//找出i后面的最小值对应的下标，然后与i的数据交换
        {
            if(num_vec[j]<min_num)
            {
                min_num=num_vec[j];
                min_j=j;
            }
        }
        swap(num_vec[i],num_vec[min_j]);
    }
}

void insertion_sort(vector<int>& num_vec)
{
    for(int i=1;i<num_vec.size();++i)
    {
        int key=num_vec[i];
        int j=i-1;
        for(;j>=0&&num_vec[j]>key;--j)
            num_vec[j+1]=num_vec[j];
        num_vec[j+1]=key;
    }
}

//希尔排序：在插入排序的基础上进行了改进，它的基本思路是先将整个数据序列分割成若干子序列分别进行直接插入排序，
//待整个序列中的记录基本有序时，再对全部数据进行依次直接插入排序。
void shell_sort(vector<int>& num_vec)
{
    int h=1;
    int len=num_vec.size();
    while(h<len/3)  h=3*h+1;
    while(h>=1)
    {
        //插入排序
        for(int i=h;i<len;i+=h)
        {
            int key=num_vec[i];
            int j=i-h;
            for(;j>=h&&num_vec[j]>key;j-=h)
                num_vec[j+h]=num_vec[j];
            num_vec[j+h]=key;
        }
        h/=3;
    }

}

//归并排序
void merge_sort(vector<int>& num_vec,vector<int>& tmp_vec,int left,int right)
{
    if(left>=right) return;//一个数据，不用排序
    int mid=left+(right-left)/2;
    merge_sort(num_vec,tmp_vec,left,mid);
    merge_sort(num_vec,tmp_vec,mid+1,right);
    //将left和right之间的数排序
    int tmp_i=left;
    int left_i=left;//左半边数据
    int right_i=mid+1;//右半边数据
    while(left_i<=mid&&right_i<=right)
    {
        if(num_vec[left_i]<num_vec[right_i])
        {
            tmp_vec[tmp_i++]=num_vec[left_i++];
        }
        else
        {
            tmp_vec[tmp_i++]=num_vec[right_i++];
        }
    }
    for(int i=left_i;i<=mid;++i)
        tmp_vec[tmp_i++]=num_vec[i];
    for(int i=right_i;i<=right;++i)
        tmp_vec[tmp_i++]=num_vec[i];
    for(int i=left;i<=right;++i)
        num_vec[i]=tmp_vec[i];
}
int main()
{
    vector<int> num_vec{6,9,50,10,30,10,20,25};
    //shell_sort(num_vec);
    vector<int> tmp_vec=num_vec;
    merge_sort(num_vec,tmp_vec,0,num_vec.size()-1);
    for(auto n:num_vec)
        cout<<n<<" ";

    return 0;
}
