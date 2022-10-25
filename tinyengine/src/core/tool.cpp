#include "engine/core/tool.hpp"

namespace engine {

std::string ReadWholeFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.fail()) {
        Loge("{} don't exists", filename.c_str());
        return "";
    }
    std::string code((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    file.close();
    return code;
}

}