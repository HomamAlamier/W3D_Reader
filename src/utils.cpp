#include <utils.h>
std::string glmVec2ToString(const glm::vec2& v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}
std::string glmVec3ToString(const glm::vec3& v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}
std::string glmVec4ToString(const glm::vec4& v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
}
std::string glmVec2ToString(const glm::ivec2& v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}
std::string glmVec3ToString(const glm::ivec3& v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}
std::string glmVec4ToString(const glm::ivec4& v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
}

template <int comp, typename T, glm::qualifier T2>
void xxx(glm::vec<comp, T, T2> s)
{

}

void vv()
{
    xxx(glm::vec2());
    xxx(glm::ivec2());
}
