//
//  World.cpp
//  Cells
//
//  Created by Arthur Sun on 2/13/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "World.hpp"

void World::solveGridOnce(float dt, int n) {
    DoubleTexture& G = *grid;
    
    marks->bind();
    clear(marks->target->fbo, 0.0f, 0.0f, 0.0f, 0.0f);
    
    
    
    marker->bind();
    marker->uniform1i("P", (*position)[1].id);
    marker->uniform1i("root", partRoot);
    marker->uniform2f("invSize", 1.0f/simSize);
    
    marks->bind();
    blitSim(marks->target->fbo, 0, count);
    
    
    
    
    extor->bind();
    extor->uniform1i("V", G[1].id);
    extor->uniform2i("size", gridSize);
    
    G[0].bind();
    //::blit(G[0].target->fbo, 0, 0, gridSize.x, gridSize.y);
    
    //G.swap();
    
    
    
    
    advector->bind();
    advector->uniform1i("V", G[1].id);
    advector->uniform1i("T", G[1].id);
    advector->uniform2f("invSize", 1.0f/gridSize);
    advector->uniform1f("dt", dt);

    G[0].bind();
    //::blit(G[0].target->fbo, 0, 0, gridSize.x, gridSize.y);

    //G.swap();
    
    
    
    
    diver->bind();
    diver->uniform1i("V", G[1].id);
    diver->uniform1i("M", marks->id);
    diver->uniform2i("size", simSize);
    
    (*divergence)[1].bind();
    ::blit((*divergence)[1].target->fbo, 0, 0, simSize.x, simSize.y);
    
    
    
    for(int i = 0; i < n; ++i) {
        weighter->bind();
        weighter->uniform1i("W", (*weights)[1].id);
        weighter->uniform1i("M", marks->id);
        weighter->uniform1i("D", (*divergence)[1].id);
        weighter->uniform2i("size", simSize);
        
        (*weights)[0].bind();
        ::blit((*weights)[0].target->fbo, 0, 0, simSize.x, simSize.y);
        
        weights->swap();
    }
}

void World::solveGrid(float dt) {
    DoubleTexture& G = *grid;
    
    G[1].bind();
    clear(G[1].target->fbo, 0.0f, 0.0f, 0.0f, 0.0f);
    
    
    partToGrid->bind();
    partToGrid->uniform1i("P", (*position)[1].id);
    partToGrid->uniform1i("V", (*velocity)[1].id);
    partToGrid->uniform2f("size", simSize);
    partToGrid->uniform2f("gridSize", gridSize);
    partToGrid->uniform1i("root", partRoot);
    
    glEnable(GL_BLEND);
    
    partToGrid->uniform1i("x", 0);
    partToGrid->uniform1i("y", 1);
    
    G[1].bind();
    blitGrid(G[1].target->fbo, 0, count);
    
    partToGrid->uniform1i("x", 1);
    partToGrid->uniform1i("y", 0);
    
    G[1].bind();
    blitGrid(G[1].target->fbo, 0, count);
    
    glDisable(GL_BLEND);
    
    
    
    
    
    waverager->bind();
    waverager->uniform1i("V", G[1].id);
    
    G[0].bind();
    ::blit(G[0].target->fbo, 0, 0, gridSize.x, gridSize.y);
    
    G.swap();
    
    
    
    
    
    setter2->bind();
    setter2->uniform1i("T", G[1].id);
    setter2->uniform4f("u", 1.0f, 1.0f, 1.0f, 1.0f);
    setter2->uniform4f("a", 0.0f, 0.0f, 0.0f, 0.0f);
    setter2->uniform2f("invSize", 1.0f/gridSize);
    
    (*dgrid)[1].bind();
    ::blit((*dgrid)[1].target->fbo, 0, 0, gridSize.x, gridSize.y);
    
    
    
    int n = 1, k = 3;
    for(int i = 0; i < n; ++i)
        solveGridOnce(dt, k);
}

void World::stepOnce(float dt) {
    stepper->bind();
    stepper->uniform1i("P", (*position)[1].id);
    stepper->uniform1i("V", (*velocity)[1].id);
    stepper->uniform2f("size", simSize);
    stepper->uniform1f("dt", dt);
    stepper->uniform1i("count", count);
    stepper->uniform1i("root", partRoot);
    
    (*position)[0].bind();
    ::blit((*position)[0].target->fbo, 0, 0, partRoot, partRoot);
    
    position->swap();
}

void World::solveOnce(float dt) {
    solveGrid(dt);
    
    
    gridSolver->bind();
    gridSolver->uniform1i("V", (*grid)[1].id);
    gridSolver->uniform1i("U", (*weights)[1].id);
    gridSolver->uniform1i("M", marks->id);
    gridSolver->uniform2i("size", simSize);
    
    (*grid)[0].bind();
    ::blit((*grid)[0].target->fbo, 0, 0, gridSize.x, gridSize.y);
    
    grid->swap();
    
    
    
    
    solver->bind();
    solver->uniform1i("P", (*position)[1].id);
    solver->uniform1i("V", (*velocity)[1].id);
    solver->uniform1i("G", (*grid)[1].id);
    solver->uniform1i("D", (*dgrid)[1].id);
    solver->uniform1i("W", (*weights)[1].id);
    solver->uniform2f("gravity", 0.0f, -600.0f);
    solver->uniform2f("gridSize", gridSize);
    solver->uniform2f("size", simSize);
    solver->uniform1f("dt", dt);
    solver->uniform1i("count", count);
    solver->uniform1i("root", partRoot);
    
    (*velocity)[0].bind();
    ::blit((*velocity)[0].target->fbo, 0, 0, partRoot, partRoot);
    
    velocity->swap();
    
    stepOnce(dt);
}

void World::render(GLuint target, GLuint x, GLuint y) {
    drawer->bind();
    drawer->uniform1i("T", (*position)[1].id);
    drawer->uniform2f("scl", 1.0f/simSize);
    drawer->uniform1i("root", partRoot);
    
    glEnable(GL_BLEND);
    
    blit(target, 0, count, x, y);
    
    glDisable(GL_BLEND);
}

void World::setVelocity(int start, int _count, float vx, float vy) {
    setter->bind();
    setter->uniform4f("a", vx, vy, 0.0f, 0.0f);
    
    (*velocity)[1].bind();
    blit((*velocity)[1].target->fbo, start, _count);
    
    (*velocity)[0].bind();
    blit((*velocity)[0].target->fbo, start, _count);
}

void World::add(float x, float y, float s, int w, int h) {
    int i = w * h;
    
    assert(count + i <= partCap);
    
    recter->bind();
    recter->uniform1f("sp", s);
    recter->uniform2f("pos", x, y);
    recter->uniform1i("hx", w);
    recter->uniform1i("hy", h);
    recter->uniform1i("count", count);
    recter->uniform1i("root", partRoot);
    
    (*position)[1].bind();
    blit((*position)[1].target->fbo, count, i);
    
    count += i;
}

void World::add(float x, float y, float s, int w, int h, float vx, float vy) {
    int i = w * h;
    assert(count + i <= partCap);
    setVelocity(i, vx, vy);
    add(x, y, s, w, h);
}
