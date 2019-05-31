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
    JUMP,
    FLYSKY
};

/*��������������*/
class Camera {
public:

    // ����ֵ
    // �����λ��
    glm::vec3 Position = glm::vec3(0.0f, 1.0f, 0.0f);
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
    void ProcessKeyboard(Direction direction, float deltaTime)
    {
        PhysicsEngine* engine = PhysicsEngine::getInstance();

        // ͨ���ƶ��ٶ��Լ�����ʱ�����ٶ�engine-
        float cameraSpeed = MovementSpeed * deltaTime;
        if (direction == FLYSKY) {
            if(!flysky)cout << "����ģʽ�򿪣�" << endl;
            else cout << "����ģʽ�رգ�" << endl;
            flysky = !flysky;
            return;
        }
        // ����ģʽ�Ƿ���
        if (flysky) {
            glm::vec3 afterMove;
            // �������ַ�����ж�
            if (direction == FORWARD)
                afterMove = moveForward(Front * cameraSpeed);
            if (direction == BACKWARD)
                afterMove = moveBack(Front * cameraSpeed);
            if (direction == LEFT)
                afterMove = moveLeft(glm::normalize(glm::cross(Front, Up)) * cameraSpeed);
            if (direction == RIGHT)
                afterMove = moveRight(glm::normalize(glm::cross(Front, Up)) * cameraSpeed);
            if (direction == JUMP)
                afterMove = glm::vec3(Position.x, Position.y + 0.01f, Position.z);
            Position = afterMove;
        }
        else {
            float forwardVector = 0.0f, rightVector = 0.0f;
            // �������ַ�����ж�
            if (direction == FORWARD)
                forwardVector += 2.0f;
            if (direction == BACKWARD)
                forwardVector -= 2.0f;
            if (direction == LEFT)
                rightVector -= 2.0f;
            if (direction == RIGHT)
                rightVector += 2.0f;
            if (forwardVector != 0.0f || rightVector != 0.0f) {
                //���߲��ı�y������
                glm::mat4 vm = getViewMatrix();
                glm::vec3 forward = glm::vec3(vm[0][2], 0.0f, vm[2][2]);
                glm::vec3 strafe = glm::vec3(vm[0][0], 0.0f, vm[2][0]);

                Position += (-forwardVector * forward + rightVector * strafe) * cameraSpeed;
                targetPos = Position + (-forwardVector * forward + rightVector * strafe) * 1.5f;

            }
            if (direction == JUMP && !engine->isJumping) {
                engine->InitJumping();
            }
        }
        
        
    }
    // ���е�ǰλ�õļ���(�����ڼ�)
    void UpdatePositionEachSecond(float deltaTime) {
       /* cout << Position.y << endl;*/
        PhysicsEngine* engine = PhysicsEngine::getInstance();
        if (engine->isJumping) {
            // �����������ٶȲ��� (v + v0)/ 2 * t
            if ((engine->currentSpeed > 0.0f && engine->currentHeight == 0.0f) || engine->currentHeight >= 0.0f) {
                float nowSpeed = engine->currentSpeed + engine->gravityFactor * deltaTime;
                engine->currentHeight += (nowSpeed + engine->currentSpeed) / 2.0f * deltaTime;
                if (engine->currentHeight < 0.0f && engine->currentHeight > -0.1f) engine->currentHeight = 0.0f;
                cout << "Height:" << (engine->currentHeight)<<"Speed"<<(engine->currentSpeed) << endl;
                engine->currentSpeed = nowSpeed;
                // �ж��Ƿ��������׶Σ�������������Ӧ�߶ȣ������ȥ
                if (engine->currentSpeed >= 0) {
                    Position = glm::vec3(Position.x, Position.y + engine->currentHeight, Position.z);
                }
                else {
                    Position = glm::vec3(Position.x, Position.y - engine->currentHeight, Position.z);
                }
            }
            else if (engine->currentSpeed <= -(engine->initialSpeed)) {
                engine->isJumping = false;
                Position = glm::vec3(Position.x, floor(Position.y - engine->currentHeight), Position.z);
             }
        }
    }
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


private:
    /**
    * @brief ǰ���ƶ�
    * @param distance �ƶ�����
    **/
    glm::vec3 moveForward(glm::vec3 distance) {
        return Position + distance;
    }

    /**
    * @brief ǰ���ƶ�
    * @param distance �ƶ�����
    **/
    glm::vec3 moveBack(glm::vec3 distance) {
        return Position - distance;
    }

    /**
    * @brief ǰ���ƶ�
    * @param distance �ƶ�����
    **/
    glm::vec3 moveRight(glm::vec3 distance) {
        return Position + distance;
    }

    /**
    * @brief ǰ���ƶ�
    * @param distance �ƶ�����
    **/
    glm::vec3 moveLeft(glm::vec3 distance) {
        return Position - distance;
    }

    static Camera* instance;
};


#endif