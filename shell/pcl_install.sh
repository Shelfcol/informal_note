#git clone https://github.com/PointCloudLibrary/pcl.git
cd pcl && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
make -j2
sudo make -j2 install
