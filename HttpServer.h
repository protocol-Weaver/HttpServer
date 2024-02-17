#ifndef TCP_LINUX
#define TCP_LINUX

namespace http {
    class TcpServer
    {
        public :
            TcpServer(std::string ip_address, int port);
           ~TcpServer();
        private:
            int m_socket;
            int m_new_socket;
            int m_port;
            std::string m_ip_address;
            int m_incomingMessage;
            struct in_addr
            {
                unsigned long s_addr;
            };
            struct sockaddr_in 
            {
                short sin_family;
                unsigned short sin_port;
                struct in_addr sin_addr;
                char sin_zero[8];
            };
            struct sockaddr_in m_socketAddress;
            unsigned int m_socketAddress_len;
            std::string m_serverMessage;
            std::string buildResponse();
            int startServer();
            void startListen();
            void closeServer();
            void sendResponse();
            void acceptConnection(int &sock);
    };

};

#endif