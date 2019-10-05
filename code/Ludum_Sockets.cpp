internal void HostInit(Connection *state) {
    state->listener = sfTcpListener_create();
    sfTcpListener_listen(state->listener, 5940, sfIpAddress_getLocalAddress());
    state->socket = sfTcpSocket_create();
}

internal void ClientInit(Connection *state) {
    state->socket = sfTcpSocket_create();
}

internal void HostAccept(Connection *state) {
    sfTcpListener_accept(state->listener, &state->socket);
}

internal void ClientConnect(Connection *state, sfIpAddress ip) {
    sfTcpSocket_connect(state->socket, ip, 5940, sfTime());
}
