#include "./includes/config/Config.hpp"
#include "./includes/server/Server.hpp"
#include "./includes/Globals.hpp"

int main(int ac, char **av) {

    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " <config_file>" << std::endl;
        return 1;
    }

    std::string path(av[1]);

    try {
        Config config(path);
        std::vector<ServerConfig> serverConfigs = config.getServerConfigs();
        std::vector<u_int16_t> ports;

        for (std::vector<ServerConfig>::iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it) {
            ports.push_back(it->getPort());
        }

        // config.printConfig();

        std::cout << "\n\nStarting server...\n\n" << std::endl;
        Server serv(config, ports);

        if (serv.run() == 0) {
            std::cout << "Server stopped. Exiting..." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}