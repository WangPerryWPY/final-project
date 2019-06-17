#include "ShaderManager.h"
ShaderManager* ShaderManager::instance = NULL;

void ShaderManager::InitialShader() {
    blockShader.setText("shadow.vs", "shadow.frag");
    simpleDepthShader.setText("shadow_mapping_depth.vs", "shadow_mapping_depth.frag");
	textshader.setText("text.vs", "text.fs");

    // ����ƽ�й�Ĳ���
    blockShader.setVec3("dir", glm::vec3(-10.0f, 0.0f, 0.0f));

    // ���÷�����ɫ������ 
    blockShader.setInt("diffuseTexture", 0);
    blockShader.setInt("shadowMap", 1);
}