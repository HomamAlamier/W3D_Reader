#include <w3d_model.h>
#include <binaryreader.h>

NAMESPACE(w3d)

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

NAMESPACE_END
