#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "PhysicsEngine.h"

#define roomSizeX 200.f
#define roomSizeY 60.f
#define roomSizeZ 200.f
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

/*��������������*/
class Camera {
public:

    // ����ֵ
    // �����λ��
    glm::vec3 Position = glm::vec3(0.0f, 2.0f, 0.0f);
    // �����ǰ����
    glm::vec3 Front = glm::vec3(1.0f, 0.0f, 0.0f);
    // �����������
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    // �����������
    glm::vec3 Right;
    // �۲�����
    glm::vec3 WorldUp = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 targetPos = glm::vec3(5.0f, 0.0f, 0.0f);

    // �ж��Ƿ����ģʽ
    bool flysky = false;
    // ŷ����
    float Yaw = -90.0f;
    float Pitch = 0.0f;

    // �����ж�������λ��
    float lastX;
    float lastY;
    float fov = 45.0f;
    // �Ƿ��һ���ƶ����
    bool firstMouse = true;

    // �����ѡ��
    float MovementSpeed = 3.0f;
    // ������ж�
    float MouseSensitivity = 0.1f;
    // ���ű���
    float Zoom = 60.0f;

    static Camera* getInstance() {
        if (instance == NULL) {
            instance = new Camera();
        }
        return instance;
    }

    /**
    * @brief ͨ����õķ�����ж����Ĵ���
    * @param direction ����ķ���
    * @param deltaTIme �����ʱ��
    */
    void ProcessKeyboard(Direction direction, float deltaTime);

    // ���е�ǰλ�õļ���(�����ڼ�)
    void UpdatePositionEachSecond(float deltaTime);
    /**
    * @brief ����ƶ��Ӷ��ƶ���ͼ
    * @param xoffset x����ƫ����
    * @param yoffset y����ƫ����
    */
    void lookAround(float xoffset, float yoffset)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
    }
    
    /**
    * @brief ����������λ��
    * @param x x����
    * @param y y����
    */
    void setLastXY(float x, float y) {
        lastX = x;
        lastY = y;
    }

    void setPosition(glm::vec3 newPos) {
        this->Position = newPos;
    }

    void setFront(glm::vec3 front) {
        this->Front = front;
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // ��������λС��
    static float getFloat(float input, int n) {

        return (float)(round(input * pow(10, n))) / (float)(pow(10, n));
    }


private:
    /**
    * @brief ǰ���ƶ�
    * @param distance �ƶ�����
    **/
    glm::vec3 moveForward(glm::vec3 distance, glm::vec3 pos) {
        return pos + distance;
    }

    /**
    * @brief ǰ���ƶ�
    * @param distance �ƶ�����
    **/
    glm::vec3 moveBack(glm::vec3 distance, glm::vec3 pos) {
        return pos - distance;
    }

    /**
    * @brief ǰ���ƶ�
    * @param distance �ƶ�����
    **/
    glm::vec3 moveRight(glm::vec3 distance, glm::vec3 pos) {
        return pos + distance;
    }

    /**
    * @brief ǰ���ƶ�
    * @param distance �ƶ�����
    **/
    glm::vec3 moveLeft(glm::vec3 distance, glm::vec3 pos) {
        return pos - distance;
    }

    static Camera* instance;
};


#endif