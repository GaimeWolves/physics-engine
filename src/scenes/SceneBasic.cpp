#include <scenes/SceneBasic.hpp>

#include <random>

#include <util.hpp>
#include <defs.hpp>

#include <physics/Circle.hpp>
#include <physics/Edge.hpp>
#include <physics/Engine.hpp>

static Physics::Engine engine;

static bool mouseDown = false;
static glm::vec2 mousePos;

static std::vector<std::shared_ptr<Physics::Edge>> bowl;
static std::shared_ptr<Physics::Edge> wallL;
static std::shared_ptr<Physics::Edge> wallR;
static std::shared_ptr<Physics::Edge> platL;
static std::shared_ptr<Physics::Edge> platR;
static std::shared_ptr<Physics::Edge> ceiling;

static std::vector<std::shared_ptr<Physics::Circle>> balls;

static std::shared_ptr<Physics::Circle> picked;

static std::default_random_engine gen;

static void createBall(float x, float y)
{
    static auto sDist = std::uniform_real_distribution<float>(0.035f, 0.05f);
    static auto hDist = std::uniform_real_distribution<float>(0.0f, 360.0f);

    float size = sDist(gen);
    float hue = hDist(gen);

    auto ball = std::make_shared<Physics::Circle>(glm::vec2(x, y), size);
    ball->setColor(color::hsl(hue, 0.8f, 0.4f));
    ball->setMass(size * 10);
    ball->setStaticFriction(0.15f);
    ball->setKineticFriction(0.09f);
    balls.push_back(ball);
    engine.addEntity(ball);
}

static void spawnRandomBall()
{
    static auto xDist = std::uniform_real_distribution<float>(-0.8f, 0.8f);
    static auto yDist = std::uniform_real_distribution<float>(0.0f, 0.8f);

    createBall(xDist(gen), yDist(gen));
}

void SceneBasic::init()
{
    reset();
}

void SceneBasic::reset()
{
    engine = Physics::Engine();
    engine.setGravity(glm::vec2(0, -0.5f));

    for (size_t i = 0; i < 100; i++)
        spawnRandomBall();

    glm::vec2 current = {-0.9f, 0.0f};
    glm::vec2 last = current;

    for (float angle = 3 * M_PI / 2; angle > M_PI / 2; angle -= 0.2)
    {
        current = {sin(angle) * 0.9f, cos(angle) * 0.9f};

        auto segment = std::make_shared<Physics::Edge>(last, current);
        segment->setMass(PHYSICS_INF_MASS);
        engine.addEntity(segment);
        bowl.push_back(segment);

        last = current;
    }

    auto segment = std::make_shared<Physics::Edge>(last, glm::vec2(0.9, 0.0f));
    segment->setMass(PHYSICS_INF_MASS);
    engine.addEntity(segment);
    bowl.push_back(segment);

    wallL = std::make_shared<Physics::Edge>(glm::vec2(-0.9f, 0.0f), glm::vec2(-0.9f, 0.9f));
    engine.addEntity(wallL);
    wallL->setMass(PHYSICS_INF_MASS);

    wallR = std::make_shared<Physics::Edge>(glm::vec2(0.9f, 0.0f), glm::vec2(0.9f, 0.9f));
    engine.addEntity(wallR);
    wallR->setMass(PHYSICS_INF_MASS);

    platL = std::make_shared<Physics::Edge>(glm::vec2(0.6f, -0.2f), glm::vec2(0.2f, -0.4f));
    engine.addEntity(platL);
    platL->setMass(PHYSICS_INF_MASS);

    platR = std::make_shared<Physics::Edge>(glm::vec2(-0.6f, -0.2f), glm::vec2(-0.2f, -0.4f));
    engine.addEntity(platR);
    platR->setMass(PHYSICS_INF_MASS);

    ceiling = std::make_shared<Physics::Edge>(glm::vec2(-0.9f, 0.9f), glm::vec2(0.9f, 0.9f));
    engine.addEntity(ceiling);
    ceiling->setMass(PHYSICS_INF_MASS);
}

void SceneBasic::handle_events(SDL_Event event)
{
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            mousePos = glm::vec2(event.button.x, event.button.y);
            mousePos = map(mousePos, glm::vec2(0.0f, 0), glm::vec2(screenSize.x, screenSize.y), glm::vec2(-1.0f, 1.0f), glm::vec2(1.0f, -1.0f));
            mouseDown = true;

            for (auto &ball : balls)
            {
                if (glm::distance(ball->getPosition(), mousePos) < ball->getRadius())
                {
                    picked = ball;
                    return;
                }
            }

            createBall(mousePos.x, mousePos.y);
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT)
            mouseDown = false;

        picked = nullptr;
        break;
    case SDL_MOUSEMOTION:
        if (mouseDown)
            mousePos = glm::vec2(event.motion.x, event.motion.y);
        mousePos = map(mousePos, glm::vec2(0.0f, 0), glm::vec2(screenSize.x, screenSize.y), glm::vec2(-1.0f, 1.0f), glm::vec2(1.0f, -1.0f));

        break;
    }
}

void SceneBasic::update(double dt)
{
    if (picked)
    {
        glm::vec2 vel = (mousePos - picked->getPosition()) / ((float)dt * 5);
        picked->setVelocity(vel);
        picked->setPosition(mousePos);
        picked->wake();
    }

    engine.update(dt, 10);
}

void SceneBasic::render()
{
    engine.render();
}