## 软件安装笔记
1. 印象笔记
   
    建议从Nixnote官网下最新的：https://sourceforge.net/projects/nevernote/，建议别用ppa的，这个好像会少拉一些包，而且没有图标。 安装中途会提示libtbb2和libcurl3，apt-get安装就行了 然后在'file文件'中建立账户,然后使用建立的账户，选择“印象笔记”（这个是国服），使用'工具'中的同步，这时会进入印象笔记大陆的服务器，如果选择的是印象笔记国际版，进入的就是国际版。两个域名是不一样的，在没登录前可以在~/.nixnote/accounts.conf中看到。登录页面左上角也不一样，国际版是Eventnote，国服是印象笔记。  在登录国服时候，输入完邮箱地址一直不会显示密码框，这个时候点击左上角的“印象笔记”链接，然后会打开网页版的印象笔记页面，在里面找到登录页面，正常登录。然后关闭登录框，再从'工具'中的同步进入，这个时候就会看到授权提示了。

   

2. slam的评估轨迹的包

    下载地址：https://github.com/uzh-rpg/rpg_trajectory_evaluation          				  

    下载地址：https://github.com/catkin/catkin_simple

    pg_trajectory_evaluation-master，catkin_simple将这两个包放在catkin_ws下面，然后catkin_make

    

3. VTK+PCL 
   
    教程地址：https://blog.csdn.net/sinat_28752257/article/details/79169647
    教程地址：https://blog.csdn.net/sinat_28752257/article/details/79169647
    
    (但是我的新电脑直接可以用我的sh文件安装)
    
    


4. ros kinect
   
    教程地址：https://blog.csdn.net/softimite_zifeng/article/details/78632211
    
    ```shell
    报错： sudo rosdep init: ERROR:cannot download default sources list from
    解决：需要翻墙，利用lantern翻墙，每个月可以有500M的翻墙额度
    ```
    
    
    
5. lantern

    下载地址：https://raw.githubusercontent.com/getlantern/lantern-binaries/master/lantern-installer-preview-64-bit.deb

    安装教程：

    ```shell
    sudo dpkg -i lantern-installer-preview-64-bit.deb
    apt-cache search libappindicator3-1
    sudo apt-get install libappindicator3-1
    # 在终端输入lantern即可启用，但要在右上角确认是否connected
    # 再sudo rosdep init，再继续后面的操作。最好不要在ros安装一开始就启用lantern，因为其额度有限，最后在sudo rosdep init出错之后再启用
    ```




6. U盘解锁方法

    ```shell
    # 查看信息，找到 /dev/sdb4       284G  217G   68G   77% /media/xxx/u盘名
    df -h 
    # 卸载u盘（不需要拔掉u盘）（此步若不成功，可以直接做下一步）
    sudo umount /media/xxx/u盘名
    # 文件修复
    sudo dosfsck -v -a /dev/sdb4 
    ```




7. tensorflow 安装并在jupyter里面运行

    - 安装anaconda3

    - 安装tensorflow
      教程：https://www.cnblogs.com/guohaoblog/p/9319791.html（(tensorflow)D:\>ipython kernelspec install-self --user很重要）
    ```shell
    # 要使用tensorflow，先进入tf虚拟环境：
    source activate tf
    # 在里面输入以下，进入jupyter。
    jupyter notebook
    # 要退出tf虚拟环境：
    source deactivate tf  
    ```

    

8. sophus安装：

    ```shell
    git clone http://github.com/strasdat/Sophus.git
    # (!!!!!!!!!!!!!!!!!!!very important),切换分支
    git checkout a621ff  
    mkdir build
    cd build
    cmake ..
    make
    ```

    

9. sophus使用问题

     ```
      CMakeFiles/run_vo.dir/run_vo.cpp.o：在函数‘main’中：
       run_vo.cpp:(.text.startup+0x1107)：对‘Sophus::SE3::inverse() const’未定义的引用
       run_vo.cpp:(.text.startup+0x114a)：对‘Sophus::SO3::matrix() const’未定义的引用
       run_vo.cpp:(.text.startup+0x116d)：对‘Sophus::SO3::matrix() const’未定义的引用
       run_vo.cpp:(.text.startup+0x1190)：对‘Sophus::SO3::matrix() const’未定义的引用
       run_vo.cpp:(.text.startup+0x11b3)：对‘Sophus::SO3::matrix() const’未定义的引用
       run_vo.cpp:(.text.startup+0x11d6)：对‘Sophus::SO3::matrix() const’未定义的引用
       CMakeFiles/run_vo.dir/run_vo.cpp.o:run_vo.cpp:(.text.startup+0x11f9): 跟着更多未定义的参考到 Sophus::SO3::matrix() const
       ../../lib/libmyslam.so：对‘Sophus::SE3::operator*(Eigen::Matrix<double, 3, 1, 0, 3, 1> const&) const’未定义的引用
       ../../lib/libmyslam.so：对‘Sophus::SE3::SE3(Sophus::SO3 const&, Eigen::Matrix<double, 3, 1, 0, 3, 1> const&)’未定义的引用
       ../../lib/libmyslam.so：对‘Sophus::SE3::operator=(Sophus::SE3 const&)’未定义的引用
       ../../lib/libmyslam.so：对‘Sophus::SE3::SE3()’未定义的引用
       ../../lib/libmyslam.so：对‘Sophus::SO3::SO3(double, double, double)’未定义的引用
       ../../lib/libmyslam.so：对‘Sophus::SE3::log() const’未定义的引用
       ../../lib/libmyslam.so：对‘Sophus::SE3::SE3(Sophus::SE3 const&)’未定义的引用
       ../../lib/libmyslam.so：对‘Sophus::SE3::operator*(Sophus::SE3 const&) const’未定义的引用
       collect2: error: ld returned 1 exit status
       test/CMakeFiles/run_vo.dir/build.make:140: recipe for target '../bin/run_vo' failed
       make[2]: *** [../bin/run_vo] Error 1
       CMakeFiles/Makefile2:140: recipe for target 'test/CMakeFiles/run_vo.dir/all' failed
       make[1]: *** [test/CMakeFiles/run_vo.dir/all] Error 2
       Makefile:83: recipe for target 'all' failed
       make: *** [all] Error 2
     ```
  
      解决办法：https://blog.csdn.net/u010003609/article/details/68961293
      set( THIRD_PARTY_LIBS   ${Sophus_LIBRARIES} libSophus.so)


10. 运行PUBG的错误

    ``` shell
    PUBG Shield
    X
    Error In Essential DlI Files!
    Please, reinstall software requirements: e.g., DirectX, MSVCRT
    Log File:
    C:\steam. _download\steamapps\common\PUBG\TslGame\Binaries\Win
    64\pubg_ fail.log
    Technical Information:
    code:00000A00/0B1A, info:0000000000135
    XINPUT1_ 3.dll
    
    
    解决：下载DirectX修复工具http://www.onlinedown.net/soft/120082.html
    ```

    

11. SolidWorks2018安装

      软件安装管家上下载，然后按照教程http://www.cadzxw.com/11054.html

      

12. 卸载pcl库

      在build文件夹里面sudo make uninstall,千万不要随便手动到usr文件家里面删任何东西

      

13. 安装google的cartographer

      教程：https://google-cartographer-ros.readthedocs.io/en/latest/demos.html
      教程：https://blog.csdn.net/qq_26482237/article/details/92676267  

      两者辅助
      巨坑：开始自己装了一个pcl-1.9,就报各自pcl的time_h找不到之类查不到的错误，还有boost，我怀疑是pcl太新，导致与boost等库不兼容，以及与cartographer不兼容，联想到ros里面预装了一个pcl-1.7,所以我就将pcl-1.9卸载，然后再重新按照网址安装cartographer（需要执行所有步骤），就成功，所以不要装其他的pcl，ros自带的就OK

      ERROR:

      ```change ceres-solver location
    Could NOT find Protobuf: Found unsuitable version "2.6.1", but required is
        at least "3.0.0" (found
        //usr/lib/x86_64-linux-gnu/libprotobuf.so;-lpthread)
      Call Stack (most recent call first):
        /home/gxf/.local/lib/python2.7/site-packages/cmake/data/share/cmake-3.18/Modules/FindPackageHandleStandardArgs.cmake:456 (_FPHSA_FAILURE_MESSAGE)
        /home/gxf/.local/lib/python2.7/site-packages/cmake/data/share/cmake-3.18/Modules/FindProtobuf.cmake:626 (FIND_PACKAGE_HANDLE_STANDARD_ARGS)
        CMakeLists.txt:35 (find_package)
    
      ```
    
    
    
14. vs2017安装完后再装2015

    只选择通用windows应用开发工具中的windows10 SDK（10.0.10586）
    	

15. keyshot8.2

    下载地址： http://www.gfxcamp.com/keyshot-pro-8280/
    	 

16. imu使用

    教程：https://blog.csdn.net/qq_31356389/article/details/79216765

    ```shell
    sudo apt-get install ros-kinetic-xsens-driver
    rosstack profile
    rospack profile
    # 赋予权限，使用前必备操作
    sudo chmod 777 /dev/ttyUSB*
    roslaunch xsens_driver xsens_driver.launch
    rostopic list
    rostopic echo /imu/data
    
    # 如果要修改imu的frame_id，则需要下载驱动包进行安装，再修改launch文件
    # 安装好后的使用：
    sudo chmod 777 /dev/ttyUSB*
    roslaunch xsens_driver xsens_driver.launch
    ```



17. Opencv安装

    教程：https://www.cnblogs.com/arkenstone/p/6490017.html

    ```shell
    WITH_QT=false
    cmake -D CMAKE_BUILD_TYPE=RELEASE -D INSTALL_PYTHON_EXAMPLES=ON -D INSTALL_C_EXAMPLES=OFF  -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.2.0/modules -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=OFF  -D WITH_GTK=ON -D WITH_OPENGL=ON  -D BUILD_EXAMPLES=ON ..
    # 如果cmake这一步出太多错，就直接
    sudo cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_QT=OFF -D OPENCV_EXTRA_MODULES_PATH= .. 
    sudo make -j4 sudo make install 
    sudo ldonfig
    # 解决办法： https://blog.csdn.net/weixin_41896508/article/details/80795239
    # 可能需要下载ippicv20151201,替换opencv-3.0.0/3rdparty/ippicv/downloads/linux-8b449a536a2157bcad08a2b9f266828b下面的压缩包即可
    
     make之后的配置https://www.jianshu.com/p/88ad5441c2ae



18. 翻译软件的安装（可以鼠标取词)

    教程：https://www.jianshu.com/p/92e96b2ad693
    朗道：http://download.huzheng.org/zh_CN/

    只需要安装朗道英译汉就可以了，如果误装了词典，删掉就可以sudo rm -rf filename

    

19. ubuntu16.04美化
    - unity
    		点击图标进出文件: https://jingyan.baidu.com/article/d45ad148bc51bb69552b80dc.html
    	
    - 图标在左边和下边切换:
    				gsettings set com.canonical.Unity.Launcher launcher-position Left
    	        gsettings set com.canonical.Unity.Launcher launcher-position Bottom
    
     - gnome tweaks        

         更换桌面背景（似乎不用安装，直接桌面右键即可更换背景）(软件中心安装，图标在usr/share/backgrounds)

         教程：https://blog.csdn.net/github_36498175/article/details/80161362
         
         
    
20. velodyne
    
    教程：https://www.jianshu.com/p/1274c002ef5f
    
    
    
21. ubuntu16.04 wifi 连接SJTU

     安全：WPA及WPA2企业
     认证：受保护的EAP（PEAP）
     不需要CA证书（R）

    

22. nvidia 驱动安装
    
    教程：https://blog.csdn.net/xunan003/article/details/81665835
    
    nvidia卸载后重装问题，ERROR: 
    ```shell
    An NVIDIA kernel module 'nvidia-drm' appears to already be loaded in your kernel. This may be because it is in use (for example, by an X server, a CUDA program, or the NVIDIA Persistence Daemon), but this may also happen if your kernel was configured without support for module unloading. Please be sure to exit any programs that may be using the GPU(s) before attempting to upgrade your driver. If no GPU-based programs are running, you know that your kernel supports module unloading, and you still receive this message, then an error may have occured that has corrupted an NVIDIA kernel module's usage count, for which the simplest remedy is to reboot your computer.
    
    解决方案: https://unix.stackexchange.com/questions/440840/how-to-unload-kernel-module-nvidia-drm/441811#441811
    
    # 在开机之后不要输入用户名和密码登陆
    ctrl+alt+F2
    # 进入管理员模式
    sudo su
    systemctl isolate multi-user.target,modprobe -r nvidia-drm
    systemctl start graphical.target
    ```
    
    
    
      cuda，cudnn安装教程：https://blog.csdn.net/lihe4151021/article/details/90237681，成功
      cudnn安装教程：https://blog.csdn.net/caicaiatnbu/article/details/87626491，成功
      cudnn安装后之后检验的时候，bug:error while loading shared libraries: libcudart.so.10.0
      教程：https://blog.csdn.net/wendygelin/article/details/88528055
    
    ```
    sudo cp /usr/local/cuda-10.0/lib64/libcudart.so.10.0 /usr/local/lib/libcudart.so.10.0 && sudo ldconfig
    sudo cp /usr/local/cuda-10.0/lib64/libcublas.so.10.0 /usr/local/lib/libcublas.so.10.0 && sudo ldconfig
    sudo cp /usr/local/cuda-10.0/lib64/libcurand.so.10.0 /usr/local/lib/libcurand.so.10.0 && sudo ldconfig
    ```
    
    ​	https://developer.nvidia.com/cuda-toolkit-archive，可以下载旧版本的CUDA
    
    
    
23. yolov3配置
    教程：https://blog.csdn.net/qq_36327203/article/details/84305303

    

24. tensorflow2.0	

    教程：https://blog.csdn.net/Taylent/article/details/99604468
    	

25. 可以下载免费视频的网站

    https://www.vjshi.com/watch/2867215.html

    

26. 清华大学的交通标志牌数据集

    https://cg.cs.tsinghua.edu.cn/dataset/form.html?dataset=tt100k

    

27. ImportError错误

    ImportError: /opt/ros/kinetic/lib/python2.7/dist-packages/cv2.so: undefined symbol: PyCObject_Type

    ​	import sys
    ​	sys.path.remove('/opt/ros/kinetic/lib/python2.7/dist-packages')
    ​	import cv2
    ​	

28. sublime 不能输入中文

    https://www.jianshu.com/p/bf05fb3a4709

    

29. tensorboard不能显示

    教程：https://blog.csdn.net/sinat_40515025/article/details/80793942
    tensorboard --logdir=/home/gxf/deeplearning/keras_learning/logs  #绝对路径

    

30. 阿里云服务器登陆
    ssh root@114.55.93.111
    gxf740297955.
    	

31. 妹子路由器密码

    路由器后面的AMIITID  2018AP8568
    管理者密码：gxf740297955.

    

32. qq for linux
    	https://blog.csdn.net/Beking17113/article/details/102870110?utm_source=distribute.pc_relevant.none-task
    	

33. python版本切换
    	https://blog.csdn.net/Romance5201314/article/details/81667778

    

34. jupyter notebook python版本切换：
    	https://blog.csdn.net/sinat_34328764/article/details/83214172

    
    
35. 配置mask-rcnn
    	教程：https://www.jianshu.com/p/f51339f886fc

       ```
       import sys
       sys.path.remove('/opt/ros/kinetic/lib/python2.7/dist-packages')
       ```

    
    
36. jupyter 转python

     jupyter nbconvert --to script xxx.ipynb

    

37. 错误Could not find a package configuration
    ```
    Could not find a package configuration file provided by "tf2_sensor_msgs" with any of the following names:
    tf2_sensor_msgsConfig.cmake
    tf2_sensor_msgs-config.cmake
    
    sudo apt-get install ros-kinetic-tf2-sensor-msgs
    ```

38. Warning: TF_OLD_DATA 
    ```shell
    Warning: TF_OLD_DATAignoring data from the past for frame sonar1_link at time 1.53089e+09 according to authority unknown_publisher
    Possible reasons are listed at http://wiki.ros.org/tf/Errors%20explained at line 277 in /tmp/binarydeb/ros-kinetic-tf2-0.5.20/src/buffer_core.cpp
    
    # bag包不能循环播放,只能播放一遍
    ```

39. 电脑打开只有背景,可以右键

    ```shell
    # 重装了gdm,然后将桌面换成了gdm
    mv /.cache /.cache-NOGOOD
    sudo reboot
    # 然后开机之后unlock那里有个齿轮,选择gdm
    ```

40. 编译ros问题 

    https://www.pianshen.com/article/7654723641/

41. pytorch c++使用 
    地址：https://pytorch.org/cppdocs/installing.html
    
    pytorch install：https://pytorch.org/get-started/previous-versions/
    
    ```shell
    ImportError: No module named machinery：换python3
    
    当我安装了python3.7后，也将软链接python3生成到/usr/bin/python37,然后我执行python37 ./tools/build_libtorch.py，然后说我没有yaml文件，然后pip37 install PyYAML，说permission denied，因为我的python3.7是在usr里面的，所以sudo pip37 install PyYAML
    
    pip install torch==1.4.0+cpu torchvision==0.5.0+cpu -f https://download.pytorch.org/whl/torch_stable.html 
    # 直接安装成功，而且python2.7都能直接诶启动。
    ```


42. 安装百度云

    下载地址：https://github.com/Shelfcol/deepin-wine-ubuntu

    ```shell
    # 解压后切换到解压文件目录，在终端中运行（授予可执行权限后）： 
    ./install.sh
    # 但是其中有依赖未安装:
    # dpkg: 依赖关系问题使得 deepin-wine32:i386 的配置工作不能继续：deepin-wine32:i386 依赖于 libasound2-plugins.
     解决: 
     sudo apt install libasound2-plugins:i386
    # 启动百度云
    /opt/deepinwine/apps/Deepin-BaiduNetDisk里面的sh文件
    ```

    

43. Could not find a package configuration file provided by "costmap_converter"
      with any of the following names:
    ```shell
    costmap_converterConfig.cmake
    costmap_converter-config.cmake
    sudo apt-get install ros-kinetic-costmap-converter
    # 下载这个package，放到catkin_ws/src下面
    # cannot find sdl_package:
    sudo apt-get install libsdl-dev
    ```


44. 安装googletest

    ```shell
    make install
    make sure build tools and cmake are installed
    sudo apt install build-essential cmake
    download and extract sources
    wget https://github.com/google/googletest/archive/release-1.10.0.tar.gz
    tar -xf release-1.10.0.tar.gz
    compile
    mkdir build
    cd build
    cmake ../googletest-release-1.10.0
    make
    install
    DESTDIR=/path/to/install make install
    (I prefer installing it next to my current project. To install it globally you may use sudo make install instead, but I would not recommend it, since it will be hard to cleanup when you remove or update googletest.)
    cleanup
    cd ..
    rm -rf build googletest-release-1.10.0 release-1.10.0.tar.gz
    ```


45. git@github.com: Permission denied (publickey). fatal: Could not read from remote repository的问题
    
    解决:https://blog.csdn.net/dotphoenix/article/details/100130424
    
46. pytorch install(no anaconda):
    Github地址：https://github.com/pytorch/pytorch
    
    ```shell 
    # 错误1
    Some depencies:
    pip install numpy ninja pyyaml mkl mkl-include setuptools cmake cffi
    	(optional)conda install -c pytorch magma-cuda102  # or [ magma-cuda101 | magma-cuda100 | magma-cuda92 ] depending on your cuda version
    Get the source code:
    	git clone --recursive https://github.com/pytorch/pytorch(if you find some packages are null,don't worry,just pass it)
    # 解决办法：
    	cd pytorch
    	git submodule sync
    	git submodule update --init --recursive
    	
    # 错误2
    	python3 ./tools/build_libtorch.py(this will mkdir build,make sure it is python3!!!!!!!!!!!!!!!!!!!!!!!)
    FAILED: ../torch/csrc/autograd/generated/Functions.cpp ../torch/csrc/jit/generated/generated_unboxing_wrappers_0.cpp ../torch/csrc/jit/generated/generated_unboxing_wrappers_1.cpp ../torch/csrc/jit/generated/generated_unboxing_wrappers_2.cpp ../torch/csrc/autograd/generated/VariableType_0.cpp ../torch/csrc/autograd/generated/VariableType_1.cpp ../torch/csrc/autograd/generated/VariableType_2.cpp ../torch/csrc/autograd/generated/VariableType_3.cpp ../torch/csrc/autograd/generated/VariableType_4.cpp ../torch/csrc/autograd/generated/ProfiledType_0.cpp ../torch/csrc/autograd/generated/ProfiledType_1.cpp ../torch/csrc/autograd/generated/ProfiledType_2.cpp ../torch/csrc/autograd/generated/ProfiledType_3.cpp ../torch/csrc/autograd/generated/ProfiledType_4.cpp ../torch/csrc/autograd/generated/TraceType_0.cpp ../torch/csrc/autograd/generated/TraceType_1.cpp ../torch/csrc/autograd/generated/TraceType_2.cpp ../torch/csrc/autograd/generated/TraceType_3.cpp ../torch/csrc/autograd/generated/TraceType_4.cpp ../torch/csrc/autograd/generated/Functions.h ../torch/csrc/autograd/generated/variable_factories.h ../torch/csrc/autograd/generated/VariableType.h ../torch/csrc/autograd/generated/python_functions.cpp ../torch/csrc/autograd/generated/python_variable_methods.cpp ../torch/csrc/autograd/generated/python_torch_functions.cpp ../torch/csrc/autograd/generated/python_nn_functions.cpp ../torch/csrc/autograd/generated/python_functions.h ../torch/testing/_internal/generated/annotated_fn_args.py 
    # 解决办法
    cd /home/gxf/software/pytorch && /usr/bin/python3 tools/setup_helpers/generate_code.py --declarations-path /home/gxf/software/pytorch/build/aten/src/ATen/Declarations.yaml --nn-path aten/src   --force_schema_registration
    
    # 错误3
    Traceback (most recent call last):
      File "tools/setup_helpers/generate_code.py", line 129, in <module>
    	main()
      File "tools/setup_helpers/generate_code.py", line 124, in main
    	options.force_schema_registration,
      File "tools/setup_helpers/generate_code.py", line 35, in generate_code
    	from tools.autograd.gen_annotated_fn_args import gen_annotated
      File "/home/gxf/software/pytorch/tools/autograd/gen_annotated_fn_args.py", line 45
    	return f"{namespace}.{op_name(func)}: {out_args!r},"
    	                                                   ^
    SyntaxError: invalid syntax
    
    # 成功：源码安装的pytorch没法使用，所以用pip安装，然后直接用python37就可以使用：
    pip install torch==1.2.0 torchvision==0.4.0 -f https://download.pytorch.org/whl/torch_stable.html
    # 安装过程中可能因为网络问题一直retrying，但是没关系，等着就好了
    # 可以直接下载whl文件进行下载，这样会比较快，成功的版本有：
    # 表示python3.7,pip37是我为python3.7配置的pip
    sudo pip37 install torchvision-0.3.0-cp37-cp37m-linux_x86_64.whl  torch-1.1.0-cp37-cp37m-linux_x86_64.whl    37
    sudo  pip install torchvision-0.3.0-cp27-cp27mu-linux_x86_64.whl torch-1.1.0-cp27-cp27mu-linux_x86_64.whl
    torch
    # 卸载：
    sudo pip37 uninstall torch torchvision
    ```
    
44. install navidation错误
	
	```shell
	ERROR: could not find SDL
	解决：sudo apt-get install libsdl-dev
	
	ERROR: could not find SDL_image
	解决：sudo apt-get install libsdl-image1.2-dev
	
	ERROR: No package 'orocos-bfl' found:make oroscos-bfl source code
	/home/gxf/catkin_ws_complex/src/complex_mechanism/joystick_drivers/wiimote/include/wiimote/wiimote_controller.h:52:35: fatal error: cwiid.h: No such file or directory
	解决：sudo apt-get install libswiid-dev
	
	ERROR: /home/gxf/catkin_ws_complex/src/complex_mechanism/joystick_drivers/spacenav_node/src/spacenav_node.cpp:40:30: fatal error: spnav.h: No such file or directory
	解决：sudo apt-get install libspnav-dev
	```
	
	
	
48. there is no file  /dev/ttyUSB*

    可能是设备或者线坏了

    

49. ubuntu不能输入拼音

    教程：https://blog.csdn.net/lupengCSDN/article/details/80279177?utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control&dist_request_id=1328769.69194.16176780383119303&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control
    system setting->language setting ,将ibus改成fcitx，将菜单和窗口的语言中中文左键点到前面，然后重启。

    Ubuntu20.04有自带的中文输入法，避免安装其输入法系统崩溃。

50. doxygen

      - https://www.jianshu.com/p/4e4ce6d6c666
      - https://blog.csdn.net/czyt1988/article/details/8901191
      - https://www.ibm.com/developerworks/cn/aix/library/au-learningdoxygen/index.html
      - https://blog.csdn.net/joyjj0218/article/details/49888453?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.compare&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.compare

    ```shell
    # 在工程目录的根目录同级处terminal
    doxygen -s -g doxyconfig
    
    (1)配置文件：
    # 项目名称，多个单词需要使用引号(“”)。
    PROJECT_NAME = “Hailuo project”
    # 输出注释文档的目录，如果为空，表示在当前目录，建议写上表示本工程的有意义的目录名称。
    OUTPUT_DIRECTORY = ./comment
    # 指定各种文件，我们常用为*.cpp *.c *.h，等等。
    FILE_PATTERNS = *.c *.cpp *.h *.hpp
    # 显示类的私有成员变量和函数。
    EXTRACT_PRIVATE = YES
    # 生成子目录的注释文件
    RECURSIVE=YES
    # 文档语言，可以指定为Chinese。
    OUTPUT_LANGUAGE = English
    # 默认编码为UTF-8，这样可以支持中文。
    DOXYFILE_ENCODING = UTF-8
    # 代码文件或目录，多个文件(目录)需要以空格隔开，如果不指定，表示当前目录，但是，如果指定目录且当前目录有代码文件的话，需要使用点号(“.”).表示当前目录。
    INPUT = xxx
    # 项目版本号。
    PROJECT_NUMBER = “1.0 beta”
    # 指定图片存放的目录，我们将图片放到当前目录下的image_dir目录中，因为我们的文档会出现测试图片示例。
    IMAGE_PATH = image_dir
    # html输出目录名称，默认为html目录，如果为“.”则表明为上述OUTPUT_DIRECTORY目录。
    HTML_OUTPUT= . 
    # 默认生成的，但我们不想生成。
    GENERATE_LATEX = NO，是否生成LaTeX
    
    (2)生成注释文件：doxygen doxyconfig
    ```

    

51. Python3.7 源码安装 

    ```shell
    # 必须cd /usr/src
    
    教程：https://developer.aliyun.com/article/675910	
    
    sudo apt-get install build-essential checkinstall
    sudo apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev \
    	libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev
    cd /usr/src
    sudo wget https://www.python.org/ftp/python/3.7.0/Python-3.7.0.tgz
    sudo tar xzf Python-3.7.0.tgz
    cd Python-3.7.0
    sudo ./configure --enable-optimizations
    sudo make altinstall
    
    # 用于防止替换默认的python二进制文件/ usr / bin / python
    
    make altinstall
    python3.7 -V
    #所以，我上面评论的第3步两个rm -rf应该不做
    #创建软连接时将创建的软连接命名为python37,pip37。使用的时候命令行变成python37,pip37。
    综上，第3步应为：
    
    # 依据本文作者的路径创建新的软链接,查看一下python37安装的位置，将路径修改一下
    
    sudo ln -s /usr/local/bin/python3.7 /usr/bin/python37   
    sudo ln -s /usr/local/bin/pip3.7 /usr/bin/pip37
    
    # 最后查看Python3和pip3版本
    python37 -V
    pip37 -V
    
    # 修改python默认版本
    # 不要将python默认版本改为python37！！！！！！！！！！！！！！！！！！！！！！！！！不然会出很多问题，终端打不开，不能更新
    # Ubuntu 16.04 卸载自带 Python3 引发的惨案  
    
    教程：https://blog.csdn.net/sinat_38682860/article/details/103276156
    
    # 添加 python37 的软链接
    sudo ln -s /usr/local/bin/python3.7 /usr/bin/python37
    # 添加 pip3 的软链接（这样pip3就是python3.7专用的，也可以起名为 pip37，不影响python3.5的pip3）
    sudo ln -s /usr/local/bin/pip3.7 /usr/bin/pip37
    
    
    !!!!!!!错误：将ubuntu默认版本改为python37：
    !!!!!!!sudo rm /usr/bin/python3  删除原本的软链接，我们可以发现这个文件上面有一个箭头
    !!!!!!!sudo ln -s /usr/local/bin/python3.7 /usr/bin/python3  链接3.7到python3
    python2.X修改为python3.X
    #删除原有的Python连接文件
    sudo rm /usr/bin/python
    #建立指向Python3.X的连接
    sudo ln -s /usr/bin/python3.5 /usr/bin/python
    #把路径/usr/bin/加入环境变量PATH中（一般不需要这一步）
    PATH=/usr/bin:$PATH
    #验证，命令行输入python，就会输出新的版本。
    python
    python3.X还原为python2.X
    #删除原有的Python连接文件
    sudo rm /usr/bin/python
    #建立指向Python2.X的连接
    sudo ln -s /usr/bin/python2.7 /usr/bin/python
    
    Error:
    ModuleNotFoundError: No module named '_ctypes' 
    解决记录: https://www.cnblogs.com/abeen/p/9355389.html
    #删官方原版软连接一时爽，撤回火葬场。遇到了报错ModuleNotFoundError: No module named 'CommandNotFound'
    ```



52. jsom安装

    教程： https://wiki.openstreetmap.org/wiki/JOSM/Linux

    

53. 马太原学长代码安装

    Error: ./autogen.sh: autoreconf: not found：

    解决： sudo apt-get install autoconf automake libtool

    Error: “ Unrecognized syntax identifier "proto3". This parser only recognizes "proto2"

    解决： https://blog.csdn.net/weixin_43707303/article/details/89015920

    

54. pip指定版本

    教程：https://blog.csdn.net/weixin_43279032/article/details/104042093
    
    
    
54. 安装https://gitlab.lrz.de/tum-cps/opendrive2lanelet
    
    ```shell
    # 需要安装python3.5以上，我安装的python3.7
    sudo python37 setup.py install
	
    # PyQt5安装失败：
    pip install PyQt5 -i http://pypi.douban.com/simple/ --trusted-host pypi.douban.com  加上国内镜像源
    
    Got keys from plugin meta data ("xcb")
    # 加载 opendrive2lanelet-gui报错
    教程：https://blog.csdn.net/sxcsxcsxcsxc/article/details/104961647
    
    QFactoryLoader::QFactoryLoader() checking directory path "/usr/local/bin/platforms" ...
    Cannot load library /usr/local/lib/python3.7/site-packages/PyQt5/Qt/plugins/platforms/libqxcb.so: (libxcb-xinerama.so.0: 无法打开共享对象文件: 没有那个文件或目录)
    
    解决：sudo apt-get  install libxcb-xinerama0
    ```
    
    

55. geographic安装使用

    ```shell
    # sudo apt-get install libgeographic-dev的不能使用，会找不到安装包，需要下载源码安装，并且下载源码原cmake3.5安装后使用会报错
    CMake Error at /usr/local/lib/cmake/GeographicLib/geographiclib-config-version.cmake:38 (elseif):
    given arguments:
    "MSVC" "AND" "NOT" "(" "MSVC_TOOLSET_VERSION" "GREATER_EQUAL" "0" "AND" "MSVC_TOOLSET_MAJOR" "EQUAL" "0" ")
    
    # 解决办法更新cmake3.7
    https://blog.csdn.net/li872149120/article/details/77892127
    ```
    
    
    
57. rosdep update 

    Error：rosdep update urlopen error [Errno 111] Connection refused
    解决：https://blog.csdn.net/mrh1714348719/article/details/103803110

    

58. windows cuda cudnn安装

    教程：https://blog.csdn.net/SpadgerZ/article/details/89454247（cuda安装直接选择第一项，然后用默认的安装）

    

59. 无法连接NVIDIA驱动

    Error：NVIDIA-SMI has failed because it couldn't communicate with the NVIDIA driver
    教程：https://blog.csdn.net/hangzuxi8764/article/details/86572093

    ```shell
    sudo apt-get install dkms
    # 注意自己安装的驱动版本号得对应
    sudo dkms install -m nvidia -v 418.56
    ```

    

60. libtorch的cuda10.0的下载地址,这个是ABI=0对应的版本，用不了
    windows：
    https://download.pytorch.org/libtorch/cu100/libtorch-win-shared-with-deps-1.2.0.zip
    linux：
    https://download.pytorch.org/libtorch/cu100/libtorch-shared-with-deps-1.2.0.zip

61. ORB-SLAM2引用torch的方法
	
    c++使用pytorch的时候只需要下载对应的libtorch，然后根据官网上的c++ example（https://pytorch.org/tutorials/advanced/cpp_export.html）用几个命令即可在CMakeLists.txt中链接上。
	当将pytorch写入CMakeLists.txt之后，发现pangolin，DBoW3的为未定义的引用，原本这些库都是没有问题的，所以明显是torch的问题。是因为我下载的Libtorch 的 cxx11 ABI =0,而其他库编译的ABI=1，所以需要寻找相应的libtorch
	
	教程： https://github.com/pytorch/pytorch/issues/17492
	
	Libtorch with cxx11 ABI are now available for download:
	
	v1.2.0:
	
	CPU:
	https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.2.0.zip
	https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-without-deps-1.2.0.zip
	https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-static-with-deps-1.2.0.zip
	https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-static-without-deps-1.2.0.zip
	
	CUDA 9.2:
	https://download.pytorch.org/libtorch/cu92/libtorch-cxx11-abi-shared-with-deps-1.2.0.zip
	https://download.pytorch.org/libtorch/cu92/libtorch-cxx11-abi-shared-without-deps-1.2.0.zip
	https://download.pytorch.org/libtorch/cu92/libtorch-cxx11-abi-static-with-deps-1.2.0.zip
	https://download.pytorch.org/libtorch/cu92/libtorch-cxx11-abi-static-without-deps-1.2.0.zip
	
	CUDA 10.0:
	https://download.pytorch.org/libtorch/cu100/libtorch-cxx11-abi-shared-with-deps-1.2.0.zip
	https://download.pytorch.org/libtorch/cu100/libtorch-cxx11-abi-shared-without-deps-1.2.0.zip
	https://download.pytorch.org/libtorch/cu100/libtorch-cxx11-abi-static-with-deps-1.2.0.zip
	https://download.pytorch.org/libtorch/cu100/libtorch-cxx11-abi-static-without-deps-1.2.0.zip
	
	Nightly builds:
	
	CPU:
	https://download.pytorch.org/libtorch/nightly/cpu/libtorch-cxx11-abi-shared-with-deps-latest.zip
	https://download.pytorch.org/libtorch/nightly/cpu/libtorch-cxx11-abi-shared-without-deps-latest.zip
	https://download.pytorch.org/libtorch/nightly/cpu/libtorch-cxx11-abi-static-with-deps-latest.zip
	https://download.pytorch.org/libtorch/nightly/cpu/libtorch-cxx11-abi-static-without-deps-latest.zip
	
	CUDA 9.2:
	https://download.pytorch.org/libtorch/nightly/cu92/libtorch-cxx11-abi-shared-with-deps-latest.zip
	https://download.pytorch.org/libtorch/nightly/cu92/libtorch-cxx11-abi-shared-without-deps-latest.zip
	https://download.pytorch.org/libtorch/nightly/cu92/libtorch-cxx11-abi-static-with-deps-latest.zip
	https://download.pytorch.org/libtorch/nightly/cu92/libtorch-cxx11-abi-static-without-deps-latest.zip
	
	CUDA 10.0:
	https://download.pytorch.org/libtorch/nightly/cu100/libtorch-cxx11-abi-shared-with-deps-latest.zip
	https://download.pytorch.org/libtorch/nightly/cu100/libtorch-cxx11-abi-shared-without-deps-latest.zip
	https://download.pytorch.org/libtorch/nightly/cu100/libtorch-cxx11-abi-static-with-deps-latest.zip
	https://download.pytorch.org/libtorch/nightly/cu100/libtorch-cxx11-abi-static-without-deps-latest.zip

​      最后我下载的版本是libtorch-cxx11-abi-shared-with-deps-1.2.0.zip
将libtorch下载完成后，重新链接此库，然后将ORBSLAM2的build文件夹删干净，然后重新编译

​      官网地址：https://pytorch.org/



62. 源码阅读软件understand安装
    教程： https://m.jb51.net/softs/633733.html

    
    
63. google浏览器屏蔽特定网站
    
    https://blog.csdn.net/oLevin/article/details/50352485

    - 需要在Hosts文件中添加规则
    
    - 以管理员身份运行【记事本】
    
    - 打开C:\system32\drivers\etc\hosts

    - 添加规则，127.0.0.1   需要禁止的网址，比如:
      127.0.0.1  www.baidu.com
    
      
    
64. vscode使用leetcode

    提示invalid- password：https://blog.csdn.net/Horace__liu/article/details/104017270

    

65. protobuf安装

    ```shell
    教程：https://blog.csdn.net/luckytanggu/article/details/105977834
    
    # step1：安装相关依赖
    $ sudo apt-get install autoconf automake libtool curl make g++ unzip
    
    # step2：下载protobuf2.6压缩包
    $ wget https://github.com/protocolbuffers/protobuf/archive/v2.6.0.zip
    
    # step3：解压压缩包并进到解压后的文件夹
    $ unzip v2.6.0.zip && cd protobuf-2.6.0
    
    # step4：由于被墙的原因，我们需要修改“autogen.sh”，修改后如下（24、25行替换22、23行）
    $ vim autogen.sh
     20 if test ! -e gtest; then
     21   echo "Google Test not present.  Fetching gtest-1.5.0 from the web..."
     22   # curl http://googletest.googlecode.com/files/gtest-1.5.0.tar.bz2 | tar jx
     23   # mv gtest-1.5.0 gtest
     24   curl -L https://github.com/google/googletest/archive/release-1.5.0.tar.gz | tar zx
     25   mv googletest-release-1.5.0 gtest
     26 fi
    
    # step5：执行“autogen.sh”（执行后会生成“configure”文件）
    $ sh autogen.sh
    
    # step6：执行“configure”，指定安装到“/opt/protobuf2”目录下
    $ ./configure --prefix=/opt/protobuf2
    
    # step7：安装
    $ make && make install
    
    # step8：通过ln软连接或者环境变量添加“protoc”命令（安装完后protoc命令路径在/opt/protobuf2/bin/protoc）
    # ln软连接方式
    $ ln -s /opt/protobuf2/bin/protoc /usr/local/bin/protoc
    
    # 环境变量方式
    $ vim ~/.bashrc
    export PROTOC=/opt/protobuf2
    export PATH=$PATH:$PROTOC/bin
    
    $ source ~/.bashrc
    
    # step9：Protobuf是C++语言开发的，官方的protoc编译器中并不支持Go语言，需要安装插件才能生成Go代码
    #$  go get github.com/golang/protobuf/protoc-gen-go
    
    验证：
    # step1：创建一个proto文件
    $ vim test.proto
    message Foo {}
    
    # step2：生成go代码
    $ protoc --cpp_out=./ test.proto
    
    # 执行成功后会看到“test.pb.cc  test.pb.h ”文件
    ```

    protobuf使用教程：

    https://zhuanlan.zhihu.com/p/88544689

    https://blog.csdn.net/liuxiaodong400/article/details/89496567

    开始一直显示protoc --version:  protobuf3，后面将catkin_ws里面的devel全部删除之后显示链接不到protoc。然后就是修改FindProtobuf.cmakeprotobuf

    重新安装之后，需要更改 FindProtobuf.cmake里面的

    set(Protobuf_INCLUDE_DIR "/opt/protobuf2/include")
    set(Protobuf_PROTOC_EXECUTABLE "/opt/protobuf2/bin/protoc")

    

66. vscode常用插件

- ROS：在linux下安装该插件可以跳出ros里自带的消息等。

- Code Spell Checker：   检查拼写错误，对于我这种经常顺序拼反的人特别有用，但要注意命名规范。

- Bracket-pair colorizer2：  彩色括号以及配对

- better comment  有更多注释的功能包括， todo ! // //  ？

- TODO highlight  TODO:  与 FIXEME: 两个命名有深色块的功能。

- GitLens 增强自带Git功能 https://www.codenong.com/js95a1a06ac0fb/

-  Darcula Theme 深色IDE主题，不实用
- [Markdown] 使用vscode开始Markdown写作之旅
https://blog.csdn.net/aihe2639/article/details/101277623
markdown math + markdwon preview enhanced + markdwon all in one

67. gcc 版本降级

    将gcc版本从7.3降级为5.5版本
    ```shell
    # 首先查看自己的gcc版本，Ubuntu18.04上默认的是7.3版本
    gcc --version
    
    gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
    Copyright (C) 2017 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions. There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    
    # 降级方法
    1. 下载gcc/g++ 5
       sudo apt-get install -y gcc-5
       sudo apt-get install -y g++-5
    2. 链接gcc/g++实现降级
       cd /usr/bin
       sudo rm gcc
       sudo ln -s gcc-5 gcc
       sudo rm g++
       sudo ln -s g++-5 g++
    # 确认版本降级成功
    # 再次查看gcc版本，可以看到已经降级.
    gcc --version
    
    gcc (Ubuntu 5.5.0-12ubuntu1) 5.5.0 20171010
    Copyrigh t (C) 2015 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions. There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.



68. GTSAM安装

 cmake -DGTSAM_BUILD_WITH_MARCH_NATIVE=OFF -DCMAKE_BUILD_TYPE=Release -DGTSAM_USE_SYSTEM_EIGEN=ON ..

可以解决:error: static assertion failed: Error: GTSAM was built against a different version of Eigen static_assert