#pragma once
#include "Scene.h"

class Level0 : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 0;

    // ————— CONSTRUCTOR ————— //
    ~Level0();

    // ————— METHODS ————— //
    void initialize() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};
