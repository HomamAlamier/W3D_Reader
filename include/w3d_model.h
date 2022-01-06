#ifndef W3D_READER_H
#define W3D_READER_H
#include <string>
#include <w3d.h>

NAMESPACE(w3d)

class W3D_Model
{
    READ_PROP_PTR(hierarchy, W3D_Hierarchy);
    READ_PROP_REF(meshs, std::vector<W3D_Mesh>)
    READ_PROP_REF(filename, std::string)
public:
    W3D_Model(const std::string& filename);
    bool loaded() const { return _hierarchy && _meshs.size() > 0; }
private:
    void readChunk(BinaryReader*);
};

NAMESPACE_END
#endif // !W3D_READER_H
