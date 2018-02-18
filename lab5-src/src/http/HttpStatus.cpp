#include <string>

#include "http/HttpStatus.hpp"

HttpStatus::HttpStatus(enum HttpStatusCode code, char const* text) : m_code(code), m_text(text) {}

std::string HttpStatus::to_string() const noexcept
{
    return std::to_string((int) m_code) + ' ' + m_text;
}

const HttpStatus HttpStatus::Ok(OK, "OK");
const HttpStatus HttpStatus::BadRequest(BAD_REQUEST, "Bad Request");
const HttpStatus HttpStatus::Forbidden(FORBIDDEN, "Forbidden");
const HttpStatus HttpStatus::NotFound(NOT_FOUND, "Not Found");
const HttpStatus HttpStatus::MethodNotAllowed(METHOD_NOT_ALLOWED, "Method Not Allowed");
const HttpStatus HttpStatus::UnsupportedMediaType(UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type");
const HttpStatus HttpStatus::InternalServerError(INTERNAL_SERVER_ERROR, "Internal Server Error");
const HttpStatus HttpStatus::HttpVersionNotSupported(HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported");
