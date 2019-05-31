#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
Camera* Camera::instance = NULL;

// ���е�ǰλ�õļ���(�����ڼ�) ��ΪCamera include PhysicsEngine�࣬���Բ��ܻ���include
void Camera::UpdatePositionEachSecond(float deltaTime) {
    PhysicsEngine* engine = PhysicsEngine::getInstance();
    //cout << "Height:" << (Position.y) << "Speed" << (engine->currentSpeed) << endl;
    /* cout << Position.y << endl;*/
    
    if (engine->isJumping) {
        // �����������ٶȲ��� (v + v0)/ 2 * t
        if (engine->currentSpeed > -(engine->initialSpeed)) {
            float nowSpeed = engine->currentSpeed + engine->gravityFactor * deltaTime * engine->slowingdownFactor;
            // ��ǰ�߹���·��
            engine->currentHeight = (nowSpeed + engine->currentSpeed) / 2.0f * deltaTime;
            // if (engine->currentHeight < 0.0f && engine->currentHeight > -0.1f) engine->currentHeight = 0.0f;
            // cout << "Height:" << (engine->currentHeight) << "Speed" << (engine->currentSpeed) << endl;
            engine->currentSpeed = nowSpeed;
            Position = glm::vec3(Position.x, Position.y + engine->currentHeight, Position.z);
        }
        else {
            engine->isJumping = false;
            engine->currentSpeed = 0.0f;
            Position = glm::vec3(Position.x, round(Position.y), Position.z);
        }
    }
}

void Camera::ProcessKeyboard(Direction direction, float deltaTime)
{
    PhysicsEngine* engine = PhysicsEngine::getInstance();

    // ͨ���ƶ��ٶ��Լ�����ʱ�����ٶ�engine-
    float cameraSpeed = MovementSpeed * deltaTime;
    if (direction == FLYSKY) {
        if (!flysky)cout << "����ģʽ�򿪣�" << endl;
        else cout << "����ģʽ�رգ�" << endl;
        flysky = !flysky;
        return;
    }
    glm::vec3 afterMove(1.0f);
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
    if (direction == FORWARD_LEFT) {
        forwardVector += 2.0f;
        rightVector -= 2.0f;
    }
    if (direction == FORWARD_RIGHT) {
        forwardVector += 2.0f;
        rightVector += 2.0f;
    }
    if (direction == BACKWARD_LEFT) {
        forwardVector -= 2.0f;
        rightVector -= 2.0f;
    }
    if (direction == BACKWARD_RIGHT) {
        forwardVector -= 2.0f;
        rightVector += 2.0f;
    }
    if (forwardVector != 0.0f || rightVector != 0.0f) {
        //���߲��ı�y������
        glm::mat4 vm = getViewMatrix();
        glm::vec3 forward = glm::vec3(vm[0][2], 0.0f, vm[2][2]);
        glm::vec3 strafe = glm::vec3(vm[0][0], 0.0f, vm[2][0]);

        afterMove = Position + (-forwardVector * forward + rightVector * strafe) * cameraSpeed;
        if (PhysicsEngine::getInstance()->CollisionDetect(afterMove)) {
            Position = afterMove;
        }

    }
    if (!flysky && direction == JUMP && !engine->isJumping) {
        engine->InitJumping();
    }
    if(flysky && direction == JUMP)
        Position = glm::vec3(Position.x, Position.y + 0.01f, Position.z);
}