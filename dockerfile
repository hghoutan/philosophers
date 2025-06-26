FROM debian:bullseye

WORKDIR /philosophers

RUN apt-get update && apt-get install -y \
    build-essential \
    valgrind \
    gcc \
    make \
    && apt-get clean
