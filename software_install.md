1.印象笔记：
	建议从Nixnote官网下最新的：https://sourceforge.net/projects/nevernote/，建议别用ppa的，这个好像会少拉一些包，而且没有图标。 安装中途会提示libtbb2和libcurl3，apt-get安装就行了 然后在'file文件'中建立账户,然后使用建立的账户，选择“印象笔记”（这个是国服），使用'工具'中的同步，这时会进入印象笔记大陆的服务器，如果选择的是印象笔记国际版，进入的就是国际版。两个域名是不一样的，在没登录前可以在~/.nixnote/accounts.conf中看到。登录页面左上角也不一样，国际版是Eventnote，国服是印象笔记。  在登录国服时候，输入完邮箱地址一直不会显示密码框，这个时候点击左上角的“印象笔记”链接，然后会打开网页版的印象笔记页面，在里面找到登录页面，正常登录。然后关闭登录框，再从'工具'中的同步进入，这个时候就会看到授权提示了。

2.slam的评估轨迹的包：
	https://github.com/uzh-rpg/rpg_trajectory_evaluation          				    rpg_trajectory_evaluation-master
	https://github.com/catkin/catkin_simple  catkin_simple
	将这两个包放在catkin_ws下面，然后catkin_make

3.VTK+PCL:      
	https://blog.csdn.net/sinat_28752257/article/details/79169647
	https://blog.csdn.net/sinat_28752257/article/details/79169647
	（但是我的新电脑直接可以用我的sh文件安装）
	
3.ros kinect:
	https://blog.csdn.net/softimite_zifeng/article/details/78632211
        sudo rosdep init: RROR:cannot download default sources list from
        需要翻墙，利用lantern翻墙，每个月可以有500M的翻墙额度：
        lantern下载：https://raw.githubusercontent.com/getlantern/lantern-binaries/master/lantern-installer-preview-64-bit.deb
        lantern安装：
        	sudo dpkg -i lantern-installer-preview-64-bit.deb
                apt-cache search libappindicator3-1
                sudo apt-get install libappindicator3-1
                然后在终端输入lantern即可启用，但要在右上角确认是否connected
        然后再sudo rosdep init，再继续后面的操作。最好不要在ros安装一开始就启用lantern，因为其额度有限，最后在sudo rosdep init出错之后再启用
        
4.U盘解锁方法：
	df -h #查看信息，找到 /dev/sdb4       284G  217G   68G   77% /media/xxx/u盘名
	sudo umount /media/xxx/u盘名 #卸载u盘（不需要拔掉u盘）（此步若不成功，可以直接做下一步）
	sudo dosfsck -v -a /dev/sdb4 #文件修复

5.tensorflow 安装并在jupyter里面运行：
	安装anaconda3
	安装tensorflow：https://www.cnblogs.com/guohaoblog/p/9319791.html（(tensorflow)D:\>ipython kernelspec install-self --user很重要）
	要使用tensorflow，先进入tf虚拟环境：source activate tf，然后在里面输入jupyter notebook，进入jupyter。要退出tf虚拟环境：source deactivate tf

5.sophus安装：
	git clone http://github.com/strasdat/Sophus.git
	git checkout a621ff  (!!!!!!!!!!!!!!!!!!!very important)
	mkdir build
	cd build
	cmake ..
	make

6.sophus使用问题：
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
	
	解决办法：https://blog.csdn.net/u010003609/article/details/68961293
		  set( THIRD_PARTY_LIBS   ${Sophus_LIBRARIES} libSophus.so)

7.运行PUBG的错误：

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

	解决：下载DirectX修复工具http://www.onlinedown.net/soft/120082.htm

8.SolidWorks2018安装：
	软件安装管家上下载，然后按照教程http://www.cadzxw.com/11054.html

9.卸载pcl库：
	在build文件夹里面sudo make uninstall,千万不要随便手动到usr文件家里面删任何东西

10.安装google的cartographer：
	https://google-cartographer-ros.readthedocs.io/en/latest/demos.html
        巨坑：开始自己装了一个pcl-1.9,就报各自pcl的time_h找不到之类查不到的错误，还有boost，我怀疑是pcl太新，导致与boost等库不兼容，以及与cartographer不兼容，联想到ros里面预装了一个pcl-1.7,所以我就将pcl-1.9卸载，然后再重新按照网址安装cartographer（需要执行所有步骤），就成功，所以不要装其他的pcl，ros自带的就OK

11.vs2017安装完后再装2015：
	只选择通用windows应用开发工具中的windows10 SDK（10.0.10586）
	
12.keyshot8.2：
	 http://www.gfxcamp.com/keyshot-pro-8280/
	 
13.imu使用:
	https://blog.csdn.net/qq_31356389/article/details/79216765
        sudo apt-get install ros-kinetic-xsens-driver
        rosstack profile
        rospack profile
	sudo chmod 777 /dev/ttyUSB*#赋予权限，使用前必备操作
	roslaunch xsens_driver xsens_driver.launch
	rostopic list
	rostopic echo /imu/data
	
	如果要修改imu的frame_id，则需要下载驱动包进行安装，再修改launch文件
	安装好后的使用：
		sudo chmod 777 /dev/ttyUSB*
		roslaunch xsens_driver xsens_driver.launch

14.Opencv安装：
	https://www.cnblogs.com/arkenstone/p/6490017.html，WITH_QT=false
     	cmake -D CMAKE_BUILD_TYPE=RELEASE -D INSTALL_PYTHON_EXAMPLES=ON -D INSTALL_C_EXAMPLES=OFF  -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.2.0/modules -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=OFF  -D WITH_GTK=ON -D WITH_OPENGL=ON  -D BUILD_EXAMPLES=ON ..
     	如果cmake这一步出太多错，就直接
     	sudo cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_QT=OFF -D OPENCV_EXTRA_MODULES_PATH= .. 
     	sudo make -j4 sudo make install 
     	sudo ldonfig
	https://blog.csdn.net/weixin_41896508/article/details/80795239
	可能需要下载ippicv20151201,替换opencv-3.0.0/3rdparty/ippicv/downloads/linux-8b449a536a2157bcad08a2b9f266828b下面的压缩包即可
	make之后的配置https://www.jianshu.com/p/88ad5441c2ae


15.翻译软件的安装（可以鼠标取词):
	https://www.jianshu.com/p/92e96b2ad693
	朗道http://download.huzheng.org/zh_CN/
	只需要安装朗道英译汉就可以了，如果误装了词典，删掉就可以sudo rm -rf filenam

16.ubuntu16.04美化:
	unity: 
		点击图标进出文件https://jingyan.baidu.com/article/d45ad148bc51bb69552b80dc.html
	gnome tweaks：
		更换桌面背景（似乎不用安装，直接桌面右键即可更换背景）(软件中心安装，图标在usr/share/backgrounds)https://blog.csdn.net/github_36498175/article/details/80161362
	图标在左边和下边切换:
		gsettings set com.canonical.Unity.Launcher launcher-position Left
                gsettings set com.canonical.Unity.Launcher launcher-position Bottom
                
17.velodyne:
	https://www.jianshu.com/p/1274c002ef5f

