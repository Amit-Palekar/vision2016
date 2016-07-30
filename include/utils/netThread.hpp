#ifndef NET_THREAD_HPP
#define NET_THREAD_HPP

#include "include/networking/udpClientServer.hpp"

void sendData (udp_client_server::udp_client& client);
void receiveData (udp_client_server::udp_server& server);
void receivePing (udp_client_server::udp_server& server);
void sendPing (udp_client_server::udp_client& client);

#endif // NET_THREAD_HPP
