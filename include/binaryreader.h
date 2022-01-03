#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <fstream>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
class ReadStream
{
public:
    virtual ~ReadStream() {}
    virtual void close() = 0;
    virtual size_t read(char* buffer, size_t size) = 0;
    virtual size_t seekFromCurrent(size_t count) = 0;
    virtual bool eof() = 0;
};

class FileReadStream : public ReadStream
{
public:
    FileReadStream(std::fstream* s);
    ~FileReadStream();
    void close() override;
    size_t read(char *buffer, size_t size) override;
    size_t seekFromCurrent(size_t count) override;
    bool eof() override { return !_fstream || _fstream->eof(); }
private:
    std::fstream* _fstream;
};

class MemoryReadStream : public ReadStream
{
public:
    MemoryReadStream(const char* buffer, size_t size);
    ~MemoryReadStream();
    void close() override;
    size_t read(char *buffer, size_t size) override;
    size_t seekFromCurrent(size_t count) override;
    bool eof() override;
private:
    const char* _buffer;
    size_t _size;
    size_t _cursor;
};


class BinaryReader
{
public:
    BinaryReader(const std::string& filename);
    BinaryReader(const char* buffer, size_t size);
    ~BinaryReader();

    template <typename T>
    size_t read(T* buffer, size_t len = sizeof(T))
    {
        if (!isOpen())
            return 0;
        return _stream->read((char*)buffer, len);
    }
    template<typename T, glm::qualifier Q>
    size_t readVec1(glm::vec<1, T, Q>& vec)
    {
        T tmp[1];
        size_t r = read(tmp, sizeof(T) * 1);
        vec = glm::make_vec1(tmp);
        return r;
    }
    template<typename T, glm::qualifier Q>
    size_t readVec2(glm::vec<2, T, Q>& vec)
    {
        T tmp[2];
        size_t r = read(tmp, sizeof(T) * 2);
        vec = glm::make_vec2(tmp);
        return r;
    }
    template<typename T, glm::qualifier Q>
    size_t readVec3(glm::vec<3, T, Q>& vec)
    {
        T tmp[3];
        size_t r = read(tmp, sizeof(T) * 3);
        vec = glm::make_vec3(tmp);
        return r;
    }
    template<typename T, glm::qualifier Q>
    size_t readVec4(glm::vec<4, T, Q>& vec)
    {
        T tmp[4];
        size_t r = read(tmp, sizeof(T) * 4);
        vec = glm::make_vec4(tmp);
        return r;
    }
    glm::mat4 readMat4();
    void close();
    size_t seekFromCurrent(size_t count);
    bool isOpen() const { return _stream && !_stream->eof(); }
private:
    ReadStream* _stream;
};

#endif // BINARYREADER_H
