#include "include/utils/netThread.hpp"


void sendData (udp_client_server::udp_client& client)
{
    double loopTime = 1.0 / LOOPS_PER_SEC;
    std::string msg;

    while (true)
    {
        // Stop sending data if an agreed symbol is received
        if (buff[0] != STOP_SIGNAL)
        {
            clock_t start = clock();
            // Check if the angles are not NaN or inf and within restrictions
            //if ((buff[0] == RESUME_SIGNAL || buff[0] == START_SIGNAL) && std::isfinite(yaw) && std::isfinite(pitch) && std::abs(yaw) < 30.0 && pitch < 90.0)
            if ((buff[0] == GET_SIGNAL || buff[0] == START_SIGNAL) && std::isfinite(yaw) && std::isfinite(pitch) && std::abs(yaw) < 30.0 && pitch < 90.0)
            {

                msg = std::to_string(yaw) + " " + std::to_string(pitch);
                std::cerr << "Sent Data:  " << msg << "\n";
                client.send(msg.c_str(), strlen(msg.c_str()));
            }
            else
            {
                std::cerr << "Data not sent, buff = " << buff << "\n";
            }

            clock_t end = clock();
            double timeElapsed = (double) (end - start) / CLOCKS_PER_SEC;	

            // Pause until loop ends
            if (timeElapsed < loopTime)
            {
                unsigned int microseconds = static_cast<int>((loopTime - timeElapsed) * MICROSEC_TO_SEC);
                //std::cerr << "Loop took " << timeElapsed << " seconds and stalling for " << static_cast<double>(microseconds) / MICROSEC_TO_SEC << " seconds\n";
                usleep(microseconds);
            }
            // Not on time
            else 
            {
                //std::cerr << "Loop took " << timeElapsed << " seconds and new FPS = " << fps << " [ERROR]\n";
            }
        }
    }
}

void receiveData (udp_client_server::udp_server& server)
{
    int maxBufferSize = 15;
    int maxWaitSec = 1;
    while (true)
    {
        // Receive data from non-blocking server
        server.timed_recv(buff, maxBufferSize, maxWaitSec);
        std::cerr << "Received Data: " << buff << "\n";
        if (buff[0] == STOP_SIGNAL)
            std::cerr << "STOP_SIGNAL\n";
        else if (buff[0] == START_SIGNAL)
            std::cerr << "START_SIGNAL\n";
        else if (buff[0] == GET_SIGNAL)
            std::cerr << "GET_SIGNAL\n";
        else if (buff[0] == RESUME_SIGNAL)
            std::cerr << "RESUME_SIGNAL\n";
        else if (buff[0] == PAUSE_SIGNAL)
            std::cerr << "PAUSE_SIGNAL\n";
    }
}

void receivePing (udp_client_server::udp_server& server)
{
    int maxBufferSize = 1;
    int maxWaitSec = 1;
    char buff [] = "0";
    while (buff[0] != START_SIGNAL)
    {
        // Receive data from non-blocking server
        server.timed_recv(buff, maxBufferSize, maxWaitSec);
    }
    std::cerr << "Received start signal: " << buff << "\n";
}

void sendPing (udp_client_server::udp_client& client)
{
    std::string msg = "@";
    printf("Sent Data: %s\n", msg.c_str());
	client.send(msg.c_str(), strlen(msg.c_str()));
}

