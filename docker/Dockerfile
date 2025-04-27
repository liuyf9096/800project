# 使用树莓派官方的 Debian 基础镜像
# FROM arm32v7/debian:bullseye-slim
FROM debian:bullseye-slim

# 设置环境变量，避免一些交互式问题
ENV DEBIAN_FRONTEND=noninteractive

# 更新软件包列表并安装所需的软件包和依赖项
RUN apt update && apt install -y \
    build-essential \
    qt5-qmake \
    qtbase5-dev \
    libzmq5-dev \
    libmosquitto-dev \
    cmake \
	vim \
    git \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /workspace

# 复制源码到容器中
COPY ./MQLink /workspace

# 使用 qmake 生成 Makefile，并指定 Makefile 输出路径到 src 目录
RUN qmake /workspace/src/MQLink.pro -o /workspace/src/Makefile

# 切换到 src 目录并运行 make
WORKDIR /workspace/src
RUN make

# 设置容器的默认命令
CMD ["bash"]
