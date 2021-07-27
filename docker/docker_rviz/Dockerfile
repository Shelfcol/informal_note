ARG from
  
FROM nvidia/opengl:1.0-glvnd-runtime-ubuntu16.04  as glvnd
FROM osrf/ros:kinetic-desktop-full
  
COPY --from=glvnd /usr/local/lib/x86_64-linux-gnu /usr/local/lib/x86_64-linux-gnu
COPY --from=glvnd /usr/local/lib/i386-linux-gnu /usr/local/lib/i386-linux-gnu
  
#COPY 10_nvidia.json /usr/local/share/glvnd/egl_vendor.d/10_nvidia.json
  
RUN echo '/usr/local/lib/x86_64-linux-gnu' >> /etc/ld.so.conf.d/glvnd.conf && \
    echo '/usr/local/lib/i386-linux-gnu' >> /etc/ld.so.conf.d/glvnd.conf && \
    ldconfig
  
ENV LD_LIBRARY_PATH /usr/local/lib/x86_64-linux-gnu:/usr/local/lib/i386-linux-gnu${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
  
# nvidia-container-runtime
ENV NVIDIA_VISIBLE_DEVICES \
    ${NVIDIA_VISIBLE_DEVICES:-all}
ENV NVIDIA_DRIVER_CAPABILITIES \
    ${NVIDIA_DRIVER_CAPABILITIES:+$NVIDIA_DRIVER_CAPABILITIES,}graphics
  
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    libgl1-mesa-dev \
    libglew-dev \
    libsdl2-dev \
    libsdl2-image-dev \
    libglm-dev \
    libfreetype6-dev \
    libglfw3-dev \
    libglfw3 \
    libglu1-mesa-dev \
    freeglut3-dev \
    dbus
    
