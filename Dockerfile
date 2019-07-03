FROM clux/muslrust:nightly as build

COPY ./ ./

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y install ca-certificates libssl-dev libssh2-1-dev && rm -rf /var/lib/apt/lists/*
ENV PKG_CONFIG_ALLOW_CROSS=1

RUN cargo build --target x86_64-unknown-linux-musl --release
RUN mkdir -p /build-out
RUN strip target/x86_64-unknown-linux-musl/release/pommedapi
RUN cp target/x86_64-unknown-linux-musl/release/pommedapi /build-out/
RUN mkdir /build-out/share
RUN cp -fr data/html/data/css data/html/data/js data/html/data/fonts /build-out/share/
RUN cp -fr data/templates /build-out/share/


FROM alpine:latest

RUN apk add --no-cache bash jq

COPY --from=build /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/ca-certificates.crt
COPY --from=build /build-out/pommedapi               /pommedapi
COPY --from=build /build-out/share                   /usr/local/share/pommedapi

ENV SSL_CERT_FILE=/etc/ssl/certs/ca-certificates.crt
ENV SSL_CERT_DIR=/etc/ssl/certs
