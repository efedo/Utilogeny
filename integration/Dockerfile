FROM alpine:latest
CMD ["/bin/sh"]
RUN /bin/sh -c "apk update"
RUN /bin/sh -c "apk add build-base git cmake ninja qt5-qtbase-dev"
ENV PATH=/usr/lib/qt5/bin/:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin



