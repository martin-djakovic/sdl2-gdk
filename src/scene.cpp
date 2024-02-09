#include <scene.hpp>

Scene::Scene(SDL_Window *window, Camera *camera) {
    renderer = SDL_GetRenderer(window);
    this->camera = camera;
}

void Scene::AddRenderObject(BasicSprite *object) {
    object->SetRenderer(renderer);
    render_objects.push_back(object);
}

void Scene::RemoveRenderObject(BasicSprite *object, bool destroy) {
    render_objects.erase(
        std::remove(render_objects.begin(), render_objects.end(), object),
        render_objects.end());

    if (destroy) {
        object->Destroy();
    }
}

void Scene::CalculateFPS() {
    if (odd_frame) {
        last_frame_time = SDL_GetPerformanceCounter();
        odd_frame = !odd_frame;
    } else {
        Uint64 this_frame_time = SDL_GetPerformanceCounter();
        fps = 1.0f / ((this_frame_time - last_frame_time) /
                      (float)SDL_GetPerformanceFrequency());
        odd_frame = !odd_frame;
    }
}

void Scene::DrawHitboxOutlines() {
    for (int i = 0; i < collide_sprites.size(); i++) {
        SDL_RenderDrawRect(renderer, collide_sprites.at(i)->GetHitbox());
    }
}

void Scene::MoveCamera() {
    // Move camera for all objects
    for (int i = 0; i < basic_sprites.size(); i++) {
        // Make sure that object isn't a focused sprite,
        // and that camera has been moved
        if (std::find(camera->GetFocusedObjects()->begin(),
                      camera->GetFocusedObjects()->end(),
                      basic_sprites.at(i)) ==
                camera->GetFocusedObjects()->end() &&
            (camera->x != 0 || camera->y != 0)) {

            basic_sprites.at(i)->MoveX(camera->x);
            basic_sprites.at(i)->MoveY(camera->y);
        }
    }

    // Move camera for all collide sprites
    for (int i = 0; i < collide_sprites.size(); i++) {
        // Make sure that collide object isn't a focused sprite,
        // and that camera has been moved
        if (std::find(camera->GetFocusedObjects()->begin(),
                      camera->GetFocusedObjects()->end(),
                      collide_sprites.at(i)) ==
                camera->GetFocusedObjects()->end() &&
            (camera->x != 0 || camera->y != 0)) {

            collide_sprites.at(i)->MoveX(camera->x, false);
            collide_sprites.at(i)->MoveY(camera->y, false);
        }
    }

    camera->x = 0;
    camera->y = 0;
}

void Scene::DrawRenderObjects() {
    for (int i = 0; i < render_objects.size(); i++) {
        if (render_objects.at(i)->IsInBounds()) {
            render_objects.at(i)->Draw();
        }
    }
}

void Scene::Destroy() {
    for (int i = 0; i < render_objects.size(); i++) {
        render_objects.at(i)->Destroy();
    }

    basic_sprites.clear();
    collide_sprites.clear();
    render_objects.clear();
}

void Scene::AddCollideObject(CollideSprite *collide_object) {
    AddRenderObject(collide_object);
    collide_object->SetColliders(&collide_sprites);
    collide_sprites.push_back(collide_object);
}

void Scene::AddCollideObject(
    const std::vector<CollideSprite *> collide_object) {
    for (int i = 0; i < collide_object.size(); i++) {
        AddCollideObject(collide_object.at(i));
    }
}

void Scene::AddObject(BasicSprite *object) {
    AddRenderObject(object);
    basic_sprites.push_back(object);
}

void Scene::AddObject(const std::vector<BasicSprite *> object) {
    for (int i = 0; i < object.size(); i++) {
        AddObject(object.at(i));
    }
}

void Scene::RemoveCollideObject(CollideSprite *collide_object, bool destroy) {
    collide_sprites.erase(std::remove(collide_sprites.begin(),
                                      collide_sprites.end(), collide_object),
                          collide_sprites.end());

    RemoveRenderObject(collide_object, destroy);
}

void Scene::RemoveCollideObject(
    const std::vector<CollideSprite *> collide_object, bool destroy) {
    for (int i = 0; i < collide_object.size(); i++) {
        RemoveCollideObject(collide_object.at(i));
    }
}

void Scene::RemoveObject(BasicSprite *object, bool destroy) {
    basic_sprites.erase(
        std::remove(basic_sprites.begin(), basic_sprites.end(), object),
        basic_sprites.end());
    RemoveRenderObject(object, destroy);
}

void Scene::RemoveObject(const std::vector<BasicSprite *> object,
                         bool destroy) {
    for (int i = 0; i < object.size(); i++) {
        RemoveObject(object.at(i));
    }
}

void Scene::Draw() {
    CalculateFPS();

    SDL_RenderClear(renderer);
    SDL_GetRendererOutputSize(renderer, &win_w, &win_h);

    MoveCamera();
    DrawRenderObjects();

    if (show_hitbox_outlines) {
        DrawHitboxOutlines();
    }

    SDL_RenderPresent(renderer);
}

std::vector<CollideSprite *> *Scene::GetAllCollideObjects() {
    return &collide_sprites;
}

std::vector<BasicSprite *> *Scene::GetAllObjects() { return &render_objects; }

int Scene::GetFPS() { return fps; }

void Scene::SetShowHitboxOutlines(bool show_hitbox_outlines) {
    SDL_SetRenderDrawColor(renderer, hitbox_outline_color.r,
                           hitbox_outline_color.g, hitbox_outline_color.b,
                           hitbox_outline_color.a);
    this->show_hitbox_outlines = show_hitbox_outlines;
}

void Scene::SetHitboxOutlineColor(SDL_Color color) {
    hitbox_outline_color = color;
    SDL_SetRenderDrawColor(renderer, hitbox_outline_color.r,
                           hitbox_outline_color.g, hitbox_outline_color.b,
                           hitbox_outline_color.a);
}