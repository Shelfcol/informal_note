#include <iostream>
#include <vector>
using namespace std;
//https://www.runoob.com/w3cnote/ten-sorting-algorithm.html
//https://zhuanlan.zhihu.com/p/41923298
void bubble_sort(vector<int>& num_vec)
{
    //�Ƚ��������ݣ����ǰ����ں��棬�򽻻���������
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
    for(int i=1;i<num_vec.size();++i)
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
