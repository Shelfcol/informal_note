1. docker里面使用rviz，并挂载本地文件夹： https://blog.csdn.net/weixin_39837709/article/details/109099684  

   需要安装nvidia驱动

   在docker-ce的基础上安装nvidia-docker2 ：https://blog.csdn.net/quantum7/article/details/86416600

   在docker_rviz文件下面打开终端运行 docker build -t  kinetic-desktop-full .   (kinetic-desktop-full是镜像名)

   bash docker_run_image.sh构建一个容器，并挂载本地文件夹，同时会进入此容器。以后都不要再运行此sh，而是用docker start docker_rviz启动容器

   docker exec -it docker_rviz bash 进入容器（docker_rviz是docker_run_image.sh里面的容器名）

   运行rviz的界面需要在进入docker容器之前运行 xhost +local:docker

2. docker每次需要sudo情况解决

   1. 创建docker用户组

   ```
    sudo groupadd docker1
   ```

   2. 应用用户加入docker用户组

   ```
    sudo usermod -aG docker ${USER}1
   ```

   3. 重启docker服务

   ```
    sudo systemctl restart docker
   ```

   4.  切换或者退出当前账户再从新登入

   ```
   su root             切换到root用户
   su ${USER}          再切换到原来的应用用户以上配置才生效 或者su gxf
   ```

3. docker命令整理： https://blog.csdn.net/qq_21197507/article/details/115071715?utm_source=app&app_version=4.12.0&code=app_1562916241&uLinkId=usr1mkqgl919blen

   

      docker容器导出生成镜像： docker export -o 新镜像名.tar 容器名

   ​		docker导入镜像: docker import 新镜像名.tar 镜像名:tag

   ​		docker run 

4. docker消失

      sudo vim /etc/docker/daemon.json  （添加docker路径，已经添加过了）
      sudo systemctl daemon-reload && systemctl restart docker
