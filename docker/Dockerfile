FROM stateoftheartio/qt6:6.5-gcc-aqt

# 安装ZeroMQ和Mosquitto依赖
RUN apt update && apt install -y libzmq5 libmosquitto-dev

# 工作目录
WORKDIR /app

# 默认启动时进入bash
CMD ["bash"]
