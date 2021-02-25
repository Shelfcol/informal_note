#include <iostream>
#include <vector>
using namespace std;
//https://www.runoob.com/w3cnote/ten-sorting-algorithm.html
//https://zhuanlan.zhihu.com/p/41923298
void bubble_sort(vector<int>& num_vec)
{
    //�Ƚ��������ݣ����ǰ����ں��棬�򽻻���������
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
//�ҳ����е���Сֵ������ʼ����
void selection_sort(vector<int>& num_vec)
{
    for(unsigned int i=0;i<num_vec.size();++i)
    {
        int min_num=num_vec[i];
        int min_j=i;
        for(unsigned int j=i+1;j<num_vec.size();++j)//�ҳ�i�������Сֵ��Ӧ���±꣬Ȼ����i�����ݽ���
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

//ϣ�������ڲ�������Ļ����Ͻ����˸Ľ������Ļ���˼·���Ƚ������������зָ�����������зֱ����ֱ�Ӳ�������
//�����������еļ�¼��������ʱ���ٶ�ȫ�����ݽ�������ֱ�Ӳ�������
void shell_sort(vector<int>& num_vec)
{
    int h=1;
    int len=num_vec.size();
    while(h<len/3)  h=3*h+1;
    while(h>=1)
    {
        //��������
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

//�鲢����
void merge_sort(vector<int>& num_vec,vector<int>& tmp_vec,int left,int right)
{
    if(left>=right) return;//һ�����ݣ���������
    int mid=left+(right-left)/2;
    merge_sort(num_vec,tmp_vec,left,mid);
    merge_sort(num_vec,tmp_vec,mid+1,right);
    //��left��right֮���������
    int tmp_i=left;
    int left_i=left;//��������
    int right_i=mid+1;//�Ұ������
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

//���ţ���׼ѡ��ߣ�����ҿ�ʼ
//https://wiki.jikexueyuan.com/project/easy-learn-algorithm/fast-sort.html
//Ϊ��Ҫ���ҿ�ʼ����Ϊ���ջ�׼�����ij����λ�õ������н���������ij����ʱ��������Ȼ�׼��С������ȴ���߿�ʼ�Ļ������һ���ƶ���ʱ��һ����ͣ�ڱȻ�׼����
//��λ�ã������ʱij�Ѿ����������׼����������н�������ô������ͱ��ŵ�ǰ��ȥ�ˣ��򲻷�������
void quick_sort(vector<int>& num_vec,int left,int right)
{
	//partion��˼·
    if(left>=right)  return;

    int i=left,j=right;
    int pivot=num_vec[left];
    while(i!=j)
    {
        while(i<j&&num_vec[j]>=pivot) --j;
        while(i<j&&num_vec[i]<=pivot) ++i;
        if(i<j) swap(num_vec[i],num_vec[j]);
    }

    num_vec[left]=num_vec[i];//��ʱij���
    num_vec[i]=pivot;
    quick_sort(num_vec,left,i-1);
    quick_sort(num_vec,i+1,right);
}

//�������򣺺������ڽ����������ֵת��Ϊ���洢�ڶ��⿪�ٵ�����ռ��С���Ϊһ������ʱ�临�Ӷȵ����򣬼�������Ҫ����������ݱ�������ȷ����Χ��������
//ʱ�临�Ӷ�O(n+k) �ռ临�Ӷ�O(k)
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
	vector<int> bucket(max-min+1,0);//ǰ��ΪԪ�ظ���������Ϊ��ʼֵ
	for(auto n:num_vec)
	{
		bucket[n-min]++;
	}
	int index=0;
	for(int i=0;i<bucket.size();++i)//��ÿ��Ͱ���Ԫ������ԭ��������
	{
		while(bucket[i]!=0)
		{
			bucket[i]--;
			num_vec[index++]=i+min;
		}
	}

}

//Ͱ����
//�����黮�ֵ�һ�������������Ͱ�Ȼ���ٶ�ÿ��Ͱ�е����ݽ������򣨿���ʹ��������������򷽷���������ٽ�����Ͱ�����������ĺϲ���һ��
//ʱ�临�Ӷ� O(n+k) �ռ临�Ӷ� O(n+k)
//��죺�����ݾ��ȷ��䵽ÿ��Ͱ�У��������������ݱ����䵽һ��Ͱ��
//������ýӽ�����һ����Χ�ڣ�������кܶ����Ŀռ�
void bucket_sort(vector<int>& num_vec,int BUCKET_SIZE=3)//Ĭ��ÿ��Ͱ������ΪBUCKET_SIZE
{
    if(num_vec.size()==0)   return;
	int min=num_vec[0];
	int max=num_vec[0];
	for(auto n:num_vec)
    {
        if(n>max)   max=n;
        if(n<min)    min=n;
    }
    int bucketCount=(max-min)/BUCKET_SIZE+1;//Ͱ�ĸ���
    //�����ݷ���ÿ��Ͱ��
    vector<vector<int>> buckets(bucketCount,vector<int>());
    for(auto n:num_vec)
    {
        //�����ǰֵ�����ĸ�Ͱ�����Ž���Ӧ��Ͱ
        int pos=(n-min)/BUCKET_SIZE;
        buckets[pos].push_back(n);
    }
    //��ÿ��Ͱ����
    for(int i=0;i<bucketCount;++i)
        count_sort(buckets[i]);

    //������Ͱ�����ݷ������η���������
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
��������
�ռ临�Ӷ� O(n+k) ʱ�临�Ӷ� O(n*k)
�ǱȽ������������㷨����ԭ���ǽ�������λ���и�ɲ�ͬ�����֣�Ȼ��ÿ��λ���ֱ�Ƚϡ�
��������Ҳ���Ա���ַ������������ֻ����ڣ����ض���ʽ�ĸ����������Ի�������Ҳ����ֻ��ʹ����������
���ȵõ��������λ��n
����������˵��ͰΪ10����0~9
������ÿ�����ֵĸ�λ����������ͬ���ַŵ�10��Ͱ����
Ȼ���ٽ�Ͱ����������ηŻ�ԭ���飬��ʱ��ԭ����ĸ�λ�����������
������ÿ�����ֵ�ʮλ��������ͬ���ַŵ�19��Ͱ���棬��Ϊ��λ���ź���ģ����Դ�ʱ�����������ͬһ��Ͱ���ôһ���Ǹ�λ��С����Ͱ�ף������Ͱ��
Ȼ���ٽ�Ͱ����������ηŻ�Ԫ���飬��ʱʮλ��Ҳ������ģ���λ����������Ҳû�б��ƻ�
������Ҫ����d������

�������򣺸��ݼ�ֵ��ÿλ����������Ͱ��
��������ÿ��Ͱֻ�洢��һ��ֵ��
Ͱ����ÿ��Ͱ�洢һ����Χ����ֵ��
*/
void radix_sort(vector<int>& num_vec)
{
    if(num_vec.size()==0)   return ;
    int max=num_vec[0];
    for(auto n:num_vec)
    {
        if(n>max) max=n;
    }
    //����������λ��
    int maxDigit=0;
    while(max>0)
    {
        ++maxDigit;
        max/=10;
    }


    int mod=10;
    int div=1;//(num%mod)/div���Եõ���Ӧλ��������
    while(maxDigit--)//����maxDigit������
    {
        vector<vector<int>> buckets(10,vector<int>());//����10��Ͱ
        //�����ַŽ�Ͱ��
        for(auto n:num_vec)
        {
            int num=(n%mod)/div;
            buckets[num].push_back(n);
        }
        //�����ַŻ�����
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
������
������������У�
1.����󶥶ѣ���������ɨ�裬���� ������󶥶����ʵ�����,��Ϊ����ȫ���������������һ���ڵ�����len�ĸ��ڵ����к�һ��Ϊ(len-1)/2����ȡ�Ѷ����֣����ֵ����
����һ�����������һ�����ֽ���
2.�ٽ�ʣ�����ֹ���С���ѣ�ȡ�Ѷ����֣�ʣ���е���Сֵ��
3.�ظ����ϲ�����ֱ��ȡ��������֣��õ��źõ�����

*/
//���Ӷѽ����󶥶ѣ���һ���������뱣֤ԭ�����Ӷ��Ѿ��Ǵ󶥶ѣ�����ÿ��ֻ�Ǳ��������ӽڵ���ӶѲ�����󶥶ѵ����ʣ�����һ���Ӷ����Ǵ󶥶�
void max_heapify(vector<int>& num_vec,int start,int end)//��[start,end]�����ݽ����󶥶�
{
    int dad=start;
    int son=2*dad+1;//�����
    while(son<=end)
    {
        //�ҳ����Ҷ����д��һ��
        if(son+1<=end&&num_vec[son+1]>num_vec[son]) ++son;
        //�������ӽڵ�
        if(num_vec[son]>num_vec[dad])
            swap(num_vec[son],num_vec[dad]);

        dad=son;
        son=dad*2+1;

    }
}
void heap_sort(vector<int>& num_vec)
{
    //��ʼ��,�����һ�����ڵ㿪ʼ����
    for(int i=num_vec.size()/2-1;i>=0;--i)
        max_heapify(num_vec,i,num_vec.size()-1);
    for(int i=num_vec.size()-1;i>0;--i)
    {
        swap(num_vec[i],num_vec[0]);//�Ѷ�Ԫ���Ѿ������ֵ��������ں��棬��һ���ݾ����ź����
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
