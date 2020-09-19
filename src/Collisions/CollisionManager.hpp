#ifndef _COLLISIONMANAGER_
#define _COLLISIONMANAGER_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <string>
#include <pthread.h>
#include <atomic>

#include "Hit.hpp"
#include "../Math/Ray.hpp"

using namespace std;

struct Collider;
struct Component;

struct RayTestResult {
  vector<Hit*> result;
  Ray ray;  
};

class CollisionManager {
public:
  static vector<Collider*> Colliders;
  static vector<RayTestResult> Result;
  static map<Ray, vector<Component*>> Work;
  static pthread_t thread;
  static atomic_flag QueueInUse;

  static void Init();
  static void * Worker(void *threadarg);
  static void AddCollider(Collider* collider);
  static void ProcessRay(Ray ray, Component* caller);
  static void ColliderVsRay(Ray ray);
  static vector<Hit*> ColliderVsRay(Ray ray, bool getIntersection = false);
  static void Clear();
};

#endif