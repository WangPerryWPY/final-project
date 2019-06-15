#pragma once
#include <iostream>
#include "Initial.h"
#include "Shader.h"
#include <vector>
#include <string>
#include <map>
#include "Model.h"
using namespace std;
#define MAX_BLOCK 6400

/*������Դ�������续���������*/
class ResourceManager {
public:

     static ResourceManager* getInstance() {
        if (instance == NULL) {
            instance = new ResourceManager();
        }
        return instance;
    }
    ResourceManager() {
    }
    ~ResourceManager() {
        for (auto iter : multipleTextureID) {
            pair<int, unsigned int*> p = iter.second;
            delete(p.second);
        }
    }
    // ----------������-----------------------
    // ��������
    enum BlockType {
        GRASS, BRICK, DIRT, WATER, Tree_birch, Tree_oak, Tree_jungle, Leave_oak, Leave_birch, Leave_jungle,
        MUSHROOM, FLOWER,VINE,STONE,
    };
	enum ModelType {
		rose, spider
	};
    // ��������
    unsigned int loadTexture(const GLchar* path);
    // ���ض���ͼƬ������ˮ
    unsigned int* loadTextures(const GLchar* path, int count);
	//����ģ��
	Model& LoadModel(const GLchar* file, std::string name);
	Model& GetModel(std::string name);
	Model loadModelFromFile(const GLchar* file);
    void setAllTexture() {
        textureID["grass_side"] = loadTexture("resource/blocks/grass_path_side.png");
        textureID["grass_top"] = loadTexture("resource/blocks/grass_path_top.png");
        textureID["dirt"] = loadTexture("resource/blocks/dirt.png");
        textureID["brick"] = loadTexture("resource/blocks/brick.png");
        multipleTextureID["water"].first = 32;
        multipleTextureID["water"].second = loadTextures("resource/blocks2/water_still.png", 32);
        // ����
        textureID["tree_oak_top"] = loadTexture("resource/blocks/log_oak_top.png");
        textureID["tree_oak_side"] = loadTexture("resource/blocks/log_oak.png");
        textureID["tree_birch_top"] = loadTexture("resource/blocks/log_birch_top.png");
        textureID["tree_birch_side"] = loadTexture("resource/blocks/log_birch.png");
        textureID["tree_jungle_top"] = loadTexture("resource/blocks/log_spruce_top.png");
        textureID["tree_jungle_side"] = loadTexture("resource/blocks/log_spruce.png");
        // ��Ҷ �����ɫ50aa55
        textureID["leave_birch"] = loadTexture("resource/blocks/leaves_birch.png");
        textureID["leave_jungle"] = loadTexture("resource/blocks/leaves_jungle.png");
        textureID["leave_oak"] = loadTexture("resource/blocks/leaves_oak.png");
        // Ģ��
        textureID["mushroom"] = loadTexture("resource/blocks/mushroom_red.png");
		textureID["flower"] = loadTexture("resource/blocks/flower_oxeye_daisy.png");
		textureID["vine"] = loadTexture("resource/blocks/end_stone.png");
		textureID["stone"] = loadTexture("resource/blocks/end_stone.png");
		Models["rose"] = LoadModel("resource/rose/Models and Textures/rose.obj","rose");
    }
    //----------------��Ⱦ----------------------------
    // ѡ����Ⱦ�ķ���
    enum RenderDirection
    {
        LEFT, RIGHT, TOP, BOTTOM, BACK, FRONT, CROSS
    };
    // ��whileѭ���з������Ⱦ(ÿ������Բ�ͬ����)
    void RenderScene(Shader &shader, BlockType blockType, unsigned int drawCount = 1);
	void RenderModelScene(Shader &shader, ModelType modelType, unsigned int drawCount =1);

    // ��Ⱦ��������
    void RenderCube(unsigned int texture, unsigned int drawCount = 1);
    // ���з���ĳ�ʼ��
    void InitCube() {
        // ���֧��40 * 40
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_BLOCK, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        Single_InitCube(cubeVAO_top, cubeVBO_top, TOP);
        Single_InitCube(cubeVAO_bottom, cubeVBO_bottom, BOTTOM);
        Single_InitCube(cubeVAO_front, cubeVBO_front, FRONT);
        Single_InitCube(cubeVAO_back, cubeVBO_back, BACK);
        Single_InitCube(cubeVAO_left, cubeVBO_left, LEFT);
        Single_InitCube(cubeVAO_right, cubeVBO_right, RIGHT);
        Single_InitCube(crossVAO, crossVBO, CROSS);
    }

    // ���е�������İ�
    void Single_InitCube(unsigned int& VAO, unsigned int &VBO, RenderDirection dir){
        // top
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // ���ݷ�����Ⱦ
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        if (dir == TOP) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(top), top, GL_STATIC_DRAW);
        }
        else if (dir == BOTTOM) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(bottom), bottom, GL_STATIC_DRAW);
        }
        else if (dir == FRONT) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(front), front, GL_STATIC_DRAW);
        }
        else if (dir == BACK) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(back), back, GL_STATIC_DRAW);
        }
        else if (dir == LEFT) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(left), left, GL_STATIC_DRAW);
        }
        else if (dir == RIGHT) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(right), right, GL_STATIC_DRAW);
        }
        else if (dir == CROSS) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(cross), cross, GL_STATIC_DRAW);
        }
        // Link vertex attributes
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glVertexAttribDivisor(3, 1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
       // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    //  ��Ⱦ����һ��
    void RenderFace(unsigned int texture, RenderDirection dir, unsigned int drawCount);

    //----------��պ�-------------------------
    // ������պе�����
    unsigned int loadCubemap(vector<std::string> faces);
    // ������պе���Ⱦ׼��
    void InitSky();
    // ������պе���Ⱦ
    void RenderSky(glm::mat4 projection);
    unsigned int getInstanceVBO() {
        return instanceVBO;
    }


    //void LoadTextureSplit(const GLchar* file, int count);
private:
    // ���ʺͶ�Ӧ��id
    map<string, unsigned int> textureID;
	//ģ��
	map<string, Model> 		Models;
    // ������Ϊ�����Ĳ���
    map<string, pair<int, unsigned int*> > multipleTextureID;
    unsigned int instanceVBO;
    static ResourceManager* instance;
    // --------------��Ⱦ����-----------------------
    GLfloat front[48] = {
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // top-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // top-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom-left
    };
    GLfloat back[48] = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // bottom-left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // bottom-right         
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,// top-left      
    };
    GLfloat left[48] = {
        // Left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // bottom-right
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right

    };
    GLfloat right[48] = {
        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right         
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // top-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left     

    };
    GLfloat bottom[48] = {
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,// bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-right

    };

    GLfloat top[48] = {
        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// top-left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top-right     
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // bottom-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// top-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f // bottom-left        
    };

    GLfloat cross[96] = {
        // Back
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
        // Front
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 
    };

    // ------------- ���鲿��-------------------------
    unsigned int cubeVBO_top, cubeVBO_bottom, cubeVBO_left, cubeVBO_right, cubeVBO_front, cubeVBO_back, crossVAO;
    unsigned int cubeVAO_top, cubeVAO_bottom, cubeVAO_left, cubeVAO_right, cubeVAO_front, cubeVAO_back, crossVBO;
    //----------------��պв���-----------------------------
    unsigned int skyboxVAO, skyboxVBO;
    float skyboxVertices[108] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    vector<string> skybox_faces
    {
        "resource/skybox/nevada_lf.jpg",
        "resource/skybox/nevada_rt.jpg",
        "resource/skybox/nevada_up.jpg",
        "resource/skybox/nevada_dn.jpg",
        "resource/skybox/nevada_ft.jpg",
        "resource/skybox/nevada_bk.jpg"
    };
    unsigned int cubemapTexture;
    Shader skyboxShader;
};