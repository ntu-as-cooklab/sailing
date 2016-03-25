#include <tcp.hpp>

// start accepting connections
void TCPserver::start_accept()
{
	// create new session
	session = new TCPsession(io_service);
	acceptor.async_accept(session->socket, boost::bind(&TCPserver::handle_accept, this, session, boost::asio::placeholders::error));
}

// handle incoming connection
void TCPserver::handle_accept(TCPsession* session, const boost::system::error_code& error)
{
	if (!error) session->start_read(); // start session
	else delete session;
	start_accept();
}

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
		// TODO: parse buffer & call voyage function
		printf("%s", buffer);

		start_read();
	}
	else delete this; // delete session
}
