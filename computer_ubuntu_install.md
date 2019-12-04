一.  msi新电脑未重装windows之前的折腾
1.安装前的首次：https://blog.csdn.net/wxc971231/article/details/88033261
2.合上盖子无法唤醒：将电源设置的 合上盖子时 选 不处理
3.循环登陆：https://blog.csdn.net/ssmixi/article/details/73483795
4.分辨率只有一个值：https://blog.csdn.net/weixin_42521913/article/details/90767336
                  		1280*1024，最佳分辨率 1920*1080
5.但是进不去图形界面。但我在选择试用Ubuntu后遇到了问题，开在了开机动画的界面，进入不了系统。这时需要关机重启，选择U盘启动，然后到显示有"Try ubuntu without installing."的界面时，按'''e''' 进入编辑开机指令的模式, 找到'''quite splash''' 并在后面加上对应的“nomodeset”,即 ：'''quite splash nomodeset'''，然后按F10即可启动ubuntu系统。


二.  msi电脑未重装windows之后
1.我的电脑型号是msi GE63 Raider RGB 8RF，按照楼主的方法也确实解决了很多问题，但是最终就是不能在AHCI模式下面进入windows，楼主的方法我试了一下，然后windows也崩了（可能是我的操作不对）。正好当时我的ubuntu系统被我搞崩了，索性一怒之下重装ubuntu和windows，步骤如下：bios里面调整为AHCI、UEFI、关闭安全启动（secure boot)，然后进行windows的安装 （https://blog.csdn.net/kxwinxp/article/details/78829132?utm_source=app），装完以后安装ubuntu（ubuntu安装完重启，但开机后一直持续几分钟的自动重复登录之后显示The system is running in low-graphics mode并 一直重复在这个界面，搜索一阵：ctrl+alt+F1,登录账户密码后,sudo apt-get install nvidia-current , sudo shutdown -r now），最后需要在开机界面按F11进入操作系统的选择，否则会直接进入windows。这次重装之后没有像最开始那样，分辨率、wifi联网都是正常的。ubuntu的安装我推荐语言选择英文，否则你在进入ctrl+alt+F1之后会显示很多菱形，表示中文不能显示，英文就不会出现这个问题，后面装好之后ubuntu里面设置了中文输入法以后也可以显示中文
2.分辨率不对:
	打开：/etc/default/grub
	搜索：#GRUB_GFXMODE=640x480
	编辑：640x480改成你想要的分辨率，并取消前面的#
	例如：GRUB_GFXMODE=1920x1080 
	GRUB_CMDLINE_LINUX_DEFAULT="quiet splash nomodeset"
	更新：sudo update-grub
	reboot
3.nvidia驱动安装:
	https://blog.csdn.net/xunan003/article/details/81665835 
	安装过程中的选择只需要默认即可 

三.ubuntu的安装
1.Ubuntu安装的步骤中分区（/区多分一点内存，这是存放库文件的位置usr， (上次装了一个texlive-full就满了，就很难受)，需要30G-50G以上，不过后来可以利用ubuntu16.04的启动盘利用gparted进行分区https://www.jianshu.com/p/e4dcb744b3a1扩容，但是这个的操作只能是在现有的ubuntu空间下，不能操作windows的磁盘）
2.分区明细：Mb（括号里的是杨宇航推荐的分区，前面是我自己摸索的分区，总的分区我给了500G，不过其实没必要这么大，300G就够了）
587763（10G） 主分区 空间起始位置 Ext4日志文件系统 /
4994（4G）逻辑分区 空间起始位置 交换空间 /swap
4000（200Mb） 逻辑分区 空间起始位置 Ext4日志文件系统 /boot
520000（剩余空间）逻辑分区 空间起始位置 Ext4日志文件系统 /home 
3.最底下的Device for boot loader installation要选/boot所在区，切记切记

