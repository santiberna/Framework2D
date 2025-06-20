#pragma once

#include <chrono>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace fileIO
{
constexpr auto DEFAULT_READ_FLAGS = std::ios::in | std::ios::binary;
constexpr auto DEFAULT_WRITE_FLAGS = std::ios::out | std::ios::trunc | std::ios::binary;

constexpr auto TEXT_READ_FLAGS = std::ios::in;
constexpr auto TEXT_WRITE_FLAGS = std::ios::out | std::ios::trunc;

using FileTime = std::chrono::time_point<std::chrono::file_clock>;

/// Open a file stream for reading. Specify 0 or std::ios::flags
std::optional<std::ifstream> OpenReadStream(const std::string& path,
    std::ios::openmode flags = DEFAULT_READ_FLAGS);

/// Open a file stream for writing. Specify 0 or std::ios::flags
std::optional<std::ofstream> OpenWriteStream(const std::string& path,
    std::ios::openmode flags = DEFAULT_WRITE_FLAGS);

/// Dumps all bytes of a stream into a vector
std::vector<std::byte> DumpStreamIntoBytes(std::istream& stream);

/// Dumps stream into a string
std::string DumpStreamIntoString(std::istream& stream);

/// Check if a file exists.
bool Exists(const std::string& path);

/// Creates a directory at the specified path, returns false if this failed
bool MakeDirectory(const std::string& path);

/// Check the last time a file was modified. Nullopt if file doesn't exist
std::optional<FileTime> GetLastModifiedTime(const std::string& path);

/// Simplifies and Transforms the path into OS preference
std::string CanonicalizePath(const std::string& path);

/// Gets the extension of the file
std::string GetExtension(const std::string& path);

/// Appends two paths
std::string PathAppend(const std::string& path1, const std::string& path2);

// Gets the base directory of a file
std::string GetDirectory(const std::string& path);
};