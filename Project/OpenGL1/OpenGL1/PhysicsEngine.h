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

// ͨ�����п��ܵ�����ƶ����������ͼ�ı仯
enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    FORWARD_LEFT,
    FORWARD_RIGHT,
    BACKWARD_LEFT,
    BACKWARD_RIGHT,
    JUMP,
    FLYSKY
};

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
    bool UpVerticalCollisionDetect(glm::vec3 pos);
    bool DownVerticalCollisionDetect(glm::vec3 pos);
    bool WalkingVerticalCollisionDetect(glm::vec3 pos);
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
    bool isFreeAll;
    // ����map ֻ�Ը߶Ƚ��б���
    // y x z
    map<int, map<int, map<int, bool> > > m;

private:

    static PhysicsEngine* instance;
};
