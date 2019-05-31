#pragma once
#include "ResourceManager.h"
#include "Shader.h"
#include "PhysicsEngine.h"
class World
{
public:
    World(){}
    ~World(){}

    static World* getInstance() {
        if (instance == NULL) {
            instance = new World();
        }
        return instance;
    }

    void Render(Shader& shader);
    // ����������Ⱦ
    void SingleRender(Shader& shader, int count, glm::vec3* position, ResourceManager::BlockType block);

private:
    static World* instance;
};