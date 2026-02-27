# Stage 1: Build
ARG BASE_IMAGE=cppchat-base:latest
FROM ${BASE_IMAGE} AS builder

WORKDIR /app
COPY . .

RUN rm -rf build && mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc)

# Stage 2: Runtime
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libsqlite3-0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy binaries from builder
COPY --from=builder /app/build/src/server/cppchat_server /app/
COPY --from=builder /app/build/src/client/cppchat_client /app/

# Create directory for database
RUN mkdir -p /app/data

EXPOSE 9090

CMD ["./cppchat_server", "9090"]
