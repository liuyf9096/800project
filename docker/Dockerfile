
FROM arm32v7/debian:bullseye-slim
# FROM debian:bullseye-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y build-essential
RUN apt install -y qt5-qmake
RUN apt install -y qtbase5-dev
RUN apt install -y libzmq5-dev
RUN apt install -y libmosquitto-dev

RUN rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY ./MQLink /workspace

RUN qmake /workspace/src/MQLink.pro -o /workspace/src/Makefile

WORKDIR /workspace/src
RUN make
RUN make clean
WORKDIR /workspace/output

RUN echo "Completed"

EXPOSE 5555

CMD ["./MQLink"]
