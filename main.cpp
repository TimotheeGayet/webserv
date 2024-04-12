#include "./includes/config/GlobalConfig.hpp"
#include "./includes/server/Server.hpp"
#include "./includes/Globals.hpp"
#include "./includes/utils.hpp"

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " <GlobalConfig_file>" << std::endl;
        return 1;
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGQUIT, SIG_IGN);
    std::string path(av[1]);

    try {
        GlobalConfig config(path);
        std::vector<ServerConfig> ServerConfigs = config.getServerConfigs();
        std::vector<u_int16_t> ports;

        for (std::vector<ServerConfig>::iterator it = ServerConfigs.begin(); it != ServerConfigs.end(); ++it) {
            ports.push_back(it->getPort());
        }

        // config.printGlobalConfig();

        std::cout << "\n\nStarting server...\n\n" << std::endl;
        Server serv(config, ports);

        is_running = true;
        if (serv.run() == 0) {
            std::cout << "Server stopped. Exiting..." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}