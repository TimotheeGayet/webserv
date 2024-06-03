// CgiHandler.cpp

#include "../../includes/cgi/CgiHandler.hpp"
#include <string>
#include <map>

char ** CgiHandler::makeEnv(ServerConfig server, Location location, Request &request) {
    std::map<std::string, std::string> env_map;
    HeaderRequest headers = request.getHeader();

    env_map["REDIRECT_STATUS"] = "200";
    env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
    env_map["SCRIPT_NAME"] = location.getPath();
    env_map["SCRIPT_FILENAME"] = location.getPath();
    env_map["REQUEST_METHOD"] = request.getMethod();
    env_map["CONTENT_LENGTH"] = headers.getContentLength();
    env_map["CONTENT_TYPE"] = "text/html";
    env_map["PATH_INFO"] = request.getPath();
    env_map["PATH_TRANSLATED"] = request.getPath();
    env_map["QUERY_STRING"] = request.getRequest();
    env_map["REQUEST_URI"] = request.getURI();
    env_map["SERVER_NAME"] = server.getServerName();

    uint16_t port = server.getPort();
    std::ostringstream oss;
    oss << port;
    std::string port_str = oss.str();
    env_map["SERVER_PORT"] = port_str;
    
    env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
    env_map["SERVER_SOFTWARE"] = "serveur_du_web";

    char **env = new char*[env_map.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::const_iterator it = env_map.begin(); it != env_map.end(); ++it) {
        std::string str = it->first + "=" + it->second;
        env[i] = new char[str.size() + 1];
        std::strcpy(env[i], str.c_str());
        i++;
    }
    env[i] = NULL;
    return env;
}

static void freeEnv(char **env) {
    for (int i = 0; env[i]; i++) {
        delete[] env[i];
    }
    delete[] env;
}


std::string CgiHandler::execute_cgi(const std::string& filename, const std::string& cgi_path, ServerConfig server, Location location, Request &request) {

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        throw std::runtime_error("Pipe error");
    }

    pid_t pid = fork();

    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        throw std::runtime_error("Fork error");
        return NULL;
    }
    else if (pid == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        std::string executable;

        size_t pos = filename.find_last_of('/');
        if (pos != std::string::npos) {
            executable = filename.substr(pos + 1);
        }
        else {
            throw std::runtime_error("Filename error");
        }

        char *args[] = {(char*)executable.c_str(), (char*)filename.c_str(), NULL};

        char **env;
        env = makeEnv(server, location, request);

        if (execve(cgi_path.c_str(), args, env) == -1) {
            freeEnv(env);
            throw std::runtime_error("Execve error");
        }
        freeEnv(env);
        return NULL;
    }
    else {
        close(pipefd[1]);

        std::stringstream result_stream;
        char buffer[4096];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            result_stream.write(buffer, bytes_read);
        }

        int status;
        waitpid(pid, &status, 0);
        close(pipefd[0]);

        if (WIFEXITED(status)) {
            std::cout << "[LOGS]: Child process end with code : " << WEXITSTATUS(status) << std::endl;
        }

        return result_stream.str();
    }
}