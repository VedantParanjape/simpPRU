FROM simppru/arm32-build-image:latest
RUN apt install -y python3

COPY entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
