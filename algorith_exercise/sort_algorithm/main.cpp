#include <iostream>
#include <vector>
using namespace std;
//https://www.runoob.com/w3cnote/ten-sorting-algorithm.html
//https://zhuanlan.zhihu.com/p/41923298
void bubble_sort(vector<int>& num_vec)
{
    //比较相邻数据，如果前面大于后面，则交换两个数据
    for(unsigned int i=num_vec.size()-1;i>0;--i)
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
    for(unsigned int i=1;i<num_vec.size();++i)
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
        /*
        if(num_vec[left_i]<num_vec[right_i])
        {
            tmp_vec[tmp_i++]=num_vec[left_i++];
        }
        else
        {
            tmp_vec[tmp_i++]=num_vec[right_i++];
        }
        */
        tmp_vec[tmp_i++]=(num_vec[left_i]<num_vec[right_i])?num_vec[left_i++]:num_vec[right_i++];
    }
    /*
    for(int i=left_i;i<=mid;++i)
        tmp_vec[tmp_i++]=num_vec[i];
    for(int i=right_i;i<=right;++i)
        tmp_vec[tmp_i++]=num_vec[i];
    */
    while(left_i<=mid) tmp_vec[tmp_i++]=num_vec[left_i++];
    while(right_i<=right) tmp_vec[tmp_i++]=num_vec[right_i++];
    for(int i=left;i<=right;++i)
        num_vec[i]=tmp_vec[i];
}

//快排：基准选左边，则从右开始
//https://wiki.jikexueyuan.com/project/easy-learn-algorithm/fast-sort.html
//为何要从右开始：因为最终基准数会和ij相遇位置的数进行交换，所以ij相遇时的数必须比基准数小，如果先从左边开始的话，最后一次移动的时候一定会停在比基准数大
//的位置，如果这时ij已经相遇，则基准数与此数进行交换，那么大的数就被放到前面去了，则不符合条件
void quick_sort(vector<int>& num_vec,int left,int right)
{
	//partion的思路
    if(left>=right)  return;

    int i=left,j=right;
    int pivot=num_vec[left];
    while(i!=j)
    {
        while(i<j&&num_vec[j]>=pivot) --j;
        while(i<j&&num_vec[i]<=pivot) ++i;
        if(i<j) swap(num_vec[i],num_vec[j]);
    }

    num_vec[left]=num_vec[i];//此时ij相等
    num_vec[i]=pivot;
    quick_sort(num_vec,left,i-1);
    quick_sort(num_vec,i+1,right);
}

//计数排序：核心在于将输入的数据值转化为键存储在额外开辟的数组空间中。作为一种线性时间复杂度的排序，计数排序要求输入的数据必须是有确定范围的整数。
//时间复杂度O(n+k) 空间复杂度O(k)
void count_sort(vector<int>& num_vec)
{
    if(num_vec.size()==0)   return;
	int min=num_vec[0];
	int max=num_vec[0];
	for(auto n:num_vec)
    {
        if(n>max)   max=n;
        if(n<min)    min=n;
    }
	vector<int> bucket(max-min+1,0);//前面为元素个数，后面为初始值
	for(auto n:num_vec)
	{
		bucket[n-min]++;
	}
	int index=0;
	for(int i=0;i<bucket.size();++i)//将每个桶里的元素填充会原来的数据
	{
		while(bucket[i]!=0)
		{
			bucket[i]--;
			num_vec[index++]=i+min;
		}
	}

}

//桶排序：
//将数组划分到一定数量的有序的桶里，然后再对每个桶中的数据进行排序（可以使用上面的任意排序方法），最后再将各个桶里的数据有序的合并到一起。
//时间复杂度 O(n+k) 空间复杂度 O(n+k)
//最快：将数据均匀分配到每个桶中；最慢：输入数据被分配到一个桶中
//数据最好接近，在一个范围内，否则会有很多空余的空间
void bucket_sort(vector<int>& num_vec,int BUCKET_SIZE=3)//默认每个桶的容量为BUCKET_SIZE
{
    if(num_vec.size()==0)   return;
	int min=num_vec[0];
	int max=num_vec[0];
	for(auto n:num_vec)
    {
        if(n>max)   max=n;
        if(n<min)    min=n;
    }
    int bucketCount=(max-min)/BUCKET_SIZE+1;//桶的个数
    //将数据放在每个桶中
    vector<vector<int>> buckets(bucketCount,vector<int>());
    for(auto n:num_vec)
    {
        //求出当前值属于哪个桶，并放进相应的桶
        int pos=(n-min)/BUCKET_SIZE;
        buckets[pos].push_back(n);
    }
    //对每个桶排序
    for(int i=0;i<bucketCount;++i)
        count_sort(buckets[i]);

    //将所有桶中数据放入依次放入数组中
    int index=0;
    for(int i=0;i<bucketCount;++i)
    {
        for(int j=0;j<buckets[i].size();++j)
        {
            num_vec[index++]=buckets[i][j];
        }
    }

}

/*
基数排序
空间复杂度 O(n+k) 时间复杂度 O(n*k)
非比较型整数排序算法，其原理是将整数按位数切割成不同的数字，然后按每个位数分别比较。
由于整数也可以表达字符串（比如名字或日期）和特定格式的浮点数，所以基数排序也不是只能使用于整数。
首先得到最大数的位数n
对于数字来说，桶为10个，0~9
先利用每个数字的个位数，来将不同数字放到10个桶里面
然后再将桶里的数字依次放回原数组，这时候原数组的个位数就是有序的
再利用每个数字的十位数，将不同数字放到19个桶里面，因为个位数排好序的，所以此时如果几个数在同一个桶里，那么一定是个位数小的在桶底，大的在桶上
然后再讲桶里的数字依次放回元数组，这时十位数也是有序的，个位数的有序性也没有被破坏
最终需要进行d次排序

基数排序：根据键值的每位数字来分配桶；
计数排序：每个桶只存储单一键值；
桶排序：每个桶存储一定范围的数值；
*/
void radix_sort(vector<int>& num_vec)
{
    if(num_vec.size()==0)   return ;
    int max=num_vec[0];
    for(auto n:num_vec)
    {
        if(n>max) max=n;
    }
    //求出最大数的位数
    int maxDigit=0;
    while(max>0)
    {
        ++maxDigit;
        max/=10;
    }


    int mod=10;
    int div=1;//(num%mod)/div可以得到相应位数的数字
    while(maxDigit--)//进行maxDigit次排序
    {
        vector<vector<int>> buckets(10,vector<int>());//定义10个桶
        //将数字放进桶里
        for(auto n:num_vec)
        {
            int num=(n%mod)/div;
            buckets[num].push_back(n);
        }
        //将数字放回数组
        int index=0;
        for(int i=0;i<buckets.size();++i)
        {
            for(int j=0;j<buckets[i].size();++j)
                num_vec[index++]=buckets[i][j];
        }
        mod*=10;
        div*=10;
    }

}
/*
堆排序
若构造递增数列：
1.构造大顶堆（从下至上扫描，交换 不满足大顶堆性质的数据,因为是完全二叉树，所以最后一个节点序列len的父节点序列号一定为(len-1)/2），取堆顶数字（最大值），
将第一个数字与最后一个数字交换
2.再将剩下数字构建小顶堆，取堆顶数字（剩余中的最小值）
3.重复以上操作，直至取完堆中数字，得到排好的序列

*/
//对子堆建立大顶堆，这一步操作必须保证原来的子堆已经是大顶堆，这样每次只是被交换的子节点的子堆不满足大顶堆的性质，而另一个子堆仍是大顶堆
void max_heapify(vector<int>& num_vec,int start,int end)//对[start,end]中数据建立大顶堆
{
    int dad=start;
    int son=2*dad+1;//左儿子
    while(son<=end)
    {
        //找出左右儿子中大的一个
        if(son+1<=end&&num_vec[son+1]>num_vec[son]) ++son;
        //交换父子节点
        if(num_vec[son]>num_vec[dad])
            swap(num_vec[son],num_vec[dad]);

        dad=son;
        son=dad*2+1;

    }
}
void heap_sort(vector<int>& num_vec)
{
    //初始化,从最后一个父节点开始建堆
    for(int i=num_vec.size()/2-1;i>=0;--i)
        max_heapify(num_vec,i,num_vec.size()-1);
    for(int i=num_vec.size()-1;i>0;--i)
    {
        swap(num_vec[i],num_vec[0]);//堆顶元素已经是最大值，则将其放在后面，这一部份就是排好序的
        max_heapify(num_vec,0,i-1);
    }
}

int main()
{
    vector<int> num_vec{6, 1, 2, 7, 9,5, 3, 4 ,5 ,10 ,100};
    //shell_sort(num_vec);

    //vector<int> tmp_vec=num_vec;
    //merge_sort(num_vec,tmp_vec,0,num_vec.size()-1);

    //quick_sort(num_vec,0,num_vec.size()-1);
	heap_sort(num_vec);
    for(auto n:num_vec)
        cout<<n<<" ";

    return 0;
}
