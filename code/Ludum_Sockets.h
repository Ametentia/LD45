#if !defined(LUDUM_SOCKETS_H_)
#define LUDUM_SOCKETS_H_

struct Connection {
    sfTcpSocket *socket;
    sfTcpListener *listener;
};

#endif // LUDUM_SOCKETS_H_
