#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
#include <stdexcept>
#include <netdb.h>
#include <signal.h>

#include "Utils.hpp"
#include "server/TcpConnection.hpp"
#include "server/Server.hpp"
#include "server/Request.hpp"
#include "server/Response.hpp"
#include "controller/Controller.hpp"
#include "controller/SendFileController.hpp"
#include "controller/TextController.hpp"
#include "controller/ExecScriptController.hpp"
#include "http/HttpStatus.hpp"
#include "error/RequestError.hpp"
#include "error/ResponseError.hpp"
#include "error/ControllerError.hpp"
#include "error/SocketError.hpp"
#include "error/ConnectionError.hpp"
#include "error/TodoError.hpp"
using namespace std;
Server::Server(Config const& config) : m_config(config)
{
	struct sockaddr_in serverAddress;
	memset((char*)&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	int port = m_config.port;
	serverAddress.sin_port = htons((u_short)port);

	struct protoent * ptrp = getprotobyname("tcp");//null?

	m_master = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
	if(m_master < 0) {	
		fprintf(stderr,"Error in 'socket' call\n");
		exit(1);
	}
	
	int optval = 1;
	setsockopt(m_master, SOL_SOCKET,SO_REUSEADDR,(char *) &optval,sizeof(int));

	int error = bind(m_master,(struct sockaddr*) &serverAddress,sizeof(serverAddress));
	if(error < 0) {
		fprintf(stderr,"Error in 'bind' call\n");
		exit(1);
	}
	error = listen(m_master,m_config.queue_length);
	if(error < 0) {
		fprintf(stderr,"Error in 'listen' call\n");
		exit(1);
	}
	//switch case?
	//throw TodoError("2", "Server constructor/connecting to a socket");
}

void Server::run_linear() const
{
	while (true)
	{
		std::string response;
		TcpConnection* conn = new TcpConnection(m_config, m_master);

		handle(conn);

		delete conn;
	}
}

void Server::run_thread_request() const
{
	while(true) {
		TcpConnection* conn = new TcpConnection(m_config, m_master);
		thread myThread([this,conn]() -> void {
				this->handle(conn);
				delete conn;
				});
		myThread.detach();
	}
	//throw TodoError("3", "You need to implement thread-per-request mode");
}

void Server::run_fork() const
{
	while (true)
	{
		std::string response;
		TcpConnection* conn = new TcpConnection(m_config, m_master);
		int pid = fork();
		if(pid == -1) {
			fprintf(stderr,"fork() failed\n");
			exit(1);
		}
		if(!pid) {
			handle(conn);
			delete conn;
			signal(SIGCHLD,SIG_DFL);
			exit(0);
		}
		delete conn;
		signal(SIGCHLD,SIG_IGN);
	}


	//throw TodoError("3", "You need to implement process-per-request mode");
}

void Server::run_thread_pool() const
{

	vector<thread> pool_party;
	for(size_t x = m_config.threads; x > 0; x--) {
		pool_party.push_back(thread([this]() -> void {
					run_linear();
			}));
	}
	for(auto & pool_noodle : pool_party) pool_noodle.join();



	//throw TodoError("3", "You need to implement thread-pool mode");
}

void Server::handle(TcpConnection* conn) const
{

	Controller const* controller = nullptr;

	try
	{
		// creating req will parse the incoming request
		Request req(m_config, *conn);

		// creating res as an empty response
		Response res(m_config, *conn);

		// Printing the request will be helpful to tell what our server is seeing
		req.print();

		std::string path = req.get_path();

		// This will route a request to the right controller
		// You only need to change this if you rename your controllers or add more routes
		if (path == "/hello-world")
		{
			controller = new TextController(m_config, "Hello world!\n");
		}
		else if (path.find("/script") == 0)
		{
			controller = new ExecScriptController(m_config, "/script");
		}
		else
		{
			controller = new SendFileController(m_config);
		}

		// Whatever controller we picked needs to be run with the given request and response
		controller->run(req, res);
	}
	catch (RequestError const& e)
	{
		d_warnf("Error parsing request: %s", e.what());

		Controller::send_error_response(m_config, conn, e.status, "Error while parsing request\n");
	}
	catch (ControllerError const& e)
	{
		d_warnf("Error while handling request: %s", e.what());

		Controller::send_error_response(m_config, conn, HttpStatus::InternalServerError, "Error while handling request\n");
	}
	catch (ResponseError const& e)
	{
		d_warnf("Error while creating response: %s", e.what());

		Controller::send_error_response(m_config, conn, HttpStatus::InternalServerError, "Error while handling response\n");
	}
	catch (ConnectionError const& e)
	{
		// Do not try to write a response when we catch a ConnectionError, because that will likely just throw
		d_errorf("Connection error: %s", e.what());
	}
	catch (TodoError const& e)
	{
		d_errorf("You tried to use unimplemented functionality: %s", e.what());
	}

	// Dont forget about freeing memory!
	delete controller;
}

Server::~Server() noexcept
{
	if (close(m_master) == -1)
	{
		d_error("Could not close master socket");
	}
}
