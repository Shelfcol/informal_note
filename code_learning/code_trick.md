1.Eigen用<<初始化

```cpp
expected initializer before ‘<<’ token
Eigen::Matrix4f transform_front_left = (Eigen::Matrix4f() << 
											-0.681064, -0.730351, 0.052336, -2.32,
											0.731354, -0.681998, 1.86265e-09, 7.65,
											0.035693, 0.0382761, 0.99863, 0.165,
											0, 0, 0, 1)
											   .finished();
```

2.c++ ubuntu cout改变颜色

```cpp
//https://blog.csdn.net/lixujie666/article/details/83544869
//http://blog.sina.com.cn/s/blog_628ba3e00101jll1.html
cerr << "\e[31m"<<"latest_point_heading ="<<"  hello  "<<"\e[0m"<< endl; //打印出红色
```



3.核心转储的调试

1.普通简单可执行文件的调试

```cpp
https://blog.csdn.net/youngys123/article/details/79804840
https://blog.csdn.net/byxdaz/article/details/90682397
1.在~/.bashrc　的最后加入： ulimit -c unlimited （一劳永逸），则可以生成core文件

2.测试
简单文件test.c
#include <stdio.h>
int main(void)

{
    printf("hello world! dump core for set value to NULL pointer/n");

    *(char *)0 = 0;

    return 0;

}

gcc -g test.c -o test (-g 表示打开调试)
./test
就会生成一个core文件
gdb test core就会将具体哪一行产生的核心转储位置打印出来。如果是其他原因导致的核心转储，可能不会显示代码的行数
```

2.ros launch文件的调试

```cpp
https://blog.csdn.net/sunzhao1000/article/details/103353631
https://blog.csdn.net/weixin_35695879/article/details/85254399
1.在CMakeLsits.txt里面加入调试指令
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
2.在launch文件里面的node加入
    launch-prefix="xterm -e gdb -ex run --args "
    或者 <node pkg="lidar_localization"  type="front_end_node"  name="front_end_node"  output="screen" launch-prefix="gnome-terminal -x gdb -ex run --args "></node>

执行roslaunch的时候就会直接弹出gdb的窗口
```

