1. ubuntu 自带中文输入法，不要再安装搜狗或谷歌拼音，否则容易开机黑屏

2. 安装cuda驱动的时候，报WARN：installation is not complet。这是正常的，不用管。

3. 多分屏终端：sudo apt-get install terminator

4. ros install :https://blog.csdn.net/qq_44339029/article/details/108919545

5. ros安装rosdep update超时：

   https://blog.csdn.net/Iamsonice/article/details/116018915?utm_source=app&app_version=4.12.0&code=app_1562916241&uLinkId=usr1mkqgl919blen

   

   1. git clone https://github.com/ros/rosdistro.git

   2. 修改文件：/usr/lib/python3/dist-packages/rosdep2/rep3.py 

      ​					/etc/ros/rosdep/sources.list.d/20-default.list

      ​					/usr/lib/python3/dist-packages/rosdistro/\__init__.py

      ​				将里面的https://raw.github,com ...改为本地地址 file:///home/gxf/...

6. ubuntu配置外网：https://rumosky.com/archives/217

   