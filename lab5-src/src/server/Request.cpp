#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <sstream>

#include "server/Request.hpp"
#include "http/HttpStatus.hpp"
#include "server/TcpConnection.hpp"
#include "Config.hpp"
#include "Utils.hpp"
#include "error/RequestError.hpp"
#include "error/ConnectionError.hpp"
#include "error/TodoError.hpp"
using namespace std;

Request::Request(Config const& config, TcpConnection& conn) :
	m_config(config),
	m_conn(conn)
{
	std::string request_line = parse_raw_line();
	fprintf(stdout,"This is the line [%s]\n",request_line.c_str());
	parse_method(request_line);
	fprintf(stdout,"This is the line [%s]\n",request_line.c_str());
	parse_route(request_line);
	fprintf(stdout,"This is the line [%s]\n",request_line.c_str());
	parse_version(request_line);
	fprintf(stdout,"This is the line [%s]\n",request_line.c_str());

	// the previous three parse_* calls should consume the entire line
	if (!request_line.empty())
	{
		throw RequestError(HttpStatus::BadRequest, "Malformed request-line\n");
	}

	parse_headers();
	parse_body();
}

void Request::parse_method(std::string& raw_line)
{
	if(raw_line.size() == 0 || raw_line.find("GET") == string::npos) {
		//	fprintf(stderr,"400 BAD REQUEST \n");
		throw RequestError(HttpStatus::MethodNotAllowed,"");
		return;
	}
	string getString("GET");
	for(size_t i = 0; i < getString.size();i++) {
		if(raw_line[0] == getString[i]) {
			m_method = m_method + raw_line[0];
			raw_line = raw_line.substr(1);
		} else {
			fprintf(stderr,"400 BAD REQUEST \n");
			return;
		}
	}
	//throw TodoError("2", "You have to implement parsing methods");
}

void Request::parse_route(std::string& raw_line)
{
	size_t index = raw_line.find("/");
	size_t space = raw_line.substr(1).find(" ");
	if(index > space) {
		//    	fprintf(stderr,"400 BAD REQUEST \n");
		throw RequestError(HttpStatus::BadRequest,"");
		return;	
	}

	for(size_t i = index;;i++) {
		if(raw_line[0] != ' ')
			m_path = m_path + raw_line[0];
		raw_line = raw_line.substr(1);
		if(raw_line[0] == ' ') return;
	}

	throw RequestError(HttpStatus::BadRequest,"");
	//throw TodoError("2", "You have to implement parsing routes");

}

void Request::parse_querystring(std::string query, std::unordered_map<std::string, std::string>& parsed)
{
	throw TodoError("6", "You have to implement parsing querystrings");
}

void Request::parse_version(std::string& raw_line)
{
	if(raw_line[0] == ' ') raw_line = raw_line.substr(1);
	string version("HTTP/1.");
	for(size_t i = 0; i < version.size(); i++) {
		if(version[i] != raw_line[0]) {
			//	fprintf(stderr,"505 HTTP Version not supported \n");
			throw RequestError(HttpStatus::HttpVersionNotSupported,"");
			return;	
		}
		m_version = m_version + version[i];
		raw_line = raw_line.substr(1);
	}
	if(raw_line[0] != '1' && raw_line[0] != '0') {
		throw RequestError(HttpStatus::HttpVersionNotSupported,"");
		return;
	}
	m_version = m_version + raw_line[0];
	raw_line = raw_line.substr(1);
	if(raw_line.size() == 2)
		raw_line = "";

	//throw TodoError("2", "You have to implement parsing HTTP version");
}

void Request::parse_headers()
{
	string line;
	string header;
	while(true) {
		line = parse_raw_line();
		if(line.size() <= 2) break;
		size_t index = line.find(":");
		if(index == string::npos) {
			fprintf(stderr,"Faulty line%sn\n",line.c_str());
			exit(1);
		}
		string header = line.substr(0,index);
		line = line.substr(index + 2);
		m_headers[header] = line;
	}
	//throw TodoError("2", "You have to implement parsing headers");
}

void Request::parse_body()
{
	if (m_method == "GET") return;

	throw TodoError("6", "You have to implement parsing request bodies");
}

std::string Request::parse_raw_line()
{
	string line;
	unsigned char req;
	while(m_conn.getc(&req)) {
		//	fprintf(stdout,"%c",req[0]);
		stringstream ss;
		ss << req;
		string temp(ss.str());
		line = line + temp;
		if(req == '\0' || line.find("\r\n") != string::npos) break;
	}
	fprintf(stdout,"bef: [%s]\n",line.c_str());
	line = line.substr(0,line.size() - 2);
	fprintf(stdout,"[%s]\n",line.c_str());
	return line;
	//throw TodoError("2", "You need to implement line fetching");
}

void Request::print() const noexcept
{
	std::cout << m_method << ' ' << m_path << ' ' << m_version << std::endl;
#ifdef DEBUG    
	for (auto const& el : m_headers)
	{
		std::cout << el.first << ": " << el.second << std::endl;
	}

	for (auto const& el : m_query)
	{
		std::cerr << el.first << ": " << el.second << std::endl;
	}

	for (auto const& el : m_body_data)
	{
		std::cerr << el.first << ": " << el.second << std::endl;
	}
#endif	
}

bool Request::try_header(std::string const& key, std::string& value) const noexcept
{
	if (m_headers.find(key) == m_headers.end())
	{
		return false;
	}
	else
	{
		value = m_headers.at(key);
		return true;
	}
}

std::string const& Request::get_path() const noexcept
{
	return m_path;
}

std::string const& Request::get_method() const noexcept
{
	return m_method;
}

std::string const& Request::get_version() const noexcept
{
	return m_version;
}

std::unordered_map<std::string, std::string> const& Request::get_headers() const noexcept
{
	return m_headers;
}

std::unordered_map<std::string, std::string> const& Request::get_query() const noexcept
{
	return m_query;
}

std::unordered_map<std::string, std::string> const& Request::get_body() const noexcept
{
	return m_body_data;
}
