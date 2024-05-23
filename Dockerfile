FROM ubuntu:latest

RUN apt-get update && apt-get install -y sqlite3 libsqlite3-dev

RUN apt-get install -y g++

COPY . /app

WORKDIR /app

RUN g++ main.cpp -o d_random -lsqlite3

CMD ["./d_random"]