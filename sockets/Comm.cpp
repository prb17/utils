/////////////////////////////////////////////////////////////////////
// Comm.h - message-passing communication facility                 //
// ver 1.0                                                         //
// Jim Fawcett, CSE687-OnLine Object Oriented Design, Fall 2017    //
/////////////////////////////////////////////////////////////////////

#include "../include/Comm.h"
#include "../include/Logger.h"
#include "../include/Utilities.h"
#include <iostream>
#include <functional>
#include <conio.h>

#pragma warning(disable:4522)
#pragma comment(lib, "Ws2_32.lib")

using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;

//----< constructor sets port >--------------------------------------

Receiver::Receiver(EndPoint ep, const std::string& name) : listener(ep.port), rcvrName(name)
{
    receiverLogger = Logger();
    receiverLogger.set_prefix("Receiver: ");
    logLevel = Logger::LOG_LEVELS::HIGH;
    receiverLogger.log(logLevel, " -- starting Receiver");
}
//----< returns reference to receive queue >-------------------------

BlockingQueue<Message>* Receiver::queue()
{
  return &rcvQ;
}
//----< starts listener thread running callable object >-------------

template<typename CallableObject>
void Receiver::start(CallableObject& co)
{
  listener.start(co);
}
//----< stops listener thread >--------------------------------------

void Receiver::stop()
{
  listener.stop();
}
//----< retrieves received message >---------------------------------

Message Receiver::getMessage()
{
  return rcvQ.deQ();
  receiverLogger.log(logLevel, " -- " + rcvrName + " deQing message");
}
//----< constructor initializes endpoint object >--------------------

Sender::Sender(std::string name) : sndrName(name)
{
    senderLogger = Logger();
    senderLogger.set_prefix("Sender: ");
    logLevel = Logger::LOG_LEVELS::HIGH;
    lastEP = EndPoint();  // used to detect change in destination
}
MsgPassingCommunication::Sender::Sender(const Sender&)
{

}

Sender& MsgPassingCommunication::Sender::operator=(Sender const& other)
{
    // TODO: insert return statement here
    connecter = other.connecter;
    lastEP = other.lastEP;
    sndrName = other.sndrName;
    senderLogger = other.senderLogger;
    logLevel = other.logLevel;
    return *this;
}
//----< destructor waits for send thread to terminate >--------------

Sender::~Sender()
{
    if (sendThread.joinable()) {
        Message msg = Message();
        msg.setMsgBody("quit");
        sndQ.enQ(msg);
        sendThread.join();
    }

    
}
//----< starts send thread deQ, inspect, and send loop >-------------

void Sender::start()
{
  std::function <void()> threadProc = [&]() {
    while (true)
    {
      Message msg = sndQ.deQ();

      if (msg.getMsgBody() == "quit")
      {
          senderLogger.log(logLevel, " -- send thread shutting down");
        return;
      }
      senderLogger.log(logLevel, " -- " + sndrName + " send thread sending " + msg.getName());
      std::string msgStr = msg.toString();

      if (msg.getDestination().address != lastEP.address || msg.getDestination().port != lastEP.port)
      {
        connecter.shutDown();
        //connecter.close();
        senderLogger.log(logLevel, " -- " + sndrName + " attempting to connect to new endpoint: " + msg.getDestination().toString());
        if (!connect(msg.getDestination()))
        {
            senderLogger.log(logLevel, sndrName + " can't connect");
          continue;
        }
        else
        {
            senderLogger.log(logLevel, sndrName + " connected to " + msg.getDestination().toString());
        }
      }
      bool sendRslt = connecter.send(msgStr.length(), (Socket::byte*)msgStr.c_str());
    }
  };
  std::thread t(threadProc);
  sendThread = std::move(t);
}
//----< stops send thread by posting quit message >------------------

void Sender::stop()
{
  Message msg;
  msg.setName("quit");
  msg.setMsgBody("quit");
  postMessage(msg);
  connecter.shutDown();
}
//----< attempts to connect to endpoint ep >-------------------------

bool Sender::connect(EndPoint ep)
{
  lastEP = ep;
  return connecter.connect(ep.address, ep.port);
}
//----< posts message to send queue >--------------------------------

void Sender::postMessage(Message msg)
{
  sndQ.enQ(msg);
}

//----< callable object posts incoming message to rcvQ >-------------
/*
*  This is ClientHandler for receiving messages and posting
*  to the receive queue.
*/
class ClientHandler
{
public:
  //----< acquire reference to shared rcvQ >-------------------------

  ClientHandler(BlockingQueue<Message>* pQ, const std::string& name = "clientHandler") : pQ_(pQ), clientHandlerName(name)
  {
      clientHandlerLogger = Logger();
      clientHandlerLogger.set_prefix("ClientHandler: ");
      logLevel = Logger::LOG_LEVELS::HIGH;
      clientHandlerLogger.log(logLevel, " -- starting ClientHandler");
  }
  //----< shutdown message >-----------------------------------------

  ~ClientHandler() 
  { 
      clientHandlerLogger.log(logLevel, " -- ClientHandler destroyed;");
  }
  //----< set BlockingQueue >----------------------------------------

  void setQueue(BlockingQueue<Message>* pQ)
  {
    pQ_ = pQ;
  }
  //----< frame message string by reading bytes from socket >--------

  std::string readMsg(Socket& socket)
  {
    std::string temp, msgString;
    while (socket.validState())
    {
      temp = socket.recvString('\n');  // read attribute
      msgString += temp;
      if (temp.length() < 2)           // if empty line we are done
        break;
    }
    return msgString;
  }
  //----< reads messages from socket and enQs in rcvQ >--------------

  void operator()(Socket socket)
  {
    while (socket.validState())
    {
      std::string msgString = readMsg(socket);
      if (msgString.length() == 0)
      {
        // invalid message
        break;
      }
      Message msg = Message::fromString(msgString);
      clientHandlerLogger.log(logLevel, " -- " + clientHandlerName + " RecvThread read message: " + msg.getName());
      pQ_->enQ(msg);
      if (msg.getMsgBody() == "quit")
        break;
    }
    clientHandlerLogger.log(logLevel, " -- terminating ClientHandler thread");
  }
private:
  BlockingQueue<Message>* pQ_;
  std::string clientHandlerName;
  Logger clientHandlerLogger;
  Logger::LOG_LEVELS logLevel;
};

Comm::Comm(EndPoint ep, std::string name) : rcvr(ep, name), sndr(name), commName(name) {}

void Comm::start()
{
  BlockingQueue<Message>* pQ = rcvr.queue();
  ClientHandler* pCh = new ClientHandler(pQ, commName);
  /*
    There is a trivial memory leak here.  
    This ClientHandler is a prototype used to make ClientHandler copies for each connection.
    Those are not created on the heap, and are destroyed when the connection closes.
    Only one Client handler prototype is created for each Comm object and will live until
    the end of the program.

    I will clean this up in the next version.
  */
  rcvr.start(*pCh);
  sndr.start();
}

void Comm::stop()
{
  rcvr.stop();
  sndr.stop();
}

void Comm::postMessage(Message msg)
{
  sndr.postMessage(msg);
}

Message Comm::getMessage()
{
  return rcvr.getMessage();
}

std::string Comm::name()
{
  return commName;
}

//----< test stub >--------------------------------------------------

#ifdef TEST_COMM

/////////////////////////////////////////////////////////////////////
// Test #1 - Demonstrates Sender and Receiver operations

void DemoSndrRcvr(const std::string& machineName)
{
  SUtils::title("Demonstrating Sender and Receiver classes");

  SocketSystem ss;
  EndPoint ep1;
  ep1.port = 9091;
  ep1.address = "localhost";
  Receiver rcvr1(ep1);
  BlockingQueue<Message>* pQ1 = rcvr1.queue();

  ClientHandler ch1(pQ1);
  rcvr1.start(ch1);

  EndPoint ep2;
  ep2.port = 9092;
  ep2.address = "localhost";
  Receiver rcvr2(ep2);
  BlockingQueue<Message>* pQ2 = rcvr2.queue();

  ClientHandler ch2(pQ2);
  rcvr2.start(ch2);

  Sender sndr;
  sndr.start();
  bool connected = sndr.connect(ep1);
  Message msg;
  msg.name("msg #1");
  msg.to(ep1);
  msg.from(msg.to());
  msg.command("do it");
  msg.attribute("bodyAttrib", "zzz");
  StaticLogger<1>::flush();
  std::cout << "\n  sndr in main posting message:  " << msg.name();
  sndr.postMessage(msg);

  msg.name("msg #2");
  msg.to(EndPoint(machineName, 9092));
  StaticLogger<1>::flush();
  std::cout << "\n  sndr in main posting message:  " << msg.name();
  sndr.postMessage(msg);

  Message rcvdMsg = rcvr1.getMessage();  // blocks until message arrives
  StaticLogger<1>::flush();
  std::cout << "\n  rcvr1 in main received message: " << rcvdMsg.name();
  rcvdMsg.show();

  rcvdMsg = rcvr2.getMessage();  // blocks until message arrives
  StaticLogger<1>::flush();
  std::cout << "\n  rcvr2 in main received message: " << rcvdMsg.name();
  rcvdMsg.show();

  SUtils::title("Sending message to EndPoint that doesn't exist");

  msg.name("msg #3");
  msg.to(EndPoint("DoesNotExist", 1111));  // Unknown endpoint - should fail
  StaticLogger<1>::flush();
  std::cout << "\n  sndr in main posting message:  " << msg.name();
  msg.show();
  sndr.postMessage(msg);                   // will never reach rcvr

  msg.name("msg #4");
  msg.to(EndPoint("localhost", 9091));
  StaticLogger<1>::flush();
  std::cout << "\n  sndr in main posting message:  " << msg.name();
  sndr.postMessage(msg);                  // this should succeed
  StaticLogger<1>::flush();
  rcvdMsg = rcvr1.getMessage();
  std::cout << "\n  rcvr1 in main received message: " << rcvdMsg.name();
  rcvdMsg.show();

  rcvr1.stop();
  rcvr2.stop();
  sndr.stop();
  StaticLogger<1>::flush();

  std::cout << "\n  press enter to quit DemoSndrRcvr";
  _getche();
  std::cout << "\n";
}

/////////////////////////////////////////////////////////////////////
// Test #2 - Demonstrates Comm class using a single thread
//           sending and receiving messages from two Comm
//           instances.

void DemoCommClass(const std::string& machineName)
{
  SUtils::title("Demonstrating Comm class");

  SocketSystem ss;

  EndPoint ep1("localhost", 9191);
  Comm comm1(ep1, "comm1");
  comm1.start();

  EndPoint ep2("localhost", 9192);
  Comm comm2(ep2, "comm2");
  comm2.start();

  // send msg from comm1 to comm1
  Message msg;
  msg.name("msg #1");
  msg.to(ep1);
  msg.from(ep1);
  StaticLogger<1>::flush();
  std::cout << "\n  comm1 in main posting message:   " << msg.name();
  comm1.postMessage(msg);
  msg = comm1.getMessage();
  StaticLogger<1>::flush();
  std::cout << "\n  comm1 in main received message:  " << msg.name();
  msg.show();

  // send msg from comm1 to comm2
  msg.name("msg #2");
  msg.from(ep1);
  msg.to(ep2);
  StaticLogger<1>::flush();
  std::cout << "\n  comm1 in main posting message:  " << msg.name();
  comm1.postMessage(msg);
  msg = comm2.getMessage();
  StaticLogger<1>::flush();
  std::cout << "\n  comm2 in main received message: " << msg.name();
  msg.show();

  // send msg from comm2 to comm1
  msg.name("msg #3");
  msg.to(ep1);
  msg.from(ep2);
  StaticLogger<1>::flush();
  std::cout << "\n  comm2 in main posting message:  " << msg.name();
  comm2.postMessage(msg);
  msg = comm1.getMessage();
  StaticLogger<1>::flush();
  std::cout << "\n  comm1 in main received message: " << msg.name();
  msg.show();

  // send msg from comm2 to comm2
  msg.name("msg #4");
  msg.from(ep2);
  msg.to(ep2);
  StaticLogger<1>::flush();
  std::cout << "\n  comm2 in main posting message:  " << msg.name();
  comm2.postMessage(msg);
  msg = comm2.getMessage();
  StaticLogger<1>::flush();
  std::cout << "\n  comm2 in main received message: " << msg.name();
  msg.show();

  comm1.stop();
  comm2.stop();
  StaticLogger<1>::flush();
  std::cout << "\n  press enter to quit DemoComm";
  _getche();
}
/////////////////////////////////////////////////////////////////////
// Test #3 - Demonstrate server with two concurrent clients
//           sending and receiving messages

//----< handler for first concurrent client >------------------------

void ThreadProcClnt1()
{
  Comm comm(EndPoint("localhost", 9891), "client1Comm");
  comm.start();
  EndPoint serverEP("localhost", 9890);
  EndPoint clientEP("localhost", 9891);
  size_t IMax = 3;
  for (size_t i = 0; i < IMax; ++i)
  {
    Message msg(serverEP, clientEP);
    msg.name("client #1 : msg #" + Utilities::Converter<size_t>::toString(i));
    std::cout << "\n  " + comm.name() + " posting:  " << msg.name();
    comm.postMessage(msg);
    Message rply = comm.getMessage();
    std::cout << "\n  " + comm.name() + " received: " << rply.name();
    ::Sleep(100);
  }
  ::Sleep(200);
  Message stop;
  stop.name("stop");
  stop.to(serverEP);
  stop.command("stop");
  comm.postMessage(stop);
}
//----< handler for 2nd concurrent client >--------------------------

void ThreadProcClnt2()
{
  Comm comm(EndPoint("localhost", 9892), "client2Comm");
  comm.start();
  EndPoint serverEP("localhost", 9890);
  EndPoint clientEP("localhost", 9892);
  size_t IMax = 3;
  for (size_t i = 0; i < IMax; ++i)
  {
    Message msg(serverEP, clientEP);
    msg.name("client #2 : msg #" + Utilities::Converter<size_t>::toString(i));
    std::cout << "\n  " + comm.name() + " posting:  " << msg.name();
    comm.postMessage(msg);
    Message rply = comm.getMessage();
    std::cout << "\n  " + comm.name() + " received: " << rply.name();
  }
}
//----< server demonstrates two-way asynchronous communication >-----
/*
*  - One server receiving messages and sending replies to
*    two concurrent clients.
*/
void DemoClientServer()
{
  SUtils::title("Demonstrating Client-Server - one server with two concurrent clients");

  SocketSystem ss;

  EndPoint serverEP("localhost", 9890);
  EndPoint clientEP("localhost", 9891);
  Comm comm(serverEP, "serverComm");
  comm.start();
  std::thread t1(ThreadProcClnt1);
  t1.detach();
  std::thread t2(ThreadProcClnt2);
  t2.detach();

  Message msg, rply;
  rply.name("reply");
  size_t count = 0;
  while (true)
  {
    msg = comm.getMessage();
    std::cout << "\n  " + comm.name() + " received message: " << msg.name();
    //msg.show();
    rply.to(msg.from());
    rply.from(serverEP);
    rply.name("server reply #" + Utilities::Converter<size_t>::toString(++count) + " to " + msg.from().toString());
    //rply.show();
    comm.postMessage(rply);
    if (msg.command() == "stop")
    {
      break;
    }
  }
  comm.stop();
  StaticLogger<1>::flush();
  std::cout << "\n  press enter to quit DemoClientServer";
  _getche();
}

Cosmetic cosmetic;

int main()
{
  SUtils::Title("Demo of Message-Passing Communication");
  Utilities::putline();

  StaticLogger<1>::attach(&std::cout);

  ///////////////////////////////////////////////////////////////////
  // remove comment on line below to show many of the gory details
  //
  //StaticLogger<1>::start();

  ///////////////////////////////////////////////////////////////////
  // if you uncomment the lines below, you will run all demos

  //DemoSndrRcvr("Odin");  // replace "Odin" with your machine name
  //DemoCommClass("Odin");
  DemoClientServer();

  return 0;
}
#endif
