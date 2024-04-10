#include "./includes/config/Config.hpp"

int main() {

    try {
        Config config("./server.conf");

        config.printConfig();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}