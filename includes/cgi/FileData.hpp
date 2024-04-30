#pragma once

#include "../Globals.hpp"

class FILEData
{
    public:
        FILEData();
        ~FILEData();

        void            setFilename(const std::string& filename);
        void            setFileData(const std::string& filedata);
        void            setFileLength(const std::string& file_length);
        void            setContentType(const std::string& content_type);

        const           std::string& getFilename() const;
        const           std::string& getFileData() const;
        const           std::string& getFileLength() const;
        const           std::string& getContentType() const;

    private:
        std::string     _filename;
        std::string     _filedata;
        std::string     _file_length;
        std::string     _content_type;
};