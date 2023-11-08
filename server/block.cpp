#include "block.h"

#include <filesystem>
#include <fstream> 
#include <iostream>
#include <exception>

static const size_t SHA1_SIZE = 40;

BlockDevice::BlockDevice(const std::filesystem::path& dir) {
    if (!std::filesystem::exists(dir)) {
        throw std::runtime_error("BlockDevice::BlockDevice path does't exist!");
    }

    if (!std::filesystem::is_directory(dir)) {
        throw std::runtime_error("BlockDevice::BlockDevice !is_directory!");
    }
    /*
    Dont care if it's not a block file. Use simple file as device as an example
    if (!std::filesystem::is_block_file(path)) {
        throw std::runtime_error("BlockDevice::BlockDevice !is_block_file!");
    }
    */

   for (const auto& p : std::filesystem::directory_iterator(dir)) {
        if (p.is_regular_file()) {
            if (p.path().filename().string().size() == SHA1_SIZE) {
                std::cout << p << " " << p.file_size() << std::endl;
                m_path.push_back(p);
            }
        }
   }

}

BlockDevice::~BlockDevice() {

}

ssize_t BlockDevice::get_block_number(const std::string& hash) {
    std::cout << "BlockDevice::get_block_number() " << hash << std::endl;
    for (size_t i = 0; i < m_path.size(); i++) {
        if (m_path[i].filename() == hash) {
            return i;
        }
    }

    return -1;
}

ssize_t BlockDevice::get_block_size(size_t block_num) {
    std::cout << "BlockDevice::get_block_size() " << block_num << std::endl;
    if (block_num > m_path.size()) {
        return -1;
    }
    
    return std::filesystem::file_size(m_path[block_num]);
}

ssize_t BlockDevice::get_block_data(size_t block_num, char* buffer, size_t buffer_size) {
    std::cout << "BlockDevice::get_block_data() " << block_num << " " << buffer_size << std::endl;

    if (block_num > m_path.size() || buffer_size == 0) {
        return -1;
    }
    std::ifstream ifs(m_path[block_num], std::ifstream::binary);
    ifs.read(buffer, buffer_size);
    ifs.close();

    return 0;
}