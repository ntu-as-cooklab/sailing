#include <tcp_server.hpp>

// wait for incoming data
void TCPsession::start_read()
{
	socket.async_read_some
	(
		boost::asio::buffer(buffer, buffer_len * sizeof(char)),
		boost::bind(&TCPsession::handle_read, this, boost::asio::placeholders::error)
	);
}

// handle incoming data
void TCPsession::handle_read(const boost::system::error_code& error)
{
	if (!error)
	{
		// simple echo
		printf("%s", buffer);

		start_read();
	}
	else delete this; // delete session
}
