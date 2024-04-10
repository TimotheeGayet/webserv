#include <string>
#include "includes/Request.hpp"
#include <iostream>

std::string generateTestRequest()
{
    return "GET https://www.boo.com/index.html HTTP/1.1\r\nHost: example.com\r User-Agent: FakeClient/1.0\r\nAccept: */*\r\n\r\n";
}

int main()
{
    std::string request = generateTestRequest();
    Request req(request);
    return 0;
}
