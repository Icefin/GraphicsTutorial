#pragma once
#include <Gengine/SpriteBatch.h>
#include <Gengine/GLSLProgram.h>
#include <vector>
#include <memory>
#include "Ball.h"

// Ball renderer interface
class BallRenderer {
public:
    virtual void renderBalls(Gengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
        const glm::mat4& projectionMatrix);
protected:
    std::unique_ptr<Gengine::GLSLProgram> m_program = nullptr;
};

// Visualizes kinetic energy
class MomentumBallRenderer : public BallRenderer {
public:
    virtual void renderBalls(Gengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
        const glm::mat4& projectionMatrix) override;
};

// Visualizes positive X component of velocity, as well as position
class VelocityBallRenderer : public BallRenderer {
public:
    VelocityBallRenderer(int screenWidth, int screenHeight);

    virtual void renderBalls(Gengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
        const glm::mat4& projectionMatrix) override;
private:
    int m_screenWidth;
    int m_screenHeight;
};