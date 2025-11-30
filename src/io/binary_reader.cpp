#include "binary_reader.h"

std::string BinaryReader::read_n_chars(const BytesBuffer &buffer, size_t size, size_t &offset)
{
    if (offset + size > buffer.size())
        throw std::runtime_error("buffer is too small");

    std::string result(reinterpret_cast<const char *>(buffer.data() + offset), size);
    offset += size;
    return result;
}

int32_t BinaryReader::readInt32(const BytesBuffer &buffer, size_t &offset)
{
    if (offset + 4 > buffer.size())
        throw std::runtime_error("buffer is too small");

    uint32_t value;
    std::memcpy(&value, buffer.data() + offset, 4);
    offset += 4;
    return value;
}

std::string BinaryReader::readLineTrimmed(const BytesBuffer &buffer, size_t &offset)
{
    for (size_t i = offset; i < buffer.size(); i++)
    {
        if (buffer[i] != '\n')
            continue;

        size_t size = i - offset;

        std::string line(reinterpret_cast<const char *>(&buffer[offset]), size);
        offset += size + 1;

        return line;
    }

    throw std::runtime_error("line terminator not found");
}

BytesBuffer BinaryReader::readExact(const BytesBuffer &buffer, uint32_t size, size_t &offset)
{
    if (offset + size > buffer.size())
        throw std::runtime_error("buffer is too small");

    BytesBuffer result(size);
    std::memcpy(result.data(), buffer.data() + offset, size);
    offset += size;

    return result;
}
