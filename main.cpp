#include <string>

std::string generateTestRequest() {
    return "GET /index.html HTTP/1.1\r\n"
           "Host: example.com\r\n"
           "User-Agent: FakeClient/1.0\r\n"
           "Accept: */*\r\n"
           "\r\n";
}

int main() {
    std::string request = generateTestRequest();
    // send request to server
    return 0;
}