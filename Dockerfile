# Étape 1 : compilation
FROM ubuntu:24.04 AS build
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY CMakeLists.txt .
COPY Headers/ Headers/
COPY Sources/ Sources/
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build

# Étape 2 : image finale (exécutable + dashboard seulement)
FROM ubuntu:24.04
WORKDIR /app
COPY --from=build /app/build/simulator .
COPY Frontend/ Frontend/
EXPOSE 8080
CMD ["./simulator"]