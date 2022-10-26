#pragma once

#include <filesystem>
#include <string>
#include <initializer_list>
#include <functional>

std::string RemoveRootFromPath(const std::string& root, const std::string& path);
void CvtWindowsDelim2Unix(std::string& text);
std::string GetFilenameNoExt(const std::string& filename);

class PathWalker final {
public:
    enum Error {
        NoError,
        PathNotExists,
    };

    using VisitFunc = std::function<void(const std::filesystem::path&)>;

    PathWalker(const std::initializer_list<std::string>& filters, VisitFunc func): filters_(filters), visitFunc_(func) {}

    Error operator()(const std::filesystem::path& path);

private:
    std::vector<std::string> filters_;
    VisitFunc visitFunc_ = nullptr;

    bool isSaitisfyExtensionFilter(const std::filesystem::path&);
    void doVisitRecursive(const std::filesystem::path&);
};