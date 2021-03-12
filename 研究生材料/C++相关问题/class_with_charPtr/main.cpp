#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

//���д���ָ�룬����Ҫд��������Ϳ�����ֵ����
class String
{
public:
    String(){num=0;};//����Ҫ�Զ���
    String(const char* cstr,int n=0);//���캯��???������������Ĭ�Ϲ��캯��������
    String(const String& str);//��������
    String& operator=(const String& str);//������ֵ
    String operator+(const String& str) const;//�ӷ������
    String operator-() const;//ȡ����һԪ�����
    String& operator+=(const String& str);
    String operator++();//ǰ׺�����
    String operator++(int);//��׺�����
    friend std::ostream& operator<<(std::ostream& os,const String& str);//<<����Ϊ��Ԫ�������cout<<str,������Ҫstr<<cout;�����
    ~String(){delete [] m_data;}
    char* get_ctsr()const {return m_data;}
private:
    char* m_data;
    int num;//Ϊ����ʾ����+,+=
};

String::String(const char* cstr,int n)//���캯��
{
    m_data=new char[strlen(cstr)+1];//size_t strlen ( const char * str ); �����ַ�������
    strcpy(m_data,cstr);
    num=n;
}
String::String(const String& str)//��������
{
    m_data=new char[strlen(str.m_data)+1];
    strcpy(m_data,str.m_data);
    num=str.num;
}
String& String::operator=(const String& str)//������ֵ,��Ҫ��delete�Լ�
{
    //��һ

    if(this!=&str)
    {
        delete [] m_data;
        m_data=NULL;
        m_data=new char [strlen(str.m_data)+1];//�������쳣��ȫ�ԡ����new���ڴ治��Ĵ�����m_data�����һ����ָ�룬�����׵��±�����
                                            //��ʱԭ����Ҳ���ٰ�ȫ��Υ�����쳣��ȫ�ԡ�
        strcpy(m_data,str.m_data);
        num=str.num;
    }
    /*
    //����
    //�����쳣��ȫ�ԣ���Ϊ������ʱString����������new�����ڴ棬�׳������ڴ治������⣬�������ǻ�û���޸�ԭ��ʵ����״̬������ʵ��״̬��Ч
    if(this!=&str)
    {
        String strTmp(str);//���ÿ������캯������һ����ʱ�������˳�{}��ͻ��Զ��ͷ�
        char* pTmp=strTmp.m_data;//����һ��ָ��ָ����ʱ��������
        strTmp.m_data=m_data;//��ʱ��������������õ�ǰ���������
        m_data=pTmp;//��ǰ����ָ����ʱָ�롣
        num=str.num;
    }*/
    return *this;
}

String  String::operator+(const String& str) const
{
    String sum;
    sum.num=num+str.num;
    /*
    String sum=str;////�ÿ������캯��������String sum���У���������������������
    sum.num+=num;*/
    return sum;
}
//һԪ����������ԺͶ�Ԫ��������֣���Ϊ�����겻ͬ
String String::operator-() const
{
    String tmp=*this;
    tmp.num=-1*tmp.num;
    return tmp;
}

String& String::operator+=(const String& str)
{
    num+=str.num;
    return *this;
}

String String::operator++()//ǰ׺�����
{
    ++this->num;
    String tmp=*this;
    return tmp;
}

String String::operator++(int)//��׺�����
{
    String tmp=*this;
    ++*this;/////////
    return tmp;
}

//����һ��������Ϊ��ʵ��������cout<<x<<y��
//cout<<Str����ת��Ϊoperator<<(cout,Str)
std::ostream& operator<<(std::ostream& os,const String& str)
{
    os<<"charptr= "<<str.m_data<<" ";
    os<<" num= "<<str.num<<endl;
    return os;
}

int main()
{
    char* cstr1="hello_world";
    char* cstr2="IamFine";
    //cstr='T';//�Ƿ���������
    //char cstr[10]="hello_world"����
    String s1(cstr1,10);//����
    cout<<"s1= "<<s1<<endl;
    String s2(cstr2,25);//����
    cout<<"s2= "<<s2<<endl;
    String s3(s1);//��������
    cout<<"s3= "<<s3<<endl;
    s3=s2;//������ֵ
    cout<<"s3= "<<s3<<endl;
    String s4=s1+s2;//�ӷ�
    //String s4=s2;//�ӷ�
    cout<<"s4= "<<s4<<endl;
    s4=s1++;
    cout<<"s4= "<<s4<<endl;
    s4=++s1;
    cout<<"s4= "<<s4<<endl;
    s4+=s1;
    cout<<"s4= "<<s4<<endl;
    s4=-s1;
    cout<<"s4= "<<s4<<endl;
    return 0;
}
