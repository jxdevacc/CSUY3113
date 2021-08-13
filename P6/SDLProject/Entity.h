#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "map.h"

enum EntityType {PLAYER, PLATFORM, ENEMY, SWORD, BACKGROUND};

enum AIType { WALKER, WAITANDGO, JUMPER };
enum AIState { IDLE, WALKING, ATTACKING};

class Entity {
public:

    EntityType entityType;
    AIType aiType;
    AIState aiState;

    bool dead = false;
    bool invincible = false;
    float invincibleTimer = 0;
    bool isAttacking = false;
    float attackCooldown = 0;
    float attackDuration = 0;
    float attackSpeed = 0;
    int state = 0; //0 = nothing 1 = win 2 = lose
    int kills = 0;
    int money = 0;
    int health = 0;
    int maxHealth = 100;
    int damage = 0;

    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;

    glm::vec3 sensorRight;
    glm::vec3 sensorLeft;

    float width = 1;
    float height = 1;

    bool jump = false;
    float jumpPower = 0;

    float speed;

    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animRight = NULL;
    int* animLeft = NULL;
    int* animUp = NULL;
    int* animDown = NULL;

    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;

    bool isActive = true;

    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;

    Entity();

    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount); 
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsX(Map* map);
    void CheckCollisionsY(Map* map);

    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);

    void AI(Entity* player);
    void AIWalker();
    void AIJumper();
    void AIWaitAndGo(Entity *player);
};