#include "driver.h"

/**
 *  Calculate the checksum of the packet
 */
void Drone::calcChecksum(void)
{
    buffer[6] = 0;
    for (int i = 1; i < 6; i++)
        buffer[6] ^= buffer[i];

}

char Drone::_getByte(enum _byteName index)
{
    bufferMutex.lock();
    char res = buffer[index];
    bufferMutex.unlock();
    return res;
}

void Drone::_setByte(enum _byteName index, char value)
{
    bufferMutex.lock();
    buffer[index] = value;
    bufferMutex.unlock();
}

/**
 *  Thread that runs for the duration of the program, keeps the communication
 *  going and sends the packets periodically
 */
void Drone::commLoop(void)
{
    while(true)
    {
        /* Send packet */
        bufferMutex.lock();
        sendto(sockfd, (const char *) buffer, CTRL_PACKET_LEN, MSG_CONFIRM,
                (const sockaddr *) &servaddr, sizeof(servaddr));

        bufferMutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    }
}

Drone::Drone(void)
{
    /* Configure Socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family         = AF_INET;
    servaddr.sin_port           = htons(UDP_COM_PORT);
    servaddr.sin_addr.s_addr    = inet_addr(IP_ADDR);
    
    buffer[0] = START_BYTE;
    buffer[7] = END_BYTE;

    on();
    commThread = std::thread(&Drone::commLoop, this);
}


Drone::~Drone(void)
{
    commThread.join();
    stop();
    off();
    close(sockfd);
}

void Drone::on(void)
{
    bufferMutex.lock();

    buffer[1] = 0x81;
    buffer[2] = 0x84;
    buffer[3] = 0x80;
    buffer[4] = 0x84;
    buffer[5] = CTRL_LAND;
    buffer[6] = 0x01;

    bufferMutex.unlock();
}

void Drone::off(void)
{
    bufferMutex.lock();
    buffer[5] = CTRL_LAND;
    bufferMutex.unlock();
}

void Drone::stop(void)
{
    bufferMutex.lock();
    buffer[5] = CTRL_STOP;
    bufferMutex.unlock();
}

void Drone::launch(void)
{
    bufferMutex.lock();
    buffer[1] = 0x81;
    buffer[2] = 0x84;
    buffer[3] = 0x80;
    buffer[4] = 0x84;
    buffer[5] = CTRL_LAUNCH;
    buffer[6] = 0x00;
    bufferMutex.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    
    bufferMutex.lock();
    buffer[5] = CTRL_LAND;
    buffer[6] = 0x01;
    bufferMutex.unlock();
}

/**
 * Set all the parameters in one go
 */
void Drone::setState(char yaw, char surge, char heave, char sway)
{
    bufferMutex.lock();
    buffer[1] = yaw;
    buffer[2] = surge;
    buffer[3] = heave;
    buffer[4] = sway;

    calcChecksum();
    bufferMutex.unlock();
}

