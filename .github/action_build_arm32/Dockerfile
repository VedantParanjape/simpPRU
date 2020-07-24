FROM arm32v7/debian:buster

RUN uname -r
RUN apt update
RUN apt install -y git build-essential flex bison autoconf automake

COPY entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
