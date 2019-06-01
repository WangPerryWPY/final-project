#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
Camera* Camera::instance = NULL;

// ���е�ǰλ�õļ���(�����ڼ�) ��ΪCamera include PhysicsEngine�࣬���Բ��ܻ���include
void Camera::UpdatePositionEachSecond(float deltaTime) {
    PhysicsEngine* engine = PhysicsEngine::getInstance();
    //cout << "Height: " << (Position.y) << "Speed: " << (engine->currentSpeed) << endl;
    if (engine->isJumping && !engine->isFreeAll) {
        // ��������ֻ���������׶�
        // �����������ٶȲ��� (v + v0)/ 2 * t
        if (engine->currentSpeed > 0) {
            float nowSpeed = engine->currentSpeed + engine->gravityFactor * deltaTime * engine->slowingdownFactor;
            // ��ǰ�߹���·��
            engine->currentHeight = (nowSpeed + engine->currentSpeed) / 2.0f * deltaTime;
            // if (engine->currentHeight < 0.0f && engine->currentHeight > -0.1f) engine->currentHeight = 0.0f;
            
            engine->currentSpeed = nowSpeed;
            glm::vec3 afterMove = glm::vec3(Position.x, Position.y + engine->currentHeight, Position.z);
            //cout << "Height: " << (afterMove.y) << "  Speed: " << (engine->currentSpeed) << endl;
            if (engine->UpVerticalCollisionDetect(afterMove)) {
                Position = afterMove;
            }
            else {
                FreeAll();
            }
        }
        else {
            FreeAll();
        }
    }
    else if (engine->isFreeAll) {
        float nowSpeed = engine->currentSpeed + engine->gravityFactor * deltaTime * engine->slowingdownFactor;
        // ��ǰ�߹���·��
        engine->currentHeight = (nowSpeed + engine->currentSpeed) / 2.0f * deltaTime;
        // if (engine->currentHeight < 0.0f && engine->currentHeight > -0.1f) engine->currentHeight = 0.0f;

        engine->currentSpeed = nowSpeed;
        glm::vec3 afterMove = glm::vec3(Position.x, Position.y + engine->currentHeight, Position.z);
        if (engine->DownVerticalCollisionDetect(afterMove)) {
            Position = afterMove;
        }
        else {
            engine->isFreeAll = false;
            if (engine->isJumping) {
                engine->isJumping = false;
            }
            engine->currentHeight = 0.0f;
            engine->currentSpeed = 0.0f;
            Position = glm::vec3(Position.x, round(Position.y), Position.z);
        }
    }
}

void Camera::FreeAll() {
    PhysicsEngine* engine = PhysicsEngine::getInstance();
    engine->currentHeight = 0.0f;
    engine->currentSpeed = 0.0f;
    engine->isFreeAll = true;

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
    // �������һ�̽��д����Ա�������ײ����������
    if (engine->isJumping && engine->currentSpeed <= -(engine->initialSpeed)) return;
    // �������ַ�����ж�
    float speed = 2.0f;
    if (direction == FORWARD)
        forwardVector += speed;
    if (direction == BACKWARD)
        forwardVector -= speed;
    if (direction == LEFT)
        rightVector -= speed;
    if (direction == RIGHT)
        rightVector += speed;
    if (direction == FORWARD_LEFT) {
        forwardVector += speed;
        rightVector -= speed;
    }
    if (direction == FORWARD_RIGHT) {
        forwardVector += speed;
        rightVector += speed;
    }
    if (direction == BACKWARD_LEFT) {
        forwardVector -= speed;
        rightVector -= speed;
    }
    if (direction == BACKWARD_RIGHT) {
        forwardVector -= speed;
        rightVector += speed;
    }
    if (forwardVector != 0.0f || rightVector != 0.0f) {
        //���߲��ı�y������
        glm::mat4 vm = getViewMatrix();
        glm::vec3 forward = glm::vec3(vm[0][2], 0.0f, vm[2][2]);
        glm::vec3 strafe = glm::vec3(vm[0][0], 0.0f, vm[2][0]);

        afterMove = Position + (-forwardVector * forward + rightVector * strafe) * cameraSpeed;
        // ����С�������ټ����� 0.001
        int remain = 3;
        if (abs(afterMove.x - Position.x) < 0.05f) {
            afterMove.x = Position.x;
        }
        if (abs(afterMove.z - Position.z) < 0.05f) {
            afterMove.z = Position.z;
        }
       // afterMove = glm::vec3(getFloat(afterMove.x, remain), getFloat(afterMove.y, remain), getFloat(afterMove.z, remain));
       cout << "Pos:"<<Position.x << " " << Position.y << " " << Position.z << endl;
       cout << "move:" << afterMove.x << " " << afterMove.y << " " << afterMove.z << endl;
       //afterMove = glm::vec3((int)afterMove.x, (int)afterMove.y, (int)afterMove.z);
        if (PhysicsEngine::getInstance()->HorizontalCollisionDetect(Position, afterMove)) {
            if (!engine->isFreeAll && !engine->isJumping && !flysky) {
                if (engine->WalkingVerticalCollisionDetect(afterMove)) {
                    //// ����λ����
                    //float offset = 0.5f;
                    //int axBiggerthanPx = 0;
                    //int azBiggerthanPz = 0;
                    //if (afterMove.x < Position.x) {
                    //    axBiggerthanPx = -1;
                    //}
                    //else if (afterMove.x > Position.x) {
                    //    axBiggerthanPx = 1;
                    //}
                    //if (afterMove.z < Position.z) {
                    //    azBiggerthanPz = -1;
                    //}
                    //else if (afterMove.z > Position.z) {
                    //    azBiggerthanPz = 1;
                    //}
                    //float x_offset = axBiggerthanPx > 0 ? 1.0f : (axBiggerthanPx == 0 ? 0.0f : -1.0f);
                    //float z_offset = azBiggerthanPz > 0 ? 1.0f : (azBiggerthanPz == 0 ? 0.0f : -1.0f);

                    // Position = glm::vec3(afterMove.x + x_offset, Position.y, afterMove.z + z_offset);
                    Position = glm::vec3(afterMove.x, Position.y, afterMove.z);
                    FreeAll();
                    return;
                }
            }
            Position = afterMove;
        }
    }
    if (!flysky && direction == JUMP && !engine->isJumping) {
        engine->InitJumping();
    }
    if (flysky && direction == JUMP) {
        afterMove = glm::vec3(Position.x, Position.y + 0.05f, Position.z);
        if (engine->UpVerticalCollisionDetect(afterMove)) {
            Position = afterMove;
        }
    }
    else if (flysky && direction == DOWN) {
        afterMove = glm::vec3(Position.x, Position.y - 0.05f, Position.z);
        if (engine->DownVerticalCollisionDetect(afterMove)) {
            Position = afterMove;
        }
    }
}