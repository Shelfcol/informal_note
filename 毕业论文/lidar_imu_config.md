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

​      
