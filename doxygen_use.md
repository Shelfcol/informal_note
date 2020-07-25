https://blog.csdn.net/u010901792/article/details/52804232
举例来说，若我们有下面这样的class。


    class MyClass {
        public:
            int member1 ;
            int member2:
            void member_function();
    };
    
加上批注后，就变成这样：

    /**
     * 我的自订类别说明 ...
     */
    class MyClass {
        public:
            int member1 ; ///< 第一个member说明 ...
            int member2:  ///< 第二个member说明 ...
            int member_function(int a, int b);
    };
    
    /**
     * @brief class或function的简易说明...
     *(空一行)
     * @param a 参数a的说明
     * @param b 参数b的说明
     *
     * @return 传回a+b。
     */ 
    int MyClass::member_function( int a, int b ) 
    {
        return a+b ;
    }
    
    
    
    
除了这个class 及function外，Doxygen 也可针对档案做说明，条件
是该批注需置于档案的前面。主要也是利用一些指令，通常这部分注
解都会放在档案的开始地方。如：

    /*! \file myfile.h
        \brief 档案简易说明
    
        详细说明.
        
        \author 作者信息
    */
    
 如您所见，档案批注约略格式如上，请别被"\" 所搞混。其实，"\" 
与"@" 都是一样的，都是告诉Doxygen 后面是一个指令。两种在
Doxygen 都可使用。笔者自己比较偏好使用"@"。


///////////////////////////完整示例：文档说明，类说明，函数说明，枚举说明////////////////////////
/**
 * Created by Qxt on 2016/10/13
 * @brief 测试Doxygen的使用
 */
public class TestClass {


  /// 枚举
  enum TYPE
    {
        TYPE_01,/*!< 枚举项01 */
        TYPE_02,///< 枚举项02
    };


  int member1;///<第一个member说明


    /**
     *
     * @see 参考项 http://www.cnblogs.com/tianzhijiexian/
     * @brief 求和
     * @author QXT
     * @date 2016/10/13 13:37:56
     * @version 0.1
     * @retval c 描述返回值的类型
     * @note 注解，可以是详细的注解
     * @remarks   备注事项（remaks）
     * @attention 注意事项(attention)
     * @warning 警告信息
     * @param a 用来相加的参数
     * @param b 用来相加的参数
     * @return 传回两个参数相加的结果
     *
     */
    int AddFun（int a,int b）{
        return (a+b);
    }
}



