#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <map>
using namespace std;


class PhysicsEngine {
public:
    PhysicsEngine();

    static PhysicsEngine* getInstance() {
        if (instance == NULL) instance = new PhysicsEngine();
        return instance;
    }

    void InitJumping() {
        isJumping = true;
        currentSpeed = initialSpeed;
        currentHeight = 0;
    }

    bool HorizontalCollisionDetect(glm::vec3 pos);
    bool VerticalCollisionDetect(glm::vec3 pos, float& downY);
    //------��ײ������-------
    int threshold;
    int max_threshold = 2;

    //----------�������ϵ�� ----------------------------
    float currentHeight;
    float initialSpeed;
    float currentSpeed;
    float slowingdownFactor;
    float gravityFactor;
    bool isJumping;
    // ����map ֻ�Ը߶Ƚ��б���
    map<int, vector<glm::vec3> > m;

private:

    static PhysicsEngine* instance;
};
