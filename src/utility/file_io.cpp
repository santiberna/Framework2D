#include <bit>
#include <utility/file_io.hpp>
#include <filesystem>

std::optional<std::ifstream> fileIO::OpenReadStream(const std::string& path,
    std::ios::openmode flags)
{
    std::ifstream stream(path, flags);
    if (stream)
    {
        return stream;
    }
    else
    {
        return std::nullopt;
    }
}

std::optional<std::ofstream> fileIO::OpenWriteStream(const std::string& path,
    std::ios::openmode flags)
{
    std::ofstream stream(path, flags);
    if (stream.is_open())
    {
        return stream;
    }
    else
    {
        return std::nullopt;
    }
}

bool fileIO::Exists(const std::string& path)
{
    return std::filesystem::exists(path);
}

bool fileIO::MakeDirectory(const std::string& path)
{
    std::error_code e {};
    std::filesystem::create_directory(path, e);
    if (e == std::error_code {})
        return true;
    else
        return false;
}

std::optional<fileIO::FileTime> fileIO::GetLastModifiedTime(const std::string& path)
{
    if (Exists(path))
    {
        return std::filesystem::last_write_time(path);
    }
    else
    {
        return std::nullopt;
    }
}

std::vector<std::byte> fileIO::DumpStreamIntoBytes(std::istream& stream)
{
    stream.seekg(0, std::ios::end);
    size_t size = stream.tellg();
    std::vector<std::byte> out(size, {});
    stream.seekg(0);
    stream.read(std::bit_cast<char*>(out.data()), size);
    return out;
}

std::string fileIO::DumpStreamIntoString(std::istream& stream)
{
    stream.seekg(0, std::ios::end);
    size_t size = stream.tellg();
    std::string out(size, {});
    stream.seekg(0);
    stream.read(std::bit_cast<char*>(out.data()), size);
    return out;
}

std::string fileIO::CanonicalizePath(const std::string& path)
{
    return std::filesystem::path(path).make_preferred().lexically_normal().string();
}

std::string fileIO::GetExtension(const std::string& path)
{
    return std::filesystem::path(path).extension().string();
}

std::string fileIO::PathAppend(const std::string& path1, const std::string& path2)
{
    std::filesystem::path a { path1 };
    std::filesystem::path b { path2 };

    return fileIO::CanonicalizePath((a / b).string());
}

std::string fileIO::GetDirectory(const std::string& path)
{
    return std::filesystem::path(path).parent_path().string();
}