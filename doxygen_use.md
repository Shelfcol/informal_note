https://blog.csdn.net/u010901792/article/details/52804232

总结：

namespace ，类，枚举的说明：
	/// @brief 整个建图过程中需要用到的变量和函数
	/// @author 古雪峰
	/// @date 2020/7/25
	
	

变量定义：
	///<表示经纬度


函数声明：
	不带详细说明的
		/// @brief 这是用来测试的函数
		/// @param a 被测试的变量
		/// @return 返回的是测试的结果（描述return的返回值）
		/// @see   Test()    （本函数参考其它的相关的函数，这里作一个链接）
		int testMe(int a);

	带详细说明的：
		/// @brief 这是用来测试的函数
		///
		/// 详细说明函数的用途1（前后空两行）
		/// 详细说明函数的用途2
		///
		/// @param a 被测试的变量
		/// @return 返回的是测试的结果（描述return的返回值）
		int testMe(int a);

