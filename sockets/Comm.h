#pragma once
/////////////////////////////////////////////////////////////////////
// Comm.h - message-passing communication facility                 //
// ver 1.0                                                         //
// Jim Fawcett, CSE687-OnLine Object Oriented Design, Fall 2017    //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package defines Sender and Receiver classes.
*  - Sender uses a SocketConnecter and supports connecting to multiple
*    sequential endpoints and posting messages.
*  - Receiver uses a SocketListener which returns a Socket on connection.
*  It also defines a Comm class
*  - Comm simply composes a Sender and a Receiver, exposing methods:
*    postMessage(Message) and getMessage()
*
*  Required Files:
*  ---------------
*  Comm.h, Comm.cpp,
*  Sockets.h, Sockets.cpp,
*  Message.h, Message.cpp,
*  Utilities.h, Utilities.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 03 Oct 2017
*  - first release
*/

#include "../include/Message.h"
#include "../include/Cpp11-BlockingQueue.h"
#include "../include/Sockets.h"
#include <string>
#include <thread>

using namespace Sockets;

namespace MsgPassingCommunication
{
  ///////////////////////////////////////////////////////////////////
  // Receiver class

  class Receiver
  {
  public:
    Receiver(EndPoint ep, const std::string& name = "Receiver");
    template<typename CallableObject>
    void start(CallableObject& co);
    void stop();
    Message getMessage();
    BlockingQueue<Message>* queue();
  private:
    BlockingQueue<Message> rcvQ;
    SocketListener listener;
    std::string rcvrName;
    Logger receiverLogger;
    Logger::LOG_LEVELS logLevel;
  };

  //BlockingQueue<Message> Receiver::rcvQ;

  ///////////////////////////////////////////////////////////////////
  // Sender class

  class Sender
  {
  public:
    Sender(std::string name = "Sender");
    Sender(const Sender&);
    Sender& operator=(Sender const&);
    ~Sender();
    void start();
    void stop();
    bool connect(EndPoint ep);
    void postMessage(Message msg);
  private:
    BlockingQueue<Message> sndQ;
    SocketConnecter connecter;
    std::thread sendThread;
    EndPoint lastEP;
    std::string sndrName;
    Logger senderLogger;
    Logger::LOG_LEVELS logLevel;
  };

  class Comm
  {
  public:
    Comm(EndPoint ep, std::string name = "Comm");
    void start();
    void stop();
    void postMessage(Message msg);
    Message getMessage();
    std::string name();
  private:
    Sender sndr;
    Receiver rcvr;
    std::string commName;
  };
}