#include "./includes/config/Config.hpp"

int main() {

    try {
        Config config("./server.conf");
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}