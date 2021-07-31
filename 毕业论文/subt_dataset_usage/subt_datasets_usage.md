1. 传感器配置

    Ouster OS1-64 (3D LiDAR) , a FLIR Tau2 thermal IR camera, a Carnegie Robotics Multisense SL stereo camera + illuminators + spinning LiDAR, and a Microstrain GX5-25 IMU（九轴）.

2. 传感器标定比较粗糙，可能建图不会很精确。Sufficiently motivated parties could use the tf_hijacker node to remove inaccurate transforms which could then be re-inserted through the use of a tf2_ros/static_transform_publisher.

3. 只使用了multisense摄像头，没有使用RealSense. 根据multisense-camera-robot-imu.yaml可知，cam融合使用的是Microstrain GX5-25 IMU，因为z方向有35cm的偏置。双目摄像头的基线距离为0.044+0.026=0.07m，即7cm

4. Lidar，IMU，Camera坐标关系

    left and right cam to imu：见multisense-camera-robot-imu.yaml

    ```yaml
    # Left camera
    cam0:
      T_cam_imu:
        [-0.0059751,  0.0020717, -0.9999800,  0.001, 
         -0.9996940,  0.0239903,  0.0060231,  0.026,
          0.0240023,  0.9997100,  0.0019277, -0.353,
          0.0000000,  0.0000000,  0.0000000,  1.000]
      camera_model: pinhole
      distortion_coeffs: [-0.20848242938518524, 0.1229957863688469, 0.0005989724886603653, -0.00030653737485408783]
      distortion_model: radtan
      intrinsics: [600.7606201171875, 530.7537841796875, 602.1273193359375, 272.0735778808594]
      resolution: [1024, 544]
      timeshift_cam_imu: 0.0
    # Right camera
    cam1:
      T_cam_imu:
        [-0.0059751,  0.0020717, -0.9999800,  0.001, 
         -0.9996940,  0.0239903,  0.0060231, -0.044,
          0.0240023,  0.9997100,  0.0019277, -0.352,
          0.0000000,  0.0000000,  0.0000000,  1.000]
      T_cn_cnm1:
        [1.0,   0.0,   0.0,  -0.070,
         0.0,   1.0,   0.0,   0.000,
         0.0,   0.0,   1.0,   0.000,
         0.0,   0.0,   0.0,   1.000]
      camera_model: pinhole
      distortion_coeffs: [-0.20917260646820068, 0.1225590780377388, 0.000664742139633745, 0.0001836364099290222]
      distortion_model: radtan
      intrinsics: [599.935302734375, 529.9777221679688, 601.3153076171875, 272.9176940917969]
      resolution: [1024, 544]
      timeshift_cam_imu: 0.0
    T_imu_body:
    #  [0.0000, 0.0000, -1.0000, 0.547,
    #  0.0000, 1.0000, 0.0000, -0.030,
    #  1.0000, 0.0000, 0.0000, -0.168,
    #  0.0000, 0.0000, 0.0000, 1.000]
    #  [1.0000, 0.0000, 0.0000, 0.0,
    #  0.0000, 1.0000, 0.0000,  0.0,
    #  0.0000, 0.0000, 1.0000,  0.0,
    #  0.0000, 0.0000, 0.0000, 1.000]
      [-0.9999660,  0.0080227,  0.0019037, 0.217,
      0.0079747,  0.9996800, -0.0240041, 0.000,
     -0.0020957, -0.0239881, -0.9997100, 0.196,
      0.0000000,  0.0000000,  0.0000000, 1.000]
    ```

    lidar2imu（或者imu2lidar，李宇航说一般是imu2lidar）见cartographer.launch: 平移： 0 0 0 ，旋转：3.134499 ，0.002063 ，-3.117097

    ```yaml
    <node pkg="tf2_ros" type="static_transform_publisher" name="imu_tf" args="0 0 0 3.134499 0.002063 -3.117097 $(arg name)/imu $(arg name)/imu2"/>
    ```

    

5. Camera内参

    两个cam的内参变量对应关系见：MultiSenseStere.yaml

    ```yaml
    \# Camera calibration and distortion parameters (OpenCV)
    Camera.fx: 600.7606201171875
    Camera.fy: 602.1273193359375
    Camera.cx: 530.7537841796875
    Camera.cy: 272.0735778808594
    Camera.k1: -0.20848242938518524
    Camera.k2: 0.1229957863688469
    Camera.p1: 0.0005989724886603653
    Camera.p2: -0.00030653737485408783
    Camera.k3: 0.0046302578411996365
    
    Camera.width: 1024
    Camera.height: 544
    
    \# Camera frames per second
    Camera.fps: 10.0
    
    \# stereo baseline (distance between cameras in m?) times fx
    Camera.bf: 42.0532434082 #30.464328718 # 40
    
    \# Color order of the images (0: BGR, 1: RGB. It is ignored if images are grayscale)
    Camera.RGB: 1
    
    \# Close/Far threshold. Baseline times.
    ThDepth: 30 # what is this?
    ```

6.  topic name

    ```yaml
    /chinook/assembled_cloud
    /chinook/boson/camera_info
    /chinook/boson/image_raw #FLIR Boson
    /chinook/cmd_vel
    /chinook/husky_velocity_controller/odom
    /chinook/imu/data  # chinook/base
    /chinook/joy
    /chinook/mac80211/scan
    /chinook/multisense/depth/camera_info
    /chinook/multisense/depth/compressedDepth
    /chinook/multisense/imu/accelerometer
    /chinook/multisense/imu/accelerometer_vector
    /chinook/multisense/imu/gyroscope
    /chinook/multisense/imu/gyroscope_vector
    /chinook/multisense/imu/imu_data
    /chinook/multisense/imu/magnetometer
    /chinook/multisense/imu/magnetometer_vector
    /chinook/multisense/left/camera_info
    /chinook/multisense/left/image_rect_color/camera_info
    /chinook/multisense/left/image_rect_color/compressed
    /chinook/multisense/lidar_points2
    /chinook/multisense/pps
    /chinook/multisense/right/camera_info
    /chinook/multisense/right/image_rect/camera_info
    /chinook/multisense/right/image_rect/compressed
    /chinook/multisense/stamped_pps
    /chinook/multisense/status
    /chinook/odom
    /chinook/ouster/imu_packets
    /chinook/ouster/lidar_packets
    /chinook/ouster/points  #激光雷达数据 frame_id: "chinook/ouster"
    /clock
    /diagnostics_agg
    /rosout
    /rosout_agg
    /tf
    ```
    
7. static_transform_publisher
    static_transform_publisher **x y z yaw pitch roll** frame_id child_frame_id period_in_ms
    Publish a static coordinate transform to tf using an x/y/z offset in meters and yaw/pitch/roll in radians. (yaw is rotation about Z, pitch is rotation about Y, and roll is rotation about X). The period, in milliseconds, specifies how often to send a transform. 100ms (10hz) is a good value.

    static_transform_publisher **x y z qx qy qz qw** frame_id child_frame_id  period_in_ms

8. os1lidar的坐标系与REP不同，需要修改xyz https://github.com/TixiaoShan/LIO-SAM/issues/94
