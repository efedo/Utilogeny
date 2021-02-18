FROM alpine:latest
CMD ["/bin/sh"]
RUN /bin/sh -c "apk update"
RUN /bin/sh -c "apk add build-base git cmake ninja"
ENV PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin



