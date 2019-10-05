#if !defined(LUDUM_SOCKETS_H_)
#define LUDUM_SOCKETS_H_

#include "Ludum_Types.h"
#include "Ludum_Platform.h"
#include "Ludum_Maths.h"
#include <SFML/Network.h>

struct Connection {
    sfTcpSocket *socket;
    sfTcpListener *listener;
};

#endif // LUDUM_SOCKETS_H_
