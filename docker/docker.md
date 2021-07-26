1.  在docker里面使用rviz

   教程：https://medium.com/intro-to-artificial-intelligence/rviz-on-docker-bdf4d0fca5b

   注意host的时候是 xhost +local:docker（注意加号前面的空格）

   运行的时候报：nvidia runtime错误

   解决方法：在docker-ce的基础上安装nvidia-docker2 ：https://blog.csdn.net/quantum7/article/details/86416600

   在教程的最后一行docker run测试的时候nvidia/cuda需要加tag：docker run --runtime=nvidia --rm nvidia/cuda:11.4.0-runtime-ubuntu20.04 nvidia-smi



​		只需要执行一次sudo bash run_ssh_rviz_c.bash ,后面执行rviz容器的时候只需要  xhost +local:docker ,  docker start rvizK, docker exec -it rviz bash

​		只需要执行一次bash run_ssh_roscore_c.bash创建新容器并进入，后面不能用docker run，而是docker start ros_melodic，docker exec -it ros_melodic

​		docker容器导出生成镜像： docker export -o 新镜像名.tar 容器名

​		docker导入镜像: docker import 新镜像名.tar 镜像名:tag

​		docker run 



​	**成功：**

​	https://github.com/Shelfcol/car_demo 这个教程可以直接调出rviz和gazebo，

​	安装xserver安装：sudo apt-get install xserver-xephyr

​	docker: Error response from daemon: could not select device driver "" with capabilities: [[gpu]]

​	解决方法：systemctl restart docker

2. 每次将docker_file下的文件放置到需要跟docker容器互通的文件夹，执行sh文件，即可构建镜像和容器，并能启动rviz。每次修改了代码之后就需要执行sh文件
