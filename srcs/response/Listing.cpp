#include "../../includes/response/Response.hpp"

static std::string create_link(std::string path_dir, const std::string &name, bool is_dir) {
    std::string last;
    if (path_dir[path_dir.size() - 1] != '/') {
        last = path_dir.substr(path_dir.find_last_of('/') + 1) + "/";
    } else {
        last = "";
    }
    std::string link = "<a href=\"";
    link += last;
    link += name;
    if (is_dir) {
        link += "/"; 
    }
    link += "\">";
    link += path_dir;
    if (last != "")
        link += "/";
    link += name;
    link += "</a>";
    return link;
}

std::string Response::generate_listing_html() {

    std::string new_path = this->_request.getInitialPath();
    std::cout << "new_path: " << new_path << std::endl;
    std::string html = "<html><body><h1>Listing : ";
    html += new_path;
    html += "</h1><ul>";


    
    DIR *dir = opendir(new_path.c_str());
    if (!dir) {
        html += "<p>Error : Cannot open the directory.</p>";
        html += "</body></html>";
        return html;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..") {
            continue;
        }
        
        bool is_dir = (entry->d_type == DT_DIR);
        
        html += "<li>";
        html += create_link(new_path, entry->d_name, is_dir);
        html += "</li>";
    }
    
    closedir(dir);
    
    html += "</ul></body></html>";
    return html;
}
