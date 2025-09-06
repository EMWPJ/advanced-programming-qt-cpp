#include <iostream>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;
// 批量重命名文件的工具函数
void batchRenameFiles(const std::string& dirPath, const std::string& prefix) {
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        std::cout << "目录不存在: " << dirPath << std::endl;
        return;
    }
    int count = 1;
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            std::string oldPath = entry.path().string();
            std::string extension = entry.path().extension().string();
            std::string newName = prefix + std::to_string(count) + extension;
            std::string newPath = (fs::path(dirPath) / newName).string();
            if (fs::exists(newPath)) {
                std::cout << "跳过已存在的文件: " << newPath << std::endl;
            } else {
                fs::rename(oldPath, newPath);
                std::cout << "重命名: " << oldPath << " -> " << newPath << std::endl;
            }
            ++count;
        }
    }
}
int main() {
    std::string dirPath, prefix;
    std::cout << "请输入要批量重命名的文件夹路径: ";
    std::getline(std::cin, dirPath);
    std::cout << "请输入新的文件名前缀: ";
    std::getline(std::cin, prefix);
    batchRenameFiles(dirPath, prefix);
    std::cout << "批量重命名完成。" << std::endl;
    std::cin.get();
    return 0;
}