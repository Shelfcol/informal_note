#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

//类中带有指针，则需要写拷贝构造和拷贝赋值操作
class String
{
public:
    String(){num=0;};//必须要自定义
    String(const char* cstr,int n=0);//构造函数
    String(const String& str);//拷贝构造
    String& operator=(const String& str);//拷贝赋值
    String operator+(const String& str) const;//加法运算符
    friend String operator+(int n,const String& str);//1+str，因为+的第一个参数不是String，所以只能定义为友元函数。
                                                     //后面不能加const，因为const是修饰*this指针的，友元函数没有*this指针
    String operator-() const;//取反，一元运算符
    String& operator+=(const String& str);
    String operator++();//前缀运算符
    String operator++(int);//后缀运算符
    explicit operator int() const;//将String对象强制转换成int，不能改变原来的对象值，也不能有返回类型，因为int已经指定了返回类型.explicit表示只能显示转换
    friend ostream& operator<<(ostream& os,const String& str);//<<声明为友元，则可以cout<<str,否则需要str<<cout;很奇怪
    ~String(){delete [] m_data;}
    char* get_ctsr()const {return m_data;}
private:
    char* m_data;
    int num;//为了演示重载+,+=
};

String::String(const char* cstr,int n)//构造函数
{
    if(cstr==0)
    {
        m_data=new char[1];
        m_data='\0';
        num=n;
    }
    else
    {
        m_data=new char[strlen(cstr)+1];//size_t strlen ( const char * str ); 返回字符串长度
        strcpy(m_data,cstr);
        num=n;
    }

}
String::String(const String& str)//拷贝构造
{
    m_data=new char[strlen(str.m_data)+1];
    strcpy(m_data,str.m_data);
    num=str.num;
}
String& String::operator=(const String& str)//拷贝赋值,需要先delete自己
{
    //法一

    if(this!=&str)
    {
        delete [] m_data;
        m_data=NULL;
        m_data=new char [strlen(str.m_data)+1];//不满足异常安全性。如果new报内存不足的错误，则m_data变成了一个空指针，很容易导致崩溃，
                                            //这时原对象也不再安全，违背了异常安全性。
        strcpy(m_data,str.m_data);
        num=str.num;
    }
    /*
    //法二
    //满足异常安全性，因为就算临时String变量里面用new分配内存，抛出诸如内存不足的问题，但是我们还没有修改原来实例的状态，所以实例状态有效
    if(this!=&str)
    {
        String strTmp(str);//利用拷贝构造函数构建一个临时变量。退出{}后就会自动释放
        char* pTmp=strTmp.m_data;//构造一个指针指向临时变量数据
        strTmp.m_data=m_data;//临时变量里面的数据用当前对象的数据
        m_data=pTmp;//当前数据指向临时指针。
        num=str.num;
    }*/
    return *this;
}

String  String::operator+(const String& str) const
{
    String sum;
    sum.num=num+str.num;
    delete [] sum.m_data;
    sum.m_data=new char [strlen(str.m_data)+strlen(m_data)+1];
    strcpy(sum.m_data,m_data);
    strcat(sum.m_data,str.m_data);//将后一个字符串吊到前一个后面
    /*
    String sum=str;////用拷贝构造函数？？？String sum不行？？？？？？？？？？？
    sum.num+=num;*/
    return sum;
}

String operator+(int n,const String& str) //友元函数，实现的时候不需要写friend，也不能加String::
{
    String tmp=str;
    tmp.num=tmp.num+n;
    return tmp;
}

//一元运算符，可以和二元运算符区分，因为特征标不同
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

String String::operator++()//前缀运算符
{
    ++this->num;
    String tmp=*this;
    return tmp;
}

String String::operator++(int)//后缀运算符
{
    String tmp=*this;
    ++*this;
    return tmp;
}

String::operator int() const//不能指定返回类型
{
    return num;
}

//返回一个引用是为了实现连续的cout<<x<<y。
//cout<<Str将被转换为operator<<(cout,Str)
ostream& operator<<(ostream& os,const String& str)
{
    os<<"charptr= "<<str.m_data<<" ";
    os<<" num= "<<str.num<<endl;
    return os;
}

int main()
{
    char* cstr1="hello_world";
    char* cstr2="IamFine";
    //cstr='T';//是否允许？？？
    //char cstr[10]="hello_world"区别
    String s1(cstr1,10);//构造
    cout<<"s1= "<<s1<<endl;
    String s2(cstr2,25);//构造
    cout<<"s2= "<<s2<<endl;
    String s3=s1;//拷贝构造。s3变量不存在，需要现在构造
    //String s3(s1);//也可以
    cout<<"s3= "<<s3<<endl;
    s3=s2;//拷贝赋值，s3变量已经存在
    cout<<"s3= "<<s3<<endl;
    String s4=s1+s2;//加法
    //String s4=s2;//加法
    cout<<"s4= "<<s4<<endl;
    s4=s1++;
    cout<<"s4= "<<s4<<endl;
    s4=++s1;
    cout<<"s4= "<<s4<<endl;
    s4=9+s1;
    cout<<"s4= "<<s4<<endl;
    s4+=s1;
    cout<<"s4= "<<s4<<endl;
    s4=-s1;
    cout<<"s4= "<<s4<<endl;
    int s=int(s4);//s=s4是错误的，因为explicit只能显式转换
    cout<<s;
    return 0;
}
