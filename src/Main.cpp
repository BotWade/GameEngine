#include "Main.hpp"

int main(void) {

    FileManager::UpdateGamePath();
    Window::Setup();

    ShaderManager::Init();
    MeshManager::Init();
    Renderer::Init();
    CollisionManager::Init();
    InterfaceManager::Init();

    InterfaceManager::AddObject(new InterfaceObject());

    ObjectsHandler::Load();

    UpdateManager::CreateGroup("Asteroids1");
    UpdateManager::CreateGroup("Asteroids2");
    UpdateManager::CreateGroup("Others");

    Object *skybox = new Object();
    Skybox *skyComp = new Skybox();
    skybox->AddComponent(skyComp);
    skyComp->LoadSkybox("Data/Skybox/SkyWater.skybox", "Data/Meshes/Teste.obj", "Data/Shaders/Skybox.shader");
    UpdateManager::AddToGroup("Others", skybox);
    ObjectsHandler::AddObject(skybox);
    
    ///Ship
    Object* ship = new Object();
    ship->Tag = "Ship";
    ship->AddComponent(new PlayerShip());

    UpdateManager::AddToGroup("Others", ship);
    ObjectsHandler::AddObject(ship);

    ///Asteroids
    for (int Index = 0; Index < 10; Index++) {
        Object* a = new Object();
        a->Tag = "Asteroid G1 N" + to_string(Index);
        a->AddComponent(new Asteroid());
        UpdateManager::AddToGroup("Asteroids1", a);
        ObjectsHandler::AddObject(a);
    }
    for (int Index = 0; Index < 10; Index++){
        Object* a = new Object();
        a->Tag = "Asteroid G2 N" + to_string(Index);
        a->AddComponent(new Asteroid());
        UpdateManager::AddToGroup("Asteroids2", a);
        ObjectsHandler::AddObject(a);
    }

    InterfaceManager::ExecuteCode(LOAD);
    ObjectsHandler::ExecuteCode(LOAD);

    glfwSwapInterval(0);

    TimeHelper::Start();

    while (!glfwWindowShouldClose(Window::window)) {

        TimeHelper::Update();
        Renderer::Clear();
        Camera::Update();
        InterfaceManager::ExecuteCode(UPDATE);
        ObjectsHandler::ExecuteCode(UPDATE);
        while (!CollisionManager::Work.empty()) { }
        ObjectsHandler::ExecuteCode(RENDER);
        InterfaceManager::ExecuteCode(RENDER);
        ObjectsHandler::ExecuteCode(POSRENDER);
        Input::Clear();
        Renderer::Swap();
    }

    glfwTerminate();
    return 0;
}
