#include "CollisionManager.hpp"
#include "../Core/Window.hpp"
#include "Collider.hpp"

vector<Collider*> CollisionManager::Colliders;
vector<RayTestResult> CollisionManager::Result;
map<Ray, vector<Component*>>CollisionManager::Work;
pthread_t CollisionManager::thread;
atomic_flag CollisionManager::QueueInUse;

void CollisionManager::Init() {
    pthread_create(&thread, NULL, Worker, NULL);
}

void * CollisionManager::Worker(void *threadarg) {
    while (!glfwWindowShouldClose(Window::window)) {
        while (!Work.empty()) {
            while (QueueInUse.test_and_set()) { }
            
            Ray ray = Work.begin()->first;

            RayTestResult result = RayTestResult();
            result.ray = ray;
            result.result = ColliderVsRay(ray, ray.StopFirst);
            vector<Component*> Components = Work.begin()->second;
            Work.erase(Work.begin());
            size_t Size = Components.size();
            if (!result.result.empty())
                for (size_t Index = 0; Index < Size; Index++)
                    Components[Index]->OnCollision(result);
            QueueInUse.clear();
        }
    }
}

void CollisionManager::AddCollider(Collider* collider) {

    Colliders.push_back(collider);
}

void CollisionManager::ProcessRay(Ray ray, Component* caller) {

    while (QueueInUse.test_and_set()) { }
    

    size_t Size = Result.size();
    for (size_t Index = 0; Index < Size; Index++) {
        if (Result[Index].ray == ray) {
            caller->OnCollision(Result[Index]);
            return;
        }
    }

    map<Ray, vector<Component*>>::iterator it = Work.find(ray);

    if (it == Work.end()) {
        Work.insert(pair<Ray, vector<Component*>>(ray, vector<Component*>()));
        Work[ray].push_back(caller);
    }
    else
        Work[ray].push_back(caller);

    QueueInUse.clear();
}

vector<Hit*> CollisionManager::ColliderVsRay(Ray ray, bool stopFirst) {

    size_t Size = Colliders.size();

    vector<Hit*> hit = vector<Hit*>();

    for (size_t Index = 0; Index < Size; Index++) {

        if (Colliders[Index]->CheckCollision(ray)) {
            Hit *h = new Hit();
            h->Origin = ray.Origin;
            h->Target = Colliders[Index]->object->transform.Position;
            h->Distance = Magnitude(Colliders[Index]->object->transform.Position - ray.Origin);
            h->Hitted = Colliders[Index];

            hit.push_back(h);
            
            if (stopFirst) {
                return hit;
            }
        }
    }

    return hit;
}