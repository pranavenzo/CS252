#ifndef CS252_HTTPSTATUS_H
#define CS252_HTTPSTATUS_H

#include <string>

/**
 * This class is used to represent a valid HTTP Status
**/
class HttpStatus
{
private:
    enum HttpStatusCode
    {
        OK = 200,
        BAD_REQUEST = 400,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        UNSUPPORTED_MEDIA_TYPE = 415,
        INTERNAL_SERVER_ERROR = 500,
        HTTP_VERSION_NOT_SUPPORTED = 505
    };
    
    enum HttpStatusCode m_code;
    std::string m_text;

    HttpStatus(enum HttpStatusCode code, char const* text);
public:
    static HttpStatus const Ok;
    static HttpStatus const BadRequest;
    static HttpStatus const Forbidden;
    static HttpStatus const NotFound;
    static HttpStatus const MethodNotAllowed;
    static HttpStatus const UnsupportedMediaType;
    static HttpStatus const InternalServerError;
    static HttpStatus const HttpVersionNotSupported;

    std::string to_string() const noexcept;
};

#endif
