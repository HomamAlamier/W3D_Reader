#include <w3d.h>
#include <iostream>
#include <memory.h>

NAMESPACE(w3d)

W3D_ChunkHeader::W3D_ChunkHeader(BinaryReader* stream)
    : _subChunks(false)
    , _size(0)
{
    if (stream && stream->isOpen())
    {
        uint32_t type = 0;
        if (stream->read(&type))
        {
            _type = type;
        }
        uint32_t tmp = 0;
        if (stream->read(&tmp))
        {
            _subChunks = (tmp >> 31);
            _size = tmp & 0x7FFFFFFF;
        }
    }
}

void W3D_ChunkHeader::print() const
{
    std::cout << "W3D_ChunkHeader:\nsubChunks ? " << (_subChunks ? "Yes" : "No")
              << "\nsize = " << _size << std::endl;
}

W3D_Chunk::W3D_Chunk(const W3D_ChunkHeader& header)
    : _header(header)
{

}

W3D_DataChunk::W3D_DataChunk(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    _data = new char[header.size()];
    reader->read(_data, header.size());
}

W3D_HierarchyHeader::W3D_HierarchyHeader(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    reader->read(&_version);
    reader->read(&_name);
    reader->read(&_numPivots);
    reader->readVec3(_center);
}

void W3D_HierarchyHeader::print() const
{
    std::cout << "W3D_HierarchyHeader:\n"
              << COUT_VAR(version)
              << COUT_VAR(name)
              << COUT_VAR(numPivots)
              << COUT_VEC3(center) << std::endl;
}

W3D_Pivot::W3D_Pivot(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    reader->read(_name, W3D_NAME_STRING_SIZE);
    reader->read(&_parentIdx);
    reader->readVec3(_translation);
    reader->readVec3(_eulerAngles);
    reader->readQuat(_rotation);
}

void W3D_Pivot::print() const
{
    std::cout << "W3D_Pivot:\n"
              << COUT_VAR(name)
              << COUT_VAR(parentIdx)
              << COUT_VEC3(translation)
              << COUT_VEC3(eulerAngles)
              << std::endl;
}

W3D_Hierarchy::W3D_Hierarchy(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    size_t sz = 0;
    do
    {
        W3D_ChunkHeader h(reader);
        sz += h.size() + W3D_ChunkHeader::W3D_ChunkHeaderSize;
        switch (h.type())
        {
        case W3D_CHUNK_HIERARCHY_HEADER:
            _hierarchyHeader = W3D_HierarchyHeader(h, reader);
            _rotateMatrix = nullptr;
            _visible = new uint32_t[_hierarchyHeader.numPivots()];
            memset(_visible, 1, sizeof(uint32_t) * _hierarchyHeader.numPivots());
            break;
        case W3D_CHUNK_PIVOTS:
            for (size_t i = 0; i < _hierarchyHeader.numPivots(); ++i)
            {
                _pivots.push_back(new W3D_Pivot(h, reader));
            }
            break;
        case W3D_CHUNK_PIVOT_FIXUPS:
        {
            uint32_t matSize = 12 * sizeof(float);
            uint32_t aSize = h.size() / matSize;
            for(uint32_t i = 0; i < aSize; ++i)
            {
                float matData[12];
                reader->read(matData, matSize);
                _pivotFixups.push_back(glm::make_mat4x3(matData));
            }
            break;
        }
        default:
            reader->seekFromCurrent(h.size());
            break;
        }
    } while (sz < header.size());
}

void W3D_Hierarchy::print() const
{
    std::cout << "W3D_Hierarchy:" << std::endl;
    _hierarchyHeader.print();
    std::cout << "pivots:" << std::endl;
    for(W3D_Pivot* pivot : _pivots)
        pivot->print();
}

W3D_MeshHeader::W3D_MeshHeader(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    reader->read(&_version);
    reader->read(&_attributes);
    reader->read(_meshName, W3D_NAME_STRING_SIZE);
    reader->read(_containerName, W3D_NAME_STRING_SIZE);
    reader->read(&_numTris);
    reader->read(&_numVertices);
    reader->read(&_numMaterials);
    reader->read(&_numDamageStages);
    reader->read(&_sortLevel);
    reader->read(&_prelitVersion);
    reader->read(_futureCounts);
    reader->read(&_vertexChannels);
    reader->read(&_faceChannels);
    reader->readVec3(_min);
    reader->readVec3(_max);
    reader->readVec3(_sphCenter);
    reader->read(&_sphRadius);
}

void W3D_MeshHeader::print() const
{
    std::cout << "W3D_MeshHeader:\n"
              << COUT_VAR(version)
              << COUT_VAR(attributes)
              << COUT_VAR(meshName)
              << COUT_VAR(containerName)
              << COUT_VAR(numTris)
              << COUT_VAR(numVertices)
              << COUT_VAR(numMaterials)
              << COUT_VAR(numDamageStages)
              << COUT_VAR(sortLevel)
              << COUT_VAR(futureCounts[0])
              << COUT_VAR(vertexChannels)
              << COUT_VAR(faceChannels)
              << COUT_VEC3(min)
              << COUT_VEC3(max)
              << COUT_VEC3(sphCenter)
              << COUT_VAR(sphRadius);
}

W3D_Material:: W3D_Material(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    size_t sz = 0;
    do
    {
        W3D_ChunkHeader h(reader);
        sz += h.size() + W3D_ChunkHeader::W3D_ChunkHeaderSize;
        switch (h.type())
        {
        case W3D_CHUNK_VERTEX_MATERIAL_NAME:
            _name = new char[h.size()];
            memset(_name, 0, h.size());
            reader->read(_name, h.size());
            break;
        case W3D_CHUNK_VERTEX_MAPPER_ARGS0:
            _mapperArgs0 = new char[h.size()];
            memset(_mapperArgs0, 0, h.size());
            reader->read(_mapperArgs0, h.size());
            break;
        case W3D_CHUNK_VERTEX_MAPPER_ARGS1:
            _mapperArgs1 = new char[h.size()];
            memset(_mapperArgs1, 0, h.size());
            reader->read(_mapperArgs1, h.size());
            break;
        case W3D_CHUNK_VERTEX_MATERIAL_INFO:
            _vertexMaterialInfo = W3D_VertexMaterial(reader);
            break;
        default:
            reader->seekFromCurrent(h.size());
            break;
        }
    } while (sz < header.size());
}

void W3D_Material::print() const
{
    std::cout << "W3D_Material:\n"
              << COUT_VAR(name)
              << COUT_VAR(mapperArgs0)
              << COUT_VAR(mapperArgs1);
    _vertexMaterialInfo.print();
}

W3D_Texture::W3D_Texture(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    size_t sz = 0;
    do
    {
        W3D_ChunkHeader h(reader);
        sz += h.size() + W3D_ChunkHeader::W3D_ChunkHeaderSize;
        switch (h.type())
        {
        case W3D_CHUNK_TEXTURE_NAME:
            _name = new char[h.size()];
            memset(_name, 0, h.size());
            reader->read(_name, h.size());
            break;
        case W3D_CHUNK_TEXTURE_INFO:
            _textureInfo = W3D_TextureInfo(reader);
            _loaded = 1;
            break;
        default:
            reader->seekFromCurrent(h.size());
            break;
        }
    } while (sz < header.size());
}

void W3D_Texture::print() const
{
    std::cout << "W3D_Texture:\n"
              << COUT_VAR(name);
    _textureInfo.print();
}


W3D_TextureStage::W3D_TextureStage(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    size_t sz = 0;
    do
    {
        W3D_ChunkHeader h(reader);
        sz += h.size() + W3D_ChunkHeader::W3D_ChunkHeaderSize;
        switch (h.type())
        {
        case W3D_CHUNK_TEXTURE_IDS:
            _textureIDCount = h.size() / sizeof(uint32_t);
            for(uint32_t i = 0; i < _textureIDCount; ++i)
            {
                uint32_t v = 0;
                reader->read(&v);
                _textureIDs.push_back(v);
            }
            break;
        case W3D_CHUNK_PER_FACE_TEXCOORD_IDS:
            _perFaceTexCoordIDCount = h.size() / (3 * sizeof(float));
            for(uint32_t i = 0; i < _perFaceTexCoordIDCount; ++i)
            {
                glm::u32vec3 v;
                reader->readVec3(v);
                _perFaceTexCoordIDs.push_back(v);
            }
            break;
        case W3D_CHUNK_STAGE_TEXCOORDS:
            for(uint32_t i = 0; i < h.size() / (2 * sizeof(float)); ++i)
            {
                glm::vec2 v;
                reader->readVec2(v);
                _texCoords.push_back(v);
            }
            break;
        default:
            reader->seekFromCurrent(h.size());
            break;
        }
    } while (sz < header.size());
}

void W3D_TextureStage::print() const
{
    std::cout << "W3D_TextureStage:\n"
              << COUT_VAR(textureIDCount)
              << COUT_VAR(perFaceTexCoordIDCount);
}

W3D_MaterialPass::W3D_MaterialPass(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    size_t sz = 0;
    do
    {
        W3D_ChunkHeader h(reader);
        sz += h.size() + W3D_ChunkHeader::W3D_ChunkHeaderSize;
        switch (h.type())
        {
        case W3D_CHUNK_VERTEX_MATERIAL_IDS:
            _vertexMaterialIDCount = h.size() / sizeof(uint32_t);
            for(uint32_t i = 0; i < _vertexMaterialIDCount; ++i)
            {
                uint32_t v = 0;
                reader->read(&v);
                _vertexMaterialIDs.push_back(v);
            }
            break;
        case W3D_CHUNK_SHADER_IDS:
            _shaderIDCount = h.size() / sizeof(uint32_t);
            for(uint32_t i = 0; i < _shaderIDCount; ++i)
            {
                uint32_t v = 0;
                reader->read(&v);
                _shaderIDs.push_back(v);
            }
            break;
        case W3D_CHUNK_DCG:
            for(uint32_t i = 0; i < h.size() / 4; ++i)
            {
                _dcg.push_back(W3D_RGBA(reader));
            }
            break;
        case W3D_CHUNK_DIG:
            for(uint32_t i = 0; i < h.size() / 4; ++i)
            {
                _dig.push_back(W3D_RGBA(reader));
            }
            break;
        case W3D_CHUNK_SCG:
            for(uint32_t i = 0; i < h.size() / 4; ++i)
            {
                _scg.push_back(W3D_RGBA(reader));
            }
            break;
        case W3D_CHUNK_TEXTURE_STAGE:
            _stageCount++;
            _textureStage.push_back(W3D_TextureStage(h, reader));
            break;
        default:
            reader->seekFromCurrent(h.size());
            break;
        }
    } while (sz < header.size());
}

void W3D_MaterialPass::print() const
{
    std::cout << "W3D_MaterialPass:\n"
              << COUT_VAR(vertexMaterialIDCount)
              << COUT_VAR(shaderIDCount)
              << COUT_VAR(stageCount);
}

W3D_Mesh::W3D_Mesh(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    size_t sz = 0;
    do
    {
        W3D_ChunkHeader h(reader);
        sz += h.size() + W3D_ChunkHeader::W3D_ChunkHeaderSize;
        switch (h.type())
        {
        case W3D_CHUNK_MESH_HEADER3:
            _meshHeader = W3D_MeshHeader(h, reader);
            break;
        case W3D_CHUNK_VERTICES:
            for(uint32_t i = 0; i < _meshHeader.numVertices(); ++i)
            {
                glm::vec3 v;
                reader->readVec3(v);
                _vertices.push_back(v);
            }
            break;
        case W3D_CHUNK_VERTEX_NORMALS:
            for(uint32_t i = 0; i < _meshHeader.numVertices(); ++i)
            {
                glm::vec3 v;
                reader->readVec3(v);
                _normals.push_back(v);
            }
            break;
        case W3D_CHUNK_VERTEX_INFLUENCES:
            for(uint32_t i = 0; i < _meshHeader.numVertices(); ++i)
            {
                _influences.push_back(W3D_VertexInfluence(reader));
            }
            break;
        case W3D_CHUNK_TRIANGLES:
            for(uint32_t i = 0; i < _meshHeader.numTris(); ++i)
            {
                _triangles.push_back(W3D_Triangle(reader));
            }
            break;
        case W3D_CHUNK_VERTEX_SHADE_INDICES:
            for(uint32_t i = 0; i < _meshHeader.numVertices(); ++i)
            {
                uint32_t v = 0;
                reader->read(&v);
                _shadeIndices.push_back(v);
            }
            break;
        case W3D_CHUNK_MATERIAL_INFO:
            _materialInfo = W3D_MaterialInfo(reader);
            break;
        case W3D_CHUNK_VERTEX_MATERIALS:
            for(uint32_t i = 0; i < _materialInfo.vertexMaterialCount(); ++i)
            {
                W3D_ChunkHeader h2(reader);
                if (h2.type() == W3D_CHUNK_VERTEX_MATERIAL)
                {
                    _materials.push_back(W3D_Material(h2, reader));
                }
                else
                {
                    reader->seekFromCurrent(h2.size());
                }
            }
            break;
        case W3D_CHUNK_SHADERS:
            for(uint32_t i = 0; i < _materialInfo.shaderCount(); ++i)
            {
                _shaders.push_back(W3D_Shader(reader));
            }
            break;
        case W3D_CHUNK_TEXTURES:
            for(uint32_t i = 0; i < _materialInfo.textureCount(); ++i)
            {
                W3D_ChunkHeader h2(reader);
                if (h2.type() == W3D_CHUNK_TEXTURE)
                {
                    _textures.push_back(W3D_Texture(h2, reader));
                }
                else
                {
                    reader->seekFromCurrent(h2.size());
                }
            }
            break;
        case W3D_CHUNK_MATERIAL_PASS:
            _materialPass.push_back(W3D_MaterialPass(h, reader));
            break;
        default:
            reader->seekFromCurrent(h.size());
            break;
        }
    } while (sz < header.size());
}

void W3D_Mesh::print() const
{

}


W3D_HLodArray::W3D_HLodArray(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    size_t sz = 0;
    do
    {
        W3D_ChunkHeader h(reader);
        sz += h.size() + W3D_ChunkHeader::W3D_ChunkHeaderSize;
        switch (h.type())
        {
        case W3D_CHUNK_HLOD_SUB_OBJECT_ARRAY_HEADER:
            _arrayHeader = W3D_HLodArrayHeader(reader);
            break;
        case W3D_CHUNK_HLOD_SUB_OBJECT:
            _subObjects.push_back(W3D_HLodSubObject(reader));
            break;
        default:
            reader->seekFromCurrent(h.size());
            break;
        }
    } while (sz < header.size());
}

void W3D_HLodArray::print() const
{

}

W3D_HLod::W3D_HLod(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    size_t sz = 0;
    if (header.size() == 0)
        return;
    do
    {
        W3D_ChunkHeader h(reader);
        sz += h.size() + W3D_ChunkHeader::W3D_ChunkHeaderSize;
        switch (h.type())
        {
        case W3D_CHUNK_HLOD_HEADER:
            _hLodHeader = W3D_HLodHeader(reader);
            break;
        case W3D_CHUNK_HLOD_LOD_ARRAY:
            _lods.push_back(W3D_HLodArray(h, reader));
            break;
        default:
            reader->seekFromCurrent(h.size());
            break;
        }
    } while (sz < header.size());
}

void W3D_HLod::print() const
{

}

W3D_AnimationChannel::W3D_AnimationChannel(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    _animationChannelheader = W3D_AnimationChannelHeader(reader);
    uint32_t sz = header.size() - W3D_AnimationChannelHeader::Size;
    //sz -= 16;
    auto x = sz / sizeof(float);
     reader->readVector(_data, x);
}

void W3D_AnimationChannel::print() const
{

}

W3D_BitChannel::W3D_BitChannel(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
{
    _bitChannelHeader = W3D_BitChannelHeader(reader);
    reader->seekFromCurrent(-1);
    uint32_t dataCount = _bitChannelHeader.lastFrame() - _bitChannelHeader.firstFrame() + 1;
    uint32_t dataSize = dataCount / 8;
    uint32_t dataRem = dataCount % 8;
    if (dataRem != 0)
        ++dataSize;
    reader->readVector(_data, dataSize);
}

void W3D_BitChannel::print() const
{

}

W3D_Animation::W3D_Animation(const W3D_ChunkHeader& header, BinaryReader* reader)
    : W3D_Chunk(header)
    , _animationChannelCount(0)
{
    size_t sz = 0;
    do
    {
        W3D_ChunkHeader h(reader);
        sz += h.size() + W3D_ChunkHeader::W3D_ChunkHeaderSize;
        switch (h.type())
        {
        case W3D_CHUNK_ANIMATION_HEADER:
            _animationHeader = W3D_AnimationHeader(reader);
            break;
        case W3D_CHUNK_ANIMATION_CHANNEL:
            _animationChannels.push_back(W3D_AnimationChannel(h, reader));
            ++_animationChannelCount;
            break;
        case W3D_CHUNK_BIT_CHANNEL:
            _bitChannels.push_back(W3D_BitChannel(h, reader));
            ++_bitChannelCount;
            break;
        default:
            reader->seekFromCurrent(h.size());
            break;
        }
    } while (sz < header.size());
}

void W3D_Animation::print() const
{

}

NAMESPACE_END
