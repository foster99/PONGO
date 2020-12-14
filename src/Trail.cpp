#include "Trail.h"
#include "GameScene.h"

Trail::Trail(GameScene* scene)
{
    this->scene = scene;
    this->clear();

    selectedList = true;

    trailShader = new ShaderProgram();
    Scene::loadShaders("trailShader", trailShader);

    trailCube = new Model();
    trailCube->loadFromFile("models/snake.obj", *trailShader);
}

void Trail::addPoint(vec2 point)
{
    if (selectedList) future_positions_1.push_back(point);
    else              future_positions_2.push_back(point);
}

void Trail::commitPositions()
{
    if (selectedList)
    {
        for (vec2 pos : future_positions_2)
        {
            positions.push_back(pos);
        }
        selectedList = false;
        future_positions_2 = vector<vec2>(0);
    }
    else
    {
        for (vec2 pos : future_positions_1)
        {
            positions.push_back(pos);
        }
        selectedList = true;
        future_positions_1 = vector<vec2>(0);
    }

    timeToChange = totalTime;
}

void Trail::update(int deltaTime)
{
    timeToChange -= deltaTime;
    if (timeToChange < 0)
        commitPositions();

    viewMatrix = scene->getViewMatrix();
    projMatrix = scene->getProjMatrix();
}

void Trail::render()
{
    const float tileSize(scene->getLevel()->getTileSize());
    mat4 modelMatrix;
    mat3 normalMatrix;
    vec3 scaleFactor = vec3(0.5f, 0.5f, 0.25f) * tileSize / trailCube->getHeight();

    for (vec2 pos : positions)
    {
        modelMatrix = mat4(1);
        modelMatrix = translate(modelMatrix, vec3(pos, 0.f));
        modelMatrix = rotate(modelMatrix, PI / 4.f, vec3(0.f, 0.f, 1.f));
        modelMatrix = scale(modelMatrix, scaleFactor);
        modelMatrix = translate(modelMatrix, -trailCube->getCenter());

        // Compute NormalMatrix
        normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));

        // Set uniforms
        trailShader->use();
        trailShader->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
        trailShader->setUniformMatrix4f("projection", mat4(projMatrix));
        trailShader->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
        trailShader->setUniformMatrix3f("normalmatrix", normalMatrix);

        // Render
        trailCube->render(*trailShader);
    }

    for (vec2 pos : future_positions_1)
    {
        modelMatrix = mat4(1);
        modelMatrix = translate(modelMatrix, vec3(pos, 0.f));
        modelMatrix = rotate(modelMatrix, PI / 4.f, vec3(0.f, 0.f, 1.f));
        modelMatrix = scale(modelMatrix, scaleFactor);
        modelMatrix = translate(modelMatrix, -trailCube->getCenter());

        // Compute NormalMatrix
        normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));

        // Set uniforms
        trailShader->use();
        trailShader->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
        trailShader->setUniformMatrix4f("projection", mat4(projMatrix));
        trailShader->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
        trailShader->setUniformMatrix3f("normalmatrix", normalMatrix);

        // Render
        trailCube->render(*trailShader);
    }

    for (vec2 pos : future_positions_2)
    {
        modelMatrix = mat4(1);
        modelMatrix = translate(modelMatrix, vec3(pos, 0.f));
        modelMatrix = rotate(modelMatrix, PI / 4.f, vec3(0.f, 0.f, 1.f));
        modelMatrix = scale(modelMatrix, scaleFactor);
        modelMatrix = translate(modelMatrix, -trailCube->getCenter());

        // Compute NormalMatrix
        normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));

        // Set uniforms
        trailShader->use();
        trailShader->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
        trailShader->setUniformMatrix4f("projection", mat4(projMatrix));
        trailShader->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
        trailShader->setUniformMatrix3f("normalmatrix", normalMatrix);

        // Render
        trailCube->render(*trailShader);
    }
}

void Trail::clear()
{
    timeToChange        = totalTime;
    positions           = vector<vec2>(0);
    future_positions_1  = vector<vec2>(0);
    future_positions_2  = vector<vec2>(0);
}

bool Trail::ballCollidedWithTrail()
{
    float xmin, xmax, ymin, ymax;
    const float offset(scene->getLevel()->getTileSize() / 2.f);
    const vec2 ballCenter(scene->getPlayerPos());

    for (vec2 center : positions)
    {
        xmin = center.x - offset;
        xmax = center.x + offset;
        ymin = center.y - offset;
        ymax = center.y + offset;

        if (xmin < ballCenter.x && ballCenter.x < xmax &&
            ymin < ballCenter.y && ballCenter.y < ymax)
            return true;
    }

    return false;
}
