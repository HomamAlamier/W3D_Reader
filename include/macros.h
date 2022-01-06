#ifndef MACROS_H
#define MACROS_H

#include <utils.h>
#ifdef __cplusplus
    #define NAMESPACE(X) namespace X {
    #define NAMESPACE_END }
#else
    #define NAMESPACE(X)
    #define NAMESPACE_END
#endif

#define READ_PROP(NAME, TYPE) \
    public: \
    TYPE NAME() const { return _##NAME; } \
    private: \
    TYPE _##NAME;

#define READ_PROP_PTR(NAME, TYPE) \
    public: \
    const TYPE* NAME() const { return _##NAME; } \
    private: \
    TYPE* _##NAME;

#define READ_PROP_REF(NAME, TYPE) \
    public: \
    const TYPE& NAME() const { return _##NAME; } \
    private: \
    TYPE _##NAME;

#define READ_PROP_ARRAY(NAME, TYPE) \
    public: \
    const TYPE* NAME() const { return _##NAME; } \
    private: \
    TYPE* _##NAME;

#define READ_PROP_ARRAY_SIZE(NAME, TYPE, SIZE) \
    public: \
    const TYPE* NAME() const { return _##NAME; } \
    private: \
    TYPE _##NAME[SIZE];

#define BINARY_CTOR(X) \
    public: \
    X() = default; \
    X(BinaryReader* reader);

#define BREAD(X) reader->read(&_##X);
#define BREAD2(X, S) reader->read(_##X, S);
#define BVEC1(X) reader->readVec1(_##X);
#define BVEC2(X) reader->readVec2(_##X);
#define BVEC3(X) reader->readVec3(_##X);
#define BVEC4(X) reader->readVec4(_##X);
#define BCLASS(X, T) _##X = T(reader);

#define AUTO_BINARY_CTOR_START(X) \
    public: \
    X() = default; \
    X(BinaryReader* reader) {

#define AUTO_BINARY_CTOR_END }

#define COUT_VAR(X) #X << " = " << _##X << std::endl
#define COUT_VEC2(X) #X << " = " << glmVec2ToString(_##X) << std::endl
#define COUT_VEC3(X) #X << " = " << glmVec3ToString(_##X) << std::endl
#define COUT_VEC4(X) #X << " = " << glmVec4ToString(_##X) << std::endl

#define BINARY_PRINT_START(X) \
    void print() const { std::cout << #X << ":\n"

#define BINARY_PRINT_END ;}

#endif // MACROS_H
