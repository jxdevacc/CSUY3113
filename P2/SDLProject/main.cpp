#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GL_GLEXT_PROTOTYPES 1

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, projectMatrix,p1Matrix,p2Matrix, ballMatrix;

// Start at 0, 0, 0
glm::vec3 player_position = glm::vec3(0, 0, 0);
// Don’t go anywhere (yet).
glm::vec3 player_movement = glm::vec3(0, 0, 0);

glm::vec3 player2_position, player2_movement = glm::vec3(0, 0, 0);

glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::normalize(glm::vec3(3, 1, 0));

float player_speed = 2.0f;
float ball_speed = 0.0f;

GLuint whiteTextureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load iamge. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Project Two: Pong! ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    viewMatrix = glm::mat4(1.0f);
    p1Matrix = glm::mat4(1.0f);
    p2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    whiteTextureID = LoadTexture("white.jpg");
}

void ProcessInput() {

    player_movement = glm::vec3(0);
    player2_movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                ball_speed = 5.0f;
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_UP]) {
        player2_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        player2_movement.y = -1.0f;
    }

    if (keys[SDL_SCANCODE_S]) {
        player_movement.y = -1.0f;
    }
    else if (keys[SDL_SCANCODE_W]) {
        player_movement.y = 1.0f;
    }

}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
   
    p1Matrix = glm::mat4(1.0f);
    p2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);



    player_position.x = -5.0f;
    player2_position.x = 5.0f;

    // Add (direction * units per second * elapsed time)
    player_position += player_movement * player_speed * deltaTime;
    player2_position += player2_movement * player_speed * deltaTime;
    ball_position += ball_movement * ball_speed * deltaTime;


    if (player_position.y > 2.75f) 
        player_position.y = 2.75f;
    if (player_position.y < -2.75f) 
        player_position.y = -2.75f;
    if (player2_position.y > 2.75f) 
        player2_position.y = 2.75f;
    if (player2_position.y < -2.75f) 
        player2_position.y = -2.75f;

    if (ball_position.y + .25f > 3.75f)
        ball_movement.y = -ball_movement.y;
    if (ball_position.y - .25f < -3.75f)
        ball_movement.y = -ball_movement.y;
    if (ball_position.x + .25f > 5.0f)
        ball_speed = 0.0f;
    if (ball_position.x - .25f < -5.0f)
        ball_speed = 0.0f;

    float x1_dist = fabs(player_position.x - ball_position.x) - ((1.0f + 0.5f) / 2.0f);
    float y1_dist = fabs(player_position.y - ball_position.y) - ((2.0f + 0.5f) / 2.0f);
    float x2_dist = fabs(player2_position.x - ball_position.x) - ((1.0f + 0.5f) / 2.0f);
    float y2_dist = fabs(player2_position.y - ball_position.y) - ((2.0f + 0.5f) / 2.0f);

    if (x1_dist <= 0.0f && y1_dist <= 0.0f) {
        ball_movement.x = -ball_movement.x;
    }
    if (x2_dist <= 0.0f && y2_dist <= 0.0f) {
        ball_movement.x = -ball_movement.x;
    }

    p1Matrix = glm::translate(p1Matrix, player_position);
    p2Matrix = glm::translate(p2Matrix, player2_position);
    ballMatrix = glm::translate(ballMatrix, ball_position);

    p1Matrix = glm::scale(p1Matrix, glm::vec3(1.0f, 2.0f, 1.0f));
    p2Matrix = glm::scale(p2Matrix, glm::vec3(1.0f, 2.0f, 1.0f));
    ballMatrix = glm::scale(ballMatrix, glm::vec3(0.5f, 0.5f, 0.5f));

}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    program.SetModelMatrix(p1Matrix);
    glBindTexture(GL_TEXTURE_2D, whiteTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(p2Matrix);
    glBindTexture(GL_TEXTURE_2D, whiteTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(ballMatrix);
    glBindTexture(GL_TEXTURE_2D, whiteTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

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
        Render();
    }

    Shutdown();
    return 0;
}