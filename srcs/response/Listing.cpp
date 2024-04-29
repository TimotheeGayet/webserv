#include "../../includes/response/Response.hpp"

static std::string create_link(const std::string &name, bool is_dir) {
    std::string link = "<a href=\"";
    link += name;
    if (is_dir) {
        link += "/\""; 
    } else {
        link += "\"";
    }
    link += ">";
    link += name;
    link += "</a>";
    return link;
}

std::string Response::generate_listing_html(const std::string &dir_path) {
    std::string html = "<html><body><h1>Listing du répertoire : ";
    html += dir_path;
    html += "</h1><ul>";
    
    DIR *dir = opendir(dir_path.c_str());
    if (!dir) {
        html += "<p>Erreur : Impossible d'ouvrir le répertoire.</p>";
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
        html += create_link(entry->d_name, is_dir);
        html += "</li>";
    }
    
    closedir(dir);
    
    html += "</ul></body></html>";
    return html;
}
