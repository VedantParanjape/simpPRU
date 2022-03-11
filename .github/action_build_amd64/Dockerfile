FROM simppru/amd64-build-image:latest

RUN apt update
RUN apt install -y cmake
COPY entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
