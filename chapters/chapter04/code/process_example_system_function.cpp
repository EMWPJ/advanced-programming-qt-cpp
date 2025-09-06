#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

// 过程式编程：系统编程示例——统计指定目录下的文件数量和总大小
int main() {
    std::string dirPath;
    std::cout << "请输入要统计的文件夹路径: ";
    std::getline(std::cin, dirPath);

    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        std::cout << "目录不存在: " << dirPath << std::endl;
        return 1;
    }

    size_t fileCount = 0;
    uintmax_t totalSize = 0;

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            ++fileCount;
            totalSize += entry.file_size();
        }
    }

    std::cout << "文件夹: " << dirPath << std::endl;
    std::cout << "文件数量: " << fileCount << std::endl;
    std::cout << "总大小: " << totalSize << " 字节" << std::endl;

    std::cin.get();
    return 0;
}
