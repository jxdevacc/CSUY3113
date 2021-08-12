#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <string>
#include <random>

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "util.h"
#include "Entity.h"
#include <vector>
#include "map.h"

#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "menu.h"

GLuint fontTextureID;
SDL_Window* displayWindow;
bool gameIsRunning = true;

Mix_Music* music;
Mix_Chunk* bounce;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[3];

int lives = 3;
float lifePosX = 0.0;
float lifePosY = 0.0;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Platformer!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("calm.mp3");
    Mix_PlayMusic(music, -1);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontTextureID = Util::LoadTexture("font1.png");

    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    SwitchToScene(sceneList[0]);

}

void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {

            if (currentScene->state.currScene < 1) {
                case SDLK_RETURN:
                    currentScene->state.nextScene = 1;
            }
            else {
                case SDLK_LEFT:
                    // Move the player left
                    break;

                case SDLK_RIGHT:
                    // Move the player right
                    break;

                case SDLK_SPACE:

                    break;
                }

            }


            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -3.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 3.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.player->movement.y = 3.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.player->movement.y = -3.0f;
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, -currentScene->state.player->position.y, 0));
    lifePosX = currentScene->state.player->position.x - 4.5;
    lifePosY = currentScene->state.player->position.y + 3.25;
}



void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);
    currentScene->state.map->Render(&program);

    if (currentScene->state.player->dead) {
        currentScene->state.player->dead = false;
        lives -= 1;
        SwitchToScene(sceneList[currentScene->state.currScene]);
    }

    currentScene->Render(&program);

    if (currentScene->state.player->state == 2) {
        Util::DrawText(&program, fontTextureID, "Mission Accomplished!", 1.0, -0.6f, glm::vec3(9, -3, 0));
        currentScene->state.player->isActive = false;
    }

    if (lives <= 0) {
        Util::DrawText(&program, fontTextureID, "Mission Failed!", 1.0, -0.6f, glm::vec3(2, -3, 0));
        currentScene->state.player->isActive = false;
    }

    Util::DrawText(&program, fontTextureID, "Lives: " + std::to_string(currentScene->state.secs) + std::to_string(currentScene->state.active) + std::to_string(currentScene->state.spawned), 0.75, -0.5f, glm::vec3(lifePosX, lifePosY, 0));

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();

        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);

        Render();

    }

    Shutdown();
    return 0;
}