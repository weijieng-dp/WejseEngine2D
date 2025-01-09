#pragma once
#include"component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct TransformComponent 
{
    glm::vec3 translate = glm::vec3(0,0, 0);
    float rotation = 90;
    glm::vec3 scale = glm::vec3(250, 250, 1);
    glm::vec3 rotate = glm::vec3(0, 0,1.0f);

    glm::mat4 transform = glm::mat4(1.0f);

};