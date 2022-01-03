#ifndef UTILS_H
#define UTILS_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
std::string glmVec2ToString(const glm::vec2& v);
std::string glmVec3ToString(const glm::vec3& v);
std::string glmVec4ToString(const glm::vec4& v);
std::string glmVec2ToString(const glm::ivec2& v);
std::string glmVec3ToString(const glm::ivec3& v);
std::string glmVec4ToString(const glm::ivec4& v);
#endif // UTILS_H
