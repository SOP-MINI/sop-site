# Dockerfile
FROM alpine:3.19 AS builder
RUN apk add --no-cache gcc make musl-dev
WORKDIR /app
COPY demo.c Makefile /app/
RUN make demo

FROM alpine:3.19
COPY --from=builder /app/demo /demo
ENTRYPOINT ["/demo"]
