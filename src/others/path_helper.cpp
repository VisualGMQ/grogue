#include "path_helper.hpp"

std::string RemoveRootFromPath(const std::string& root, const std::string& path) {
    int i = 0;
    for (; i < root.size(); i++) {
        char c1 = root[i];
        if (c1 == '\\') c1 = '/';
        char c2 = path[i];
        if (c2 == '\\') c2 = '/';
        if (c1 != c2) {
            break;
        }
    }
    return path.substr(i + 1);
}

void CvtWindowsDelim2Unix(std::string& text) {
    for (auto& c : text) {
        if (c == '\\')
            c = '/';
    }
}

std::string GetFilenameNoExt(const std::string& filename) {
    auto idx = filename.find_last_of(".");
    if (idx == filename.npos) return filename;
    return std::string(filename.data(), idx);
}