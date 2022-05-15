FROM ubuntu:latest

RUN apt-get -q -y update \
 && apt-get -q -y install build-essential telnet iproute2 make

COPY /src/Server.c /home/Server.c
COPY /src/Server.h /home/Server.h
COPY /src/main.c /home/main.c
COPY /src/main.h /home/main.h
COPY /src/makefile /home/makefile


ADD wrapper /usr/bin/
RUN chmod a+x /usr/bin/wrapper

ENTRYPOINT ["wrapper"]