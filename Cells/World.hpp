//
//  World.hpp
//  Cells
//
//  Created by Arthur Sun on 2/13/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include "setup.hpp"

class World
{
    
protected:
    
    DoubleTexture* position;
    DoubleTexture* velocity;
    DoubleTexture* weights;
    DoubleTexture* divergence;
    DoubleTexture* grid;
    DoubleTexture* dgrid;
    Texture* marks;
    
    Shader* drawer;
    Shader* solver;
    Shader* recter;
    Shader* weighter;
    Shader* stepper;
    Shader* setter;
    Shader* partToGrid;
    Shader* diver;
    Shader* gridSolver;
    Shader* marker;
    Shader* waverager;
    Shader* setter2;
    Shader* advector;
    Shader* extor;
    
    glm::vec2 simSize;
    glm::vec2 gridSize;
    int partRoot;
    int partCap;
    
    GLuint VAO;
    GLuint VBO;
    
public:
    
    int count;
    
    World(int sx, int sy, int pr, float u) : simSize(sx, sy), gridSize(sx + 1.0f, sy + 1.0f),  partRoot(pr), partCap(pr * pr), count(0) {
        position = new DoubleTexture(GL_NEAREST);
        position->image(GL_RG32F, GL_RG, partRoot, partRoot, GL_FLOAT, 0);
        
        velocity = new DoubleTexture(GL_NEAREST);
        velocity->image(GL_RG32F, GL_RG, partRoot, partRoot, GL_FLOAT, 0);
        
        weights = new DoubleTexture(GL_LINEAR);
        weights->image(GL_R32F, GL_RED, simSize.x, simSize.y, GL_FLOAT, 0);
        
        grid = new DoubleTexture(GL_LINEAR);
        grid->image(GL_RGBA32F, GL_RGBA, gridSize.x, gridSize.y, GL_FLOAT, 0);
        
        dgrid = new DoubleTexture(GL_LINEAR);
        dgrid->image(GL_RGB32F, GL_RGB, gridSize.x, gridSize.y, GL_FLOAT, 0);
        
        divergence = new DoubleTexture(GL_LINEAR);
        divergence->image(GL_R32F, GL_RED, simSize.x, simSize.y, GL_FLOAT, 0);
        
        marks = new Texture(GL_NEAREST);
        marks->image(GL_R32F, GL_RED, simSize.x, simSize.y, GL_FLOAT, 0);
        
        drawer = new Shader("GLSL/drawer.vs", "GLSL/drawer.fs", "GLSL/shared.glsl");
        solver = new Shader("GLSL/pass.vs", "GLSL/solver.fs", "GLSL/shared.glsl");
        recter = new Shader("GLSL/texture.vs", "GLSL/drawRect.fs", "GLSL/shared.glsl");
        weighter = new Shader("GLSL/pass.vs", "GLSL/weight.fs", "GLSL/shared.glsl");
        stepper = new Shader("GLSL/pass.vs", "GLSL/step.fs", "GLSL/shared.glsl");
        setter = new Shader("GLSL/texture.vs", "GLSL/set.fs", "GLSL/shared.glsl");
        setter2 = new Shader("GLSL/pass.vs", "GLSL/set2.fs", "GLSL/shared.glsl");
        partToGrid = new Shader("GLSL/grid.vs", "GLSL/grid.fs", "GLSL/shared.glsl");
        diver = new Shader("GLSL/pass.vs", "GLSL/divergence.fs", "GLSL/shared.glsl");
        gridSolver = new Shader("GLSL/pass.vs", "GLSL/gridSolver.fs", "GLSL/shared.glsl");
        marker = new Shader("GLSL/marker.vs", "GLSL/marker.fs", "GLSL/shared.glsl");
        waverager = new Shader("GLSL/pass.vs", "GLSL/waverage.fs", "GLSL/shared.glsl");
        advector = new Shader("GLSL/pass.vs", "GLSL/advect.fs", "GLSL/shared.glsl");
        extor = new Shader("GLSL/pass.vs", "GLSL/ext.fs", "GLSL/shared.glsl");
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        std::vector<float> texels(partCap * 2);
        
        for(int y = 0; y < partRoot; ++y) {
            for(int x = 0; x < partRoot; ++x) {
                int i = (x + y * partRoot) * 2;
                texels[i + 0] = (x + 0.5f) / (float)partRoot;
                texels[i + 1] = (y + 0.5f) / (float)partRoot;
            }
        }
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 2 * partCap * sizeof(float), texels.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        (*weights)[1].bind();
        clear((*weights)[1].target->fbo, 0.0f, 0.0f, 0.0f, 0.0f);
        
        (*grid)[1].bind();
        clear((*grid)[1].target->fbo, 0.0f, 0.0f, 0.0f, 0.0f);
        
        (*velocity)[0].bind();
        clear((*velocity)[0].target->fbo, 0.0f, 0.0f, 0.0f, 0.0f);
        
        (*velocity)[1].bind();
        clear((*velocity)[1].target->fbo, 0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    ~World() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        
        delete position;
        delete velocity;
        delete weights;
        delete grid;
        delete divergence;
        delete dgrid;
        delete marks;
        
        delete drawer;
        delete solver;
        delete recter;
        delete weighter;
        delete stepper;
        delete setter;
        delete partToGrid;
        delete diver;
        delete gridSolver;
        delete marker;
        delete waverager;
        delete setter2;
        delete advector;
        delete extor;
    }
    
    void add(float, float, float, int, int, float, float);
    
    void add(float, float, float, int, int);
    
    void stepOnce(float dt);
    
    void solveGridOnce(float dt, int n);
    
    void solveOnce(float dt);
    
    void solveGrid(float dt);
    
    void render(GLuint target, GLuint x, GLuint y);
    
    void setVelocity(int start, int _count, float vx, float vy);
    
    inline void setVelocity(int _count, float vx, float vy) {
        setVelocity(count, _count, vx, vy);
    }
    
    inline void setVelocity(float vx, float vy) {
        setVelocity(0, partCap, vx, vy);
    }
    
    inline void blit(GLuint target, int start, int count, GLuint x, GLuint y) {
        glBindFramebuffer(GL_FRAMEBUFFER, target);
        glBindVertexArray(VAO);
        glViewport(0, 0, x, y);
        glDrawArrays(GL_POINTS, start, count);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    inline void blit(GLuint target, int start, int count) {
        blit(target, start, count, partRoot, partRoot);
    }
    
    inline void blitSim(GLuint target, int start, int count) {
        blit(target, start, count, simSize.x, simSize.y);
    }
    
    inline void blitGrid(GLuint target, int start, int count) {
        blit(target, start, count, gridSize.x, gridSize.y);
    }
};

#endif /* World_hpp */
