// DefaultErrors.cpp

#include "../../includes/config/DefaultErrors.hpp"

DefaultErrors::DefaultErrors():
    _400("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>400 Bad Request</title></head><body><h1>400 Bad Request</h1><p>Your browser sent a request that this server could not understand.</p></body></html>"),
    _401("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>401 Unauthorized</title></head><body><h1>401 Unauthorized</h1><p>You are not authorized to access this resource.</p></body></html>"),
    _403("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>403 Forbidden</title></head><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>"),
    _404("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>404 Not Found</title></head><body><h1>404 Not Found</h1><p>The requested URL was not found on this server.</p></body></html>"),
    _405("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>405 Method Not Allowed</title></head><body><h1>405 Method Not Allowed</h1><p>The method specified in the request is not allowed for the resource.</p></body></html>"),
    _406("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>406 Not Acceptable</title></head><body><h1>406 Not Acceptable</h1><p>The server cannot generate a response that is acceptable according to the request's Accept headers.</p></body></html>"),
    _408("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>408 Request Timeout</title></head><body><h1>408 Request Timeout</h1><p>The server timed out waiting for the request.</p></body></html>"),
    _410("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>410 Gone</title></head><body><h1>410 Gone</h1><p>The requested resource is no longer available and will not be available again.</p></body></html>"),
    _411("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>411 Length Required</title></head><body><h1>411 Length Required</h1><p>The request did not specify the length of its content, which is required by the requested resource.</p></body></html>"),
    _413("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>413 Payload Too Large</title></head><body><h1>413 Payload Too Large</h1><p>The request payload is too large for the server to process.</p></body></html>"),
    _414("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>414 URI Too Long</title></head><body><h1>414 URI Too Long</h1><p>The URI provided in the request is too long for the server to process.</p></body></html>"),
    _415("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>415 Unsupported Media Type</title></head><body><h1>415 Unsupported Media Type</h1><p>The server does not support the media type of the request.</p></body></html>"),
    _416("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>416 Range Not Satisfiable</title></head><body><h1>416 Range Not Satisfiable</h1><p>The requested range is not satisfiable by the server.</p></body></html>"),
    _417("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>417 Expectation Failed</title></head><body><h1>417 Expectation Failed</h1><p>The server could not meet the expectations indicated by the Expect request header.</p></body></html>"),
    _418("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>418 I'm a teapot</title></head><body><h1>418 I'm a teapot</h1><p>This server is a teapot, not a coffee maker.</p></body></html>"),
    _429("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>429 Too Many Requests</title></head><body><h1>429 Too Many Requests</h1><p>You have made too many requests to the server.</p></body></html>"),
    _500("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>500 Internal Server Error</title></head><body><h1>500 Internal Server Error</h1><p>The server encountered an unexpected condition that prevented it from fulfilling the request.</p></body></html>"),
    _501("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>501 Not Implemented</title></head><body><h1>501 Not Implemented</h1><p>The server does not support the functionality required to fulfill the request.</p></body></html>"),
    _502("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>502 Bad Gateway</title></head><body><h1>502 Bad Gateway</h1><p>The server received an invalid response from an upstream server.</p></body></html>"),
    _503("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>503 Service Unavailable</title></head><body><h1>503 Service Unavailable</h1><p>The server is currently unable to handle the request due to a temporary overload or maintenance of the server.</p></body></html>"),
    _504("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>504 Gateway Timeout</title></head><body><h1>504 Gateway Timeout</h1><p>The server did not receive a timely response from an upstream server.</p></body></html>"),
    _505("<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>505 HTTP Version Not Supported</title></head><body><h1>505 HTTP Version Not Supported</h1><p>The server does not support the HTTP protocol version used in the request.</p></body></html>") {}

DefaultErrors::~DefaultErrors() {}

std::string DefaultErrors::getError(int code) {
    switch (code) {
        case 400:
            return "400 Bad Request";
        case 401:
            return "401 Unauthorized";
        case 403:
            return "403 Forbidden";
        case 404:
            return "404 Not Found";
        case 405:
            return "405 Method Not Allowed";
        case 406:
            return "406 Not Acceptable";
        case 408:
            return "408 Request Timeout";
        case 410:
            return "410 Gone";
        case 411:
            return "411 Length Required";
        case 413:
            return "413 Payload Too Large";
        case 414:
            return "414 URI Too Long";
        case 415:
            return "415 Unsupported Media Type";
        case 416:
            return "416 Range Not Satisfiable";
        case 417:
            return "417 Expectation Failed";
        case 418:
            return "418 I'm a teapot";
        case 429:
            return "429 Too Many Requests";
        case 500:
            return "500 Internal Server Error";
        case 501:
            return "501 Not Implemented";
        case 502:
            return "502 Bad Gateway";
        case 503:
            return "503 Service Unavailable";
        case 504:
            return "504 Gateway Timeout";
        case 505:
            return "505 HTTP Version Not Supported";
        default:
            return "400 Bad Request";
    }
}

std::string DefaultErrors::getErrorPage(int code) {
    switch (code) {
        case 400:
            return this->_400;
        case 401:
            return this->_401;
        case 403:
            return this->_403;
        case 404:
            return this->_404;
        case 405:
            return this->_405;
        case 406:
            return this->_406;
        case 408:
            return this->_408;
        case 410:
            return this->_410;
        case 411:
            return this->_411;
        case 413:
            return this->_413;
        case 414:
            return this->_414;
        case 415:
            return this->_415;
        case 416:
            return this->_416;
        case 417:
            return this->_417;
        case 418:
            return this->_418;
        case 429:
            return this->_429;
        case 500:
            return this->_500;
        case 501:
            return this->_501;
        case 502:
            return this->_502;
        case 503:
            return this->_503;
        case 504:
            return this->_504;
        case 505:
            return this->_505;
        default:
            return this->_400;
    }
}