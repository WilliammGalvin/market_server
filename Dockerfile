FROM fedora:38

RUN dnf install -y gcc-c++ cmake make

WORKDIR /app

COPY . .

RUN mkdir build && cd build && cmake .. && make

EXPOSE 5000

CMD ["./build/market_server"]
