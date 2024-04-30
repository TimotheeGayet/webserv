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

    std::string new_path = dir_path.substr(0, dir_path.find_last_of('/'));
    std::string html = "<html><body><h1>Listing du r&eacute;pertoire : ";
    html += new_path;
    html += "</h1><ul>";


    
    DIR *dir = opendir(new_path.c_str());
    if (!dir) {
        html += "<p>Erreur : Impossible d'ouvrir le r&eacute;pertoire.</p>";
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
