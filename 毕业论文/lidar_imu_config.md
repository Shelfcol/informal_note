1. imu和velodyne16的时间同步
   https://github.com/ros-drivers/velodyne/issues/183
   
2. usb_cam使用和标定：https://blog.csdn.net/heyijia0327/article/details/43538695

   https://github.com/Livox-SDK/livox_camera_lidar_calibration#notes

   保存话题里面的iamge_raw  : rosrun image_view image_saver image:=/usb_cam/image_raw _save_all_image:=false _filename_format:=foo.jpg __name:=image_saver  

   rosservice call /image_saver/save 触发保存图片

3. AprilTag ros 使用：https://blog.csdn.net/weixin_43331257/article/details/83271761

   1. 修改launch文件里的摄像头topic的名称：
        打开apriltags_ros/launch文件价下的continuous_detection.launch
      这里的参数根据自己摄像头发布的topic修改，和摄像头的topic一致才能订阅到摄像头的图像信息。
   
   2. tag_id和size的设定
        打开apriltags_ros/config文件价下的tags.yaml文件，添加需要检测的id、size、name。例如：检测id＝９,size=0.05(m)的标签，name可以缺省，可以添加多个。
      roslaunch apriltags_ros continuous_detection.launch
      rostopic　echo　/tag_detections
   
   3.  可以给每个tag设置xyz坐标
   
4. lidar camera标定  

   制作标定板，直接用apriltag得到相机检测得到的关键点，即为tag的中心。然后lidar利用rviz选点，得到对应的关键点。最后SVD求解T

   启动lidar，然后rostopic echo /clicked_point, 在rviz上点击Publish Point，再用鼠标点击对应点，则会echo出相应的点坐标

   将对应点保存到csv里面，前三个数是摄像机关键点xyz，后三个数是lidar关键点xyz

   R = [-0.4740870620703534, -0.6776151402737751, 0.5622091952727697;
     -0.7985958878874178, 0.06202661381151492, -0.5986629327329247;
     0.3707911345040629, -0.732796302427101, -0.5705466797049662]
   t = [-0.2372337037635569;
     -0.0354852655601314;
     0.2392597169997224]
   R_inv = [-0.4740870620703534, -0.7985958878874178, 0.3707911345040629;
     -0.6776151402737751, 0.06202661381151492, -0.732796302427101;
     0.5622091952727697, -0.5986629327329247, -0.5705466797049662]
   t_inv = [-0.2295231987056953;
     0.01677651734663871;
     0.2486400936567945]

rosbag record /velodyne_points /tag_detections  /tag_detections_image /tag_detections_image/compressed  /tag_detections_image/compressed/parameter_descriptions /tag_detections_image/compressed/parameter_updates /tag_detections_image/compressedDepth /tag_detections_image/compressedDepth/parameter_descriptions /tag_detections_image/compressedDepth/parameter_updates /tag_detections_image/theora /tag_detections_image/theora/parameter_descriptions /tag_detections_image/theora/parameter_updates

