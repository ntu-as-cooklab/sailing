#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <voyage.hpp>

// For multithreading
// http://stackoverflow.com/questions/6104200/boost-threads-with-clr
namespace boost {
    namespace detail {
        namespace win32 {
            struct _SECURITY_ATTRIBUTES: public ::_SECURITY_ATTRIBUTES {};
        }
    }
}

class TCPsession
{
public:
	boost::asio::ip::tcp::socket socket;

	// TCP session constructor
	TCPsession(boost::asio::io_service& io_service) :
		socket(io_service)
		{}

	// wait for incoming data
	void start_read();

	static Voyage* voyage;

private:

	int buffer_len = 1;
	char* buffer = new char[buffer_len];

	// handle incoming data
	void handle_read(const boost::system::error_code& error);

};

class TCPserver
{
public:
	// TCP server constructor
  	TCPserver(int PORT) :
		acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT))
		{}

	// run TCPserver thread
	void operator()()
	{
		try
		{
			printf("Server starting\n");
			start_accept();
			io_service.run();
		}
		catch (std::exception& e) { std::cerr << "Server exception: " << e.what() << "\n"; }
	}

	// stop TCPserver
	void stop() { io_service.stop(); }

private:
	// start accepting connections
  	void start_accept();
	// handle incoming connection
	void handle_accept(TCPsession* session, const boost::system::error_code& error);

	boost::asio::io_service 		io_service;
	boost::asio::ip::tcp::acceptor 	acceptor;
	TCPsession* 					session;
};

#endif
