#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <filesystem>

class BlockDevice {

public:

    BlockDevice() = delete;

    BlockDevice(const std::filesystem::path& dir);

    ~BlockDevice();

/**
 * get block number
 * @param hash
 * @return block number in m_path otherwise -1
*/
    ssize_t get_block_number(const std::string& hash);

/**
 * get block size
 * @param block_num - block number
 * @return block size otherwise -1
*/
    ssize_t get_block_size(size_t block_num);

/**
 * get block data
 * @param block_num - block number
 * @param buffer - get data to buffer
 * @param buffer_size - size to get
 * @return 0 if success otherwise -1
*/
    ssize_t get_block_data(size_t block_num, char* buffer, size_t buffer_size);
private:

    std::vector<std::filesystem::path> m_path;
};

/**
 * Use standart map as tree | Also can use B-tree
 * Key = hash
 * Value = Block
*/
typedef std::shared_ptr<BlockDevice> block_device_ptr;