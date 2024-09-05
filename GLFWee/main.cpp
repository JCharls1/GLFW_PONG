#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PADDLE_WIDTH 20.0f
#define PADDLE_HEIGHT 100.0f
#define LEFT_PADDLE_POSITION -WINDOW_WIDTH / 2 + PADDLE_WIDTH / 2 + 50
#define RIGHT_PADDLE_POSITION WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2 - 50
#define STARTING_BALL_VELOCITY 200.0f
float paddleSpeed = 300.0f;  // Pixels per second
float leftPaddleY = 0.0f;    // Y position for the left paddle
float rightPaddleY = 0.0f;   // Y position for the right paddle

// Ball settings
float ballX = 0.0f;          // X position of the ball
float ballY = 0.0f;          // Y position of the ball
float ballVelocityX = STARTING_BALL_VELOCITY; // X velocity of the ball
float ballVelocityY = STARTING_BALL_VELOCITY; // Y velocity of the ball
const float BALL_SIZE = 20.0f; // Ball size
float Timer = 0;

void processInput(GLFWwindow* window, float deltaTime) {
    // Left Paddle Controls (W and S)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        leftPaddleY += paddleSpeed * deltaTime;
        if (leftPaddleY + PADDLE_HEIGHT / 2 > WINDOW_HEIGHT / 2)  // Keep paddle within window
            leftPaddleY = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        leftPaddleY -= paddleSpeed * deltaTime;
        if (leftPaddleY - PADDLE_HEIGHT / 2 < -WINDOW_HEIGHT / 2)  // Keep paddle within window
            leftPaddleY = -WINDOW_HEIGHT / 2 + PADDLE_HEIGHT / 2;
    }

    // Right Paddle Controls (Up and Down Arrow)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        std::cout << "up" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        std::cout << "up" << std::endl;
    }
}

void addSpeedToBall(float deltaTime) {
    Timer += deltaTime;

    if (Timer > 5) {
        ballVelocityX += 100;
        ballVelocityY += 100;
        Timer = 0;
        std::cout << ballVelocityX << std::endl;
        std::cout << ballVelocityY << std::endl;
    }
}

void AI(float deltaTime) {
    if (ballY > rightPaddleY) {
        rightPaddleY += paddleSpeed * deltaTime;
        if (rightPaddleY + PADDLE_HEIGHT / 2 > WINDOW_HEIGHT / 2)
            rightPaddleY = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    }else if (ballY < rightPaddleY) {
        rightPaddleY -= paddleSpeed * deltaTime;
        if (rightPaddleY - PADDLE_HEIGHT / 2 < -WINDOW_HEIGHT / 2)
            rightPaddleY = -WINDOW_HEIGHT / 2 + PADDLE_HEIGHT / 2;
    }
}

void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2, y - height / 2);
    glVertex2f(x + width / 2, y - height / 2);
    glVertex2f(x + width / 2, y + height / 2);
    glVertex2f(x - width / 2, y + height / 2);
    glEnd();
}

void updateBallPosition(float deltaTime) {
    ballX += ballVelocityX * deltaTime;
    ballY += ballVelocityY * deltaTime;

    // Ball collision with top and bottom walls
    if (ballY + BALL_SIZE / 2 > WINDOW_HEIGHT / 2 || ballY - BALL_SIZE / 2 < -WINDOW_HEIGHT / 2) {
        ballVelocityY = -ballVelocityY;  // Reverse Y direction
    }

    // Ball collision with paddles
    if (ballX - BALL_SIZE / 2 < LEFT_PADDLE_POSITION && ballY < leftPaddleY + PADDLE_HEIGHT / 2 && ballY > leftPaddleY - PADDLE_HEIGHT / 2) {
        ballVelocityX = -ballVelocityX;  // Reverse X direction
    }
    if (ballX + BALL_SIZE / 2 > RIGHT_PADDLE_POSITION && ballY < rightPaddleY + PADDLE_HEIGHT / 2 && ballY > rightPaddleY - PADDLE_HEIGHT / 2) {
        ballVelocityX = -ballVelocityX;  // Reverse X direction
    }

    // Ball goes out of bounds
    if (ballX - BALL_SIZE / 2 < -WINDOW_WIDTH / 2 || ballX + BALL_SIZE / 2 > WINDOW_WIDTH / 2) {
        ballX = 0.0f;  // Reset ball to center
        ballY = 0.0f;
        ballVelocityX = STARTING_BALL_VELOCITY;
        ballVelocityY = STARTING_BALL_VELOCITY;
        Timer = 0;
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong Game", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set up OpenGL view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2, -1, 1);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        static float lastFrameTime = 0.0f;
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Input handling
        processInput(window, deltaTime);
        AI(deltaTime);
        addSpeedToBall(deltaTime);

        // Update ball position
        updateBallPosition(deltaTime);

        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw paddles and ball
        drawRectangle(LEFT_PADDLE_POSITION, leftPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT);  // Left paddle
        drawRectangle(RIGHT_PADDLE_POSITION, rightPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT);  // Right paddle
        drawRectangle(ballX, ballY, BALL_SIZE, BALL_SIZE);  // Ball

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
