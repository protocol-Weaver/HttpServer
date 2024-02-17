#include <iostream>
#include "HttpServer.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>


namespace
{
    const int BUFFER_SIZE = 30720;

    // Prints  To Console
    void log(std::string message)
    {
        std::cout << message;
    }

    // Exits The Program and Prints Error To Console
    void exitWithError(std::string message)
    {
        log("ERROR" + message);
        exit(1);
    }
};

namespace http
{
    TcpServer::TcpServer(std::string ip_address, int port)
    : m_ip_address(ip_address), m_socket(), m_new_socket(), m_port(port), m_incomingMessage(),
    m_socketAddress(), m_socketAddress_len(sizeof(m_socketAddress)), m_serverMessage(buildResponse())
    {
        m_socketAddress.sin_family = AF_INET;
        m_socketAddress.sin_port = htons(m_port);
        m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
        startServer();
    }

    // Starts The Server by creating Socket Structure and Listening on PORT 
    int TcpServer::startServer()
    {
        m_socket = socket(AF_INET,SOCK_STREAM,0);
        if(m_socket < 0)
        {
            exitWithError("Couldn't Create Socket");
            return 1;
        }

        if (bind(m_socket,(sockaddr *)&m_socketAddress, m_socketAddress_len) < 0)
        {
            exitWithError("Cannot connect socket to address");
            return 1;
        }

        startListen();
        
        return 0;
    }
    
    TcpServer::~TcpServer()
    {
        closeServer();   
    }

    // Closes The Server
    void TcpServer::closeServer()
    {
        close(m_socket);
        close(m_new_socket);
        exit(0);
    }

    // Listens on a Port for Socket Connection
    void TcpServer::startListen()
    {
        // Error Handling
        if(listen(m_socket, 20) < 0)
        {
            exitWithError("Socket listen failed");
        }

        // Console Message
        std::ostringstream ss;
        ss << "\n*** Listening on PORT: " << ntohs(m_socketAddress.sin_port) << " ***\n\n";
        log(ss.str());

        int bytesReceived;

        // Server Response Sending and Receiving Structure
        while(true)
        {
            log("====Waiting For New Connection====\n\n");
            acceptConnection(m_socket);
            
            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);

            if(bytesReceived < 0)
            {
                exitWithError("Failed to read bytes from client socket connection");
            }

            std::ostringstream ss;
            ss << "------ Received Request from client ------\n\n";
            log(ss.str());

            sendResponse();
            close(m_new_socket);
        }
    }

    // Accepts Connection From A Socket
    void TcpServer::acceptConnection(int &sock)
    {
        // Initialize The New Socket
        m_new_socket = accept(m_socket, (sockaddr*) &m_socketAddress, &m_socketAddress_len);
        
        // Error Handling
        if(m_new_socket < 0)
        {
            std::ostringstream ss;
            ss << 
            "Server failed to accept incoming connection from ADDRESS: " 
            << m_socketAddress.sin_addr.s_addr << "; PORT: " 
            << ntohs(m_socketAddress.sin_port);
            exitWithError(ss.str());
        }
    }
    

    // Used To initialize Response for Socket
    std::string TcpServer::buildResponse()
    {
        // HTML file that will be Displayed when Response Received
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n" << htmlFile;

        return ss.str();
    }

    // Sends Response To the Socket
    void TcpServer::sendResponse()
    {
        // Bytes Variables
        int bytesSent;
        long totalBytesSent = 0;

        // Sending The Bytes of Buffer To Socket 
        while (totalBytesSent < m_serverMessage.size())
        {
            bytesSent = send(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size(), 0);
            if (bytesSent < 0)
            {
                break;
            }
            totalBytesSent += bytesSent;
        }

        if (totalBytesSent == m_serverMessage.size())
        {
            log("------ Server Response sent to client ------\n\n");
        }
        else
        {
            log("Error sending response to client.");
        }
    }
    
};