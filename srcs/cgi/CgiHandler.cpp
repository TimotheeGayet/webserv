// CgiHandler.cpp

#include "../../includes/cgi/CgiHandler.hpp"
#include <string>
#include <map>

std::string CgiHandler::execute_cgi(const std::string& filename) {

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

        char *args[] = {(char*)"php-cgi", (char*)filename.c_str(), NULL};

        if (execve("./ubuntu_cgi_tester", args, NULL) == -1) {
            throw std::runtime_error("Execve error");
        }
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