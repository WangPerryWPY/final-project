#include "ShaderManager.h"
ShaderManager* ShaderManager::instance = NULL;

void ShaderManager::InitialShader() {
    blockShader.setText("shadow.vs", "shadow.frag");
    simpleDepthShader.setText("shadow_mapping_depth.vs", "shadow_mapping_depth.frag");
    directionalLight.setText("multipleLight.vs", "multipleLight.frag");

    // ����ƽ�й�Ĳ���
    directionalLight.use();
    directionalLight.setInt("material.diffuse", 0);
    directionalLight.setInt("material.specular", 1);

    // ���÷�����ɫ������ 
    blockShader.setInt("diffuseTexture", 0);
    blockShader.setInt("shadowMap", 1);
}