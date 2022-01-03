#ifndef W3D_READER_H
#define W3D_READER_H
#include <string>
#include <w3d.h>
class W3D_Model
{
    READ_PROP_PTR(hierarchy, W3D_Hierarchy);
    READ_PROP_REF(meshs, std::vector<W3D_Mesh>)
    READ_PROP_REF(filename, std::string)
public:
    W3D_Model(const std::string& filename);
private:
    void readChunk(BinaryReader*);
};

/*
class W3D_Chunk;
class W3D_ChunkHeader;
typedef void (*ProcessChunkCallback)(W3D_Chunk*);

class W3D_Reader
{
public:
    W3D_Reader(const std::string& filename);
    bool open();
    bool readChunk();
    void setProcessChunkCallback(ProcessChunkCallback cb) { _pcCallback = cb; }
    const std::string& filename() const { return _filename; }
private:
    bool processChunk(BinaryReader* reader);
    bool processChunk(BinaryReader* reader, W3D_ChunkHeader* header);
    std::string _filename;
    BinaryReader* _binaryReader;
    ProcessChunkCallback _pcCallback;
};
*/

#endif // !W3D_READER_H
