#include <w3d_model.h>
#include <binaryreader.h>

W3D_Model::W3D_Model(const std::string& filename)
    : _filename(filename)
{
    BinaryReader br(filename);
    while (br.isOpen())
    {
        readChunk(&br);
    }
    br.close();
}

void W3D_Model::readChunk(BinaryReader* br)
{
    W3D_ChunkHeader* header = new W3D_ChunkHeader(br);
    header->print();
    W3D_Chunk* c = nullptr;
    switch (header->type())
    {
    case W3D_ChunkHeader::W3D_CHUNK_HIERARCHY:
        _hierarchy = new W3D_Hierarchy(header, br);
        break;
    case W3D_ChunkHeader::W3D_CHUNK_MESH:
        _meshs.push_back(W3D_Mesh(header, br));
        break;
    default:
        br->seekFromCurrent(header->size());
        delete header;
        break;
    }
}

/*
W3D_Reader::W3D_Reader(const std::string& filename)
    : _filename(filename)
    , _binaryReader(nullptr)
{

}

bool W3D_Reader::open()
{
    _binaryReader = new BinaryReader(_filename);
    return _binaryReader->isOpen();
}
bool W3D_Reader::processChunk(BinaryReader *reader)
{
    if (!reader && !open())
        return false;
    if (!reader->isOpen())
        return false;
    W3D_ChunkHeader header(reader);
    return processChunk(reader, &header);
}
bool W3D_Reader::processChunk(BinaryReader *reader, W3D_ChunkHeader* header)
{
    if (!reader && !open())
        return false;
    if (!reader->isOpen())
        return false;
    header->print();
    W3D_Chunk* c = nullptr;
    switch (header->type())
    {
    case W3D_ChunkHeader::W3D_CHUNK_HIERARCHY:
        c = new W3D_Hierarchy(header, _binaryReader);
        break;
    default:
        header->setDataOnly();
        c = new W3D_DataChunk(header, reader);
        break;
    }
    if (c && _pcCallback)
        _pcCallback(c);
    delete c;
    return true;
}

bool W3D_Reader::readChunk()
{
    W3D_ChunkHeader* header = new W3D_ChunkHeader(_binaryReader);
    header->print();
    processChunk(_binaryReader, header);
    delete header;
    return true;
}*/
