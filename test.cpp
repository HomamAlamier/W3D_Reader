#include <w3d_model.h>
#include <iostream>

void pchunk(W3D_Chunk* c)
{
    c->print();
}

int main()
{
    W3D_Model mdl("test.W3D");
    mdl.hierarchy()->print();
    return 0;
}
