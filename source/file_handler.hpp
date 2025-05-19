#include <fstream>
#ifndef FILE_HANDLER
#define FILE_HANDLER

class FileHandler{
    public:
        static std::ifstream create_input_stream(std::string cs_file_path);
        static int get_input_stream_char(std::istream *is);
};

#endif