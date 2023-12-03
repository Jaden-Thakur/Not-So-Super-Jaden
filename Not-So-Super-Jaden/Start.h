#pragma once
#include "Scene.h"

class Start : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 1;

    // ————— CONSTRUCTOR ————— //
    ~Start();

    // ————— METHODS ————— //
    void initialize() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};
