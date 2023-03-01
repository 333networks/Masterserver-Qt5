FROM debian:bullseye-slim AS build

RUN apt-get update \
    && apt-get install -y build-essential libqt5core5a libqt5sql5 libqt5network5 qtbase5-dev qt5-qmake

COPY src /src
RUN cd src && qmake && make

FROM debian:bullseye-slim

RUN apt-get update \
    && apt-get install -y libqt5core5a libqt5sql5 libqt5network5 \
    && apt-get autoclean

RUN mkdir -p /masterserver/bin /masterserver/data /masterserver/log \
    && chown -R daemon:daemon /masterserver

COPY --from=build /src/MasterServer-Qt5 /masterserver/bin/

USER daemon

# skips the "create tables y/N" interactive prompt
ENV AUTO_CREATE_DB=y

ENTRYPOINT /masterserver/bin/MasterServer-Qt5