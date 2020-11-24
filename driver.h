#include <thread>       // multithreading
#include <mutex>        // threadsafe variables
#include <cstdlib>
#include <iostream>
#include <chrono>       // time keeping
#include <sys/socket.h> // websocket
#include <sys/types.h>  // websocket types
#include <unistd.h>     // close(fd)
#include <netinet/in.h> // inet_addr()
#include <arpa/inet.h>  // inet_addr()
#include <cstring>      // memset()

#define IP_ADDR         "192.168.0.1"
#define UDP_COM_PORT    50000
#define UDP_VIDEO_PORT  40000

#define START_BYTE      0x66
#define END_BYTE        0x99

#define CTRL_PACKET_LEN 8
#define CTRL_LAND       0x00
#define CTRL_LAUNCH     0x01
#define CTRL_STOP       0x04
#define CTRL_FLIP       0x08
#define CTRL_RSTGYRO    0x80

enum _byteName{Start, Yaw, Surge, Heave, Sway, Cmd, Checksum, Stop};
enum state{Landed, Launching, Flying, Flip};

class Drone
{
    private:
        struct sockaddr_in servaddr;
        int sockfd;
        char buffer[8];
        void calcChecksum(void);
        void commLoop(void);
        std::thread commThread;
        std::mutex bufferMutex;
        char _getByte(enum _byteName byteName);
        void _setByte(enum _byteName byteName, char value);
        char droneState;

    public:
        Drone(void);
        ~Drone(void);
        void on(void);
        void off(void);
        void stop(void);
        void launch(void);
        void setState(char yaw, char surge, char heave, char sway);
        char getYaw(void) { return _getByte(Yaw); };
        char getSurge(void) { return _getByte(Surge); };
        char getHeave(void) { return _getByte(Heave); };
        char getSway(void) { return _getByte(Sway); };
        char getCmd(void) { return _getByte(Cmd); };
        void setYaw(char value) { _setByte(Yaw, value); calcChecksum(); };
        void setSurge(char value){ _setByte(Surge, value); calcChecksum(); };
        void setSway(char value){ _setByte(Sway, value); calcChecksum(); };
        void setCmd(char value){ _setByte(Cmd, value); calcChecksum(); };

};
