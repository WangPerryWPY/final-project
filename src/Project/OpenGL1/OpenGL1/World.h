#pragma once
#include "ResourceManager.h"
#include "Shader.h"
#include "PhysicsEngine.h"

//���������������ķ���
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
	//��Ⱦģ��
	void Rendermodel(Shader& shader, int count, glm::vec3* position, ResourceManager::ModelType model);
    // ������
    void DrawTree(ResourceManager::BlockType tree_type, ResourceManager::BlockType leave_type, glm::vec3 pos, Shader& shader);
private:
    static World* instance;
};