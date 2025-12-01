FROM fedora:38

RUN dnf install -y gcc-c++ cmake make

WORKDIR /app

COPY . .

RUN mkdir build
WORKDIR /app/build
RUN cmake .. && make

WORKDIR /app

EXPOSE 5000

ENTRYPOINT [ "./build/market_server" ]
CMD [ "data/AAPL_5Y_history.csv", "3000" ] 