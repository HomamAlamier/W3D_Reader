#include "binaryreader.h"
#include <memory.h>

BinaryReader::BinaryReader(const std::string& filename)
    : _stream(new FileReadStream(new std::fstream(filename, std::ios::in)))
{
}

BinaryReader::BinaryReader(const char* buffer, size_t size)
    : _stream(new MemoryReadStream(buffer, size))
{}

BinaryReader::~BinaryReader()
{
    if (_stream)
    {
        delete _stream;
    }
}

void BinaryReader::close()
{
    _stream->close();
}
size_t BinaryReader::readQuat(glm::quat& q)
{
    float tmp[4];
    size_t r = read(tmp, sizeof(float) * 4);
    q = glm::make_quat(tmp);
    return r;
}
glm::mat4 BinaryReader::readMat4()
{
    float tmp[16] = { 0.f };
    read(tmp, 16 * sizeof(float));
    return glm::mat4(tmp[0],  tmp[1],  tmp[2],  tmp[3],
                     tmp[4],  tmp[5],  tmp[6],  tmp[7],
                     tmp[8],  tmp[9],  tmp[10], tmp[11],
                     tmp[12], tmp[13], tmp[14], tmp[15]);
}

size_t BinaryReader::seekFromCurrent(size_t count)
{
    return _stream->seekFromCurrent(count);
}

FileReadStream::FileReadStream(std::fstream* s)
    : _fstream(s)
{}

FileReadStream::~FileReadStream()
{
    delete _fstream;
}

void FileReadStream::close()
{
    _fstream->close();
}

size_t FileReadStream::read(char *buffer, size_t size)
{
    if (!_fstream || _fstream->bad() || _fstream->eof())
        return 0;
    return _fstream->read(buffer, size).gcount();
}

size_t FileReadStream::seekFromCurrent(size_t count)
{
    if (!_fstream || _fstream->bad() || _fstream->eof())
        return 0;
    return _fstream->seekg(count, std::ios_base::cur).gcount();
}

MemoryReadStream::MemoryReadStream(const char* buffer, size_t size)
    : _buffer(buffer)
    , _size(size)
    , _cursor(0)
{
}

MemoryReadStream::~MemoryReadStream()
{
    close();
}

void MemoryReadStream::close()
{
    delete[] _buffer;
    _cursor = _size;
    _buffer = nullptr;
}

size_t MemoryReadStream::read(char *buffer, size_t size)
{
    if (!_buffer || _cursor == _size)
        return 0;
    size_t sz = (_size - _cursor > size) ? size : _size - _cursor;
    memcpy(buffer, &_buffer[_cursor], sz);
    _cursor += sz;
    return sz;
}

size_t MemoryReadStream::seekFromCurrent(size_t count)
{
    if (!_buffer || _cursor == _size)
        return 0;
    size_t sz = _size - _cursor;
    if (count > sz)
    {
        _cursor += sz;
        return sz;
    }
    else
    {
        _cursor += count;
        return count;
    }
}

bool MemoryReadStream::eof()
{
    return !_buffer || _cursor == _size;
}
