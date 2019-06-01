#pragma once
#include "Initial.h"
#include "Shader.h"
#include "Camera.h"
// ��������е���ɫ�����й���
class ShaderManager
{
public:
    ShaderManager() {}
    void InitialShader();
    static ShaderManager* getInsatance() {
        if (instance == NULL) {
            instance = new ShaderManager();
        }
        return instance;
    }

    void DirectionalLightSet() {
        // ƽ�й�Դ
        directionalLight.setVec3("dirLight.direction", direcionalDirection);
        directionalLight.setVec3("dirLight.ambient", directionalAmbient);
        directionalLight.setVec3("dirLight.diffuse", directionalDiffuse);
        directionalLight.setVec3("dirLight.specular", directionalSpecular);
    }

    void ObjectShaderSet(glm::mat4 projection, glm::mat4 view, glm::vec3 lightPos, glm::mat4 lightSpaceMatrix) {
        // ���ÿɱ����ɫ������
        blockShader.setMat4("projection", projection);
        blockShader.setMat4("view", view);
        blockShader.setVec3("viewPos", Camera::getInstance()->Position);
        blockShader.setVec3("lightPos", lightPos);
        blockShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        // ���巴����������
        blockShader.setFloat("ambientStrength", 0.3f);
        blockShader.setFloat("shininess", 64.0f);
        blockShader.setFloat("diffuseFactor", 1.0f);
        blockShader.setFloat("specularStrength", 1.0f);
        blockShader.setVec3("lightColor", glm::vec3(1.0f));
    }
    Shader blockShader;
    Shader simpleDepthShader;
    Shader directionalLight;

private:
    static ShaderManager* instance;
    // ----------ƽ�й����----------------------------
    // �����Դ
    glm::vec3 direcionalDirection = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 directionalAmbient = glm::vec3(0.4f);
    glm::vec3 directionalDiffuse = glm::vec3(0.6f);
    glm::vec3 directionalSpecular = glm::vec3(0.1f);
};