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

void W3D_Model::readHModel(BinaryReader* reader, W3D_ChunkHeader* header)
{
    /*size_t sz = 0;
    do
    {
        W3D_ChunkHeader* h = new W3D_ChunkHeader(reader);
        sz += h->size() + W3D_ChunkHeader::W3D_ChunkHeaderSize;
        bool del = true;
        switch (h->type())
        {
        case W3D_CHUNK_HMODEL_HEADER:
            W3D_HModelHeader hmh(reader);
            _hlod[0].
            break;
        case W3D_CHUNK_HLOD_LOD_ARRAY:
            del = false;
            break;
        default:
            reader->seekFromCurrent(h->size());
            break;
        }
        if (del)
            delete h;
    } while (sz < header->size());*/
}

void W3D_Model::readChunk(BinaryReader* br)
{
    W3D_ChunkHeader header(br);
    W3D_Chunk* c = nullptr;
    switch (header.type())
    {
    case W3D_CHUNK_HIERARCHY:
        _hierarchy = W3D_Hierarchy(header, br);
        break;
    case W3D_CHUNK_MESH:
        _meshs.push_back(W3D_Mesh(header, br));
        break;
    case W3D_CHUNK_HLOD:
        _hlod.push_back(W3D_HLod(header, br));
        break;
    case W3D_CHUNK_ANIMATION:
        _animations.push_back(W3D_Animation(header, br));
        break;
    default:
        br->seekFromCurrent(header.size());
        break;
    }
}

NAMESPACE_END
