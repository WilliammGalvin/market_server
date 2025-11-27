# Market Server

A simulated trading server written in C++.

## Packet Formats

### Bar Packet

```c++
struct BarPacket {
    int64   timestamp
    double  open
    double  close
    double  high
    double  low
    int64   volume
}
```

Packet size: `48 bytes`

### Order Packet

```c++
struct Order {
    uint64     order_id
    uint32     client_id

    char[8]    symbol
    OrderSide  side
    OrderType  type

    double     price
    double     quantity

    uint64     timestamp
}
```

Packet size: `48 bytes`

**OrderSide**<br>
BUY = 0<br>
SELL = 1<br>

**OrderType**<br>
MARKET = 0<br>
LIMIT = 1<br>
