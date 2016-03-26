#ifndef TCP_HPP_INCLUDED
#define TCP_HPP_INCLUDED

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

// For multithreading
// http://stackoverflow.com/questions/6104200/boost-threads-with-clr
namespace boost {
    namespace detail {
        namespace win32 {
            struct _SECURITY_ATTRIBUTES: public ::_SECURITY_ATTRIBUTES {};
        }
    }
}

// Base TCPsession --> inherit for implementation
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

	virtual ~TCPsession() {};

protected:

	int buffer_len = 1;
	char* buffer = new char[buffer_len];

	// handle incoming data
	virtual void handle_read(const boost::system::error_code& error);

};

template <class Session>
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
			std::cout << "Server starting\n";
			start_accept();
			io_service.run();
		}
		catch (std::exception& e) { std::cerr << "Server exception: " << e.what() << "\n"; }
	}

	// stop TCPserver
	void stop() { io_service.stop(); }

private:
	// start accepting connections
  	void start_accept()
	{
		// create new session
		session = new Session(io_service);
		acceptor.async_accept(session->socket, boost::bind(&TCPserver::handle_accept, this, session, boost::asio::placeholders::error));
	}

	// handle incoming connection
	void handle_accept(Session* session, const boost::system::error_code& error)
	{
		if (!error) session->start_read(); // start session
		else delete session;
		start_accept();
	}

	boost::asio::io_service 		io_service;
	boost::asio::ip::tcp::acceptor 	acceptor;
	Session* 						session;
};

#endif
