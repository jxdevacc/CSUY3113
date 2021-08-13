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
#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include <string>
#include <random>

struct GameState {
	Map* map;
	Entity* sword;
	Entity* player;
	Entity* enemies;
	GLuint enemyTextureID;
	GLuint bossTextureID;
	int active;
	int spawned;
	int currScene;
	int nextScene;
	float secs;
	float attackTimer;
};
class Scene {
public:
	GameState state;
	virtual void Initialize(Entity* player = NULL) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ShaderProgram* program) = 0;
};