#include "../../includes/cgi/FileData.hpp"

FILEData::FILEData() {}

FILEData::~FILEData() {}

void FILEData::setFilename(const std::string& filename) {
    _filename = filename;
}

void FILEData::setFileData(const std::string& filedata) {
    _filedata = filedata;
}

void FILEData::setFileLength(const std::string& file_length) {
    _file_length = file_length;
}

void FILEData::setContentType(const std::string& content_type) {
    _content_type = content_type;
}

const std::string& FILEData::getFilename() const {
    return _filename;
}

const std::string& FILEData::getFileData() const {
    return _filedata;
}

const std::string& FILEData::getFileLength() const {
    return _file_length;
}

const std::string& FILEData::getContentType() const {
    return _content_type;
}
