FROM simppru/amd64-build-image:latest

RUN apt install cmake
COPY entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
