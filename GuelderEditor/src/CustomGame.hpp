// temporary helper function, creates a 1x1x1 cube centered at offset
Mesh3D CreateCube3DMesh(const Vector3& offset)
{
    Vertices3D vertices
    {
        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    };

    for(auto& v : vertices) {
        v.pos += offset;
    }

    Indices indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                          12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

    return Mesh3D{ vertices, indices };
}
void ChangeCubeColor(Mesh3D& mesh, const Vector3& color)
{
    for(auto& v : mesh.m_Vertices)
        v.color = color;
}

const Mesh2D square =
{
    {
       {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
       {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
       {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
       {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    },

    {
        0, 1, 2,
        2, 3, 0
    }
};
const Mesh2D triangle =
{
    {
       {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
       {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
       {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    },

    {0, 1, 2}
};
const Mesh2D hexagon =
{
    {
        {{ 0.0f,  0.5f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.25f}, {1.0f, 1.0f, 0.0f}},
        {{-0.5f, -0.25f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.0f, -0.5f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.25f}, {0.0f, 1.0f, 1.0f}},
        {{ 0.5f,  0.25f}, {1.0f, 0.0f, 1.0f}}
    },

    {
        0, 1, 5, // Triangle 1
        1, 2, 5, // Triangle 2
        2, 3, 5, // Triangle 3
        3, 4, 5  // Triangle 4
    }
};

class MyActor2D : public Actor2D
{
public:
    MyActor2D() = default;
    MyActor2D(const Vulkan::Mesh2D& mesh, const Actor2DTransform& transform)
        : Actor2D(mesh, transform) {}

    void Update() override
    {
        transform.rotation = glm::mod(transform.rotation + .001f, glm::two_pi<float>());
        transform.position.y = .2f * sin(GEApplication::GetTime());
    }
};
class MyActor3D : public Actor3D
{
public:
    MyActor3D() = default;
    MyActor3D(const Vulkan::Mesh3D& mesh, const Actor3DTransform& transform)
        : Actor3D(mesh, transform) {}

    void Update() override
    {
        //transform.rotation.y = glm::mod(transform.rotation.y + .0001f, glm::two_pi<float>());
        //transform.rotation.x = glm::mod(transform.rotation.x + .0001f, glm::two_pi<float>());
        //transform.rotation.z = glm::mod(transform.rotation.z + .0001f, glm::two_pi<float>());

        transform.position.x = glm::mod(transform.position.x + .1f, glm::two_pi<float>());
        transform.position.y = glm::mod(transform.position.y + .1f, glm::two_pi<float>());
    }
};
class CameraActor : public Actor3D
{
public:
    CameraActor(const CameraComponent& cameraComponent = CameraComponent{})//it cannot find CameraActor
        : cameraComponent(cameraComponent) {}

    bool IsComplete() const noexcept override { return false; }

    void Update() override
    {

    }

    CameraComponent cameraComponent;
};
class MyPlayerController : public PlayerController
{
public:
    DECLARE_DEFAULT_COPYING_AND_MOVING(MyPlayerController);
    MyPlayerController(World* world)
        : world(world) {}
    ~MyPlayerController() override = default;

    void BeginPlay() override
    {
        auto found = world->GetActor("cube");
        cube = dynamic_cast<MyActor3D*>(found.get());
    }
    void SetupInput() override
    {
        BindKey(Keys::W, InputKeyState::Pressed, "move_forward", this, &MyPlayerController::MoveForward);
        BindKey(Keys::S, InputKeyState::Pressed, "move_backward", this, &MyPlayerController::MoveBackward);
        BindKey(Keys::D, InputKeyState::Pressed, "move_right", this, &MyPlayerController::MoveRight);
        BindKey(Keys::A, InputKeyState::Pressed, "move_left", this, &MyPlayerController::MoveLeft);
        BindKey(Keys::Space, InputKeyState::Pressed, "move_up", this, &MyPlayerController::MoveUp);
        BindKey(Keys::C, InputKeyState::Pressed, "move_down", this, &MyPlayerController::MoveDown);

        BindKey(Keys::Up, InputKeyState::Pressed, "look_up", this, &MyPlayerController::RotateUp);
        BindKey(Keys::Down, InputKeyState::Pressed, "look_down", this, &MyPlayerController::RotateDown);
        BindKey(Keys::Right, InputKeyState::Pressed, "look_right", this, &MyPlayerController::RotateRight);
        BindKey(Keys::Left, InputKeyState::Pressed, "look_left", this, &MyPlayerController::RotateLeft);

        BindKey(Keys::None, InputKeyState::Pressed, "rotation_clamp", this, &MyPlayerController::ClampRotation);

        BindKey(Keys::F, InputKeyState::Down, "cube_color", this, &MyPlayerController::ChangeCubeColor);

        BindKey(Keys::_1, InputKeyState::Pressed, "cube_rotate_x", this, &MyPlayerController::RotateCubeByX);
        BindKey(Keys::_2, InputKeyState::Pressed, "cube_rotate_y", this, &MyPlayerController::RotateCubeByY);
        BindKey(Keys::_3, InputKeyState::Pressed, "cube_rotate_z", this, &MyPlayerController::RotateCubeByZ);

        BindKey(Keys::LeftShift, InputKeyState::Down, "left_shift_down", this, &MyPlayerController::IsLeftShiftDown);
        BindKey(Keys::LeftShift, InputKeyState::Released, "left_shift_released", this, &MyPlayerController::IsLeftShiftReleased);
    }

    void ClampRotation(float)
    {
        camera->transform.rotation.x = glm::clamp(camera->transform.rotation.x, -1.5f, 1.5f);
        camera->transform.rotation.y = glm::mod(camera->transform.rotation.y, glm::two_pi<float>());
    }

    void IsLeftShiftDown(float)
    {
        rotateMultimplier = -1.f;
    }
    void IsLeftShiftReleased(float)
    {
        rotateMultimplier = 1.f;
    }

    void ChangeCubeColor(float)
    {
        if(cube)
        {
            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_real_distribution<float> dist(0.f, 1.f);

            auto newMesh = cube->meshComponent->GetMesh();
            ::ChangeCubeColor(newMesh, { dist(gen), dist(gen), dist(gen) });
            cube->meshComponent->SetMesh(newMesh);
        }
    }

    void RotateCubeByX(float)
    {
        if(cube)
            cube->transform.rotation.x = glm::mod(cube->transform.rotation.x + .001f * rotateMultimplier, glm::two_pi<float>());
    }
    void RotateCubeByY(float)
    {
        if(cube)
            cube->transform.rotation.y = glm::mod(cube->transform.rotation.y + .001f * rotateMultimplier, glm::two_pi<float>());
    }
    void RotateCubeByZ(float)
    {
        if(cube)
            cube->transform.rotation.z = glm::mod(cube->transform.rotation.z + .001f * rotateMultimplier, glm::two_pi<float>());
    }

    void MoveForward(float deltaTime)
    {
        float yaw = camera->transform.rotation.y;
        const Vector3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
        if(glm::dot(forwardDir, forwardDir) > std::numeric_limits<float>::epsilon())
            camera->transform.translation += moveSpeed * deltaTime * glm::normalize(forwardDir);
    }
    void MoveBackward(float deltaTime)
    {
        float yaw = camera->transform.rotation.y;
        const Vector3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
        if(glm::dot(-forwardDir, -forwardDir) > std::numeric_limits<float>::epsilon())
            camera->transform.translation += moveSpeed * deltaTime * glm::normalize(-forwardDir);
    }
    void MoveRight(float deltaTime)
    {
        float yaw = camera->transform.rotation.y;
        const Vector3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
        const Vector3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
        if(glm::dot(rightDir, rightDir) > std::numeric_limits<float>::epsilon())
            camera->transform.translation += moveSpeed * deltaTime * glm::normalize(rightDir);
    }
    void MoveLeft(float deltaTime)
    {
        float yaw = camera->transform.rotation.y;
        const Vector3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
        const Vector3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
        if(glm::dot(-rightDir, -rightDir) > std::numeric_limits<float>::epsilon())
            camera->transform.translation += moveSpeed * deltaTime * glm::normalize(-rightDir);
    }
    void MoveUp(float deltaTime)
    {
        const Vector3 upDir{ 0.f, -1.f, 0.f };
        if(glm::dot(upDir, upDir) > std::numeric_limits<float>::epsilon())
            camera->transform.translation += moveSpeed * deltaTime * glm::normalize(upDir);
    }
    void MoveDown(float deltaTime)
    {
        const Vector3 upDir{ 0.f, -1.f, 0.f };
        if(glm::dot(-upDir, -upDir) > std::numeric_limits<float>::epsilon())
            camera->transform.translation += moveSpeed * deltaTime * glm::normalize(-upDir);
    }

    void RotateUp(float deltaTime)
    {
        Vector3 rotate{ 0 };
        rotate.x += 1.f;
        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
            camera->transform.rotation += lookSpeed * deltaTime * glm::normalize(rotate);
    }
    void RotateDown(float deltaTime)
    {
        Vector3 rotate{ 0 };
        rotate.x -= 1.f;
        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
            camera->transform.rotation += lookSpeed * deltaTime * glm::normalize(rotate);
    }
    void RotateRight(float deltaTime)
    {
        Vector3 rotate{ 0 };
        rotate.y += 1.f;
        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
            camera->transform.rotation += lookSpeed * deltaTime * glm::normalize(rotate);
    }
    void RotateLeft(float deltaTime)
    {
        Vector3 rotate{ 0 };
        rotate.y -= 1.f;
        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
            camera->transform.rotation += lookSpeed * deltaTime * glm::normalize(rotate);
    }

    MyActor3D* cube;
    World* world;

    float moveSpeed = 1.f;
    float lookSpeed = 1.f;

    float rotateMultimplier = 1.f;
};
class MyGameMode : public GameMode
{
public:
    DECLARE_DEFAULT_COPYING_AND_MOVING(MyGameMode);
    MyGameMode(CameraComponent& cameraComponent, World* world)
    {
        m_PlayerController = std::move(std::make_unique<MyPlayerController>(MyPlayerController{ world }));
        cameraComponent.SetViewTarget(Vector3{ -1.f, -5.f, 2.f }, Vector3{ 0.f, 0.f, .5f });
        m_PlayerController->camera = &cameraComponent;
    }
    ~MyGameMode() override = default;
};

void SpawnPencil(World* world)
{
    //pencil
    world->SpawnActor3D
    (
        MakeActor
        (
            MyActor3D
            {


                    CreateCube3DMesh({0, 0.f, 0}),

                    {
                        {0, 0, .5f},
                        {.4f, 1.5f, .4f},//scale
                        {2,1,1},//position
                        {0, 0, 0},//rotation
                    }

            }
        )
    );
    world->SpawnActor3D
    (
        MakeActor
        (
            MyActor3D
            {
                    CreateCube3DMesh({1.0f, 1.38f, 0}),

                    {
                        {0, 0, .5f},
                        {.4f, .4f, .4f},//scale
                        {2,1,.5f},//position
                        {0, 0, 0},//rotation
                    }

            }
        )

    );
    world->SpawnActor3D
    (
        MakeActor
        (
            MyActor3D
            {

                    CreateCube3DMesh({-1.0f, 1.38f, 0}),

                    {
                        {0, 0, .5f},
                        {.4f, .4f, .4f},//scale
                        {2,1,.5f},//position
                        {0, 0, 0},//rotation
                    }

            }
        )

    );
}
void Spawn2Ds(World* world)
{
    world->SpawnActor2D
    (
        MakeActor
        (
            MyActor2D
            {

                    triangle,

                    {
                        {},
                        {.2f, .2f},//scale
                        {-.5f, 1.f},//position
                        0.f//rotation
                    }

            }
        )
    );
    world->SpawnActor2D
    (
        MakeActor
        (
            MyActor2D
            {
                    square,

                    {
                        {},
                        {.2f, .2f},//scale
                        {.5f, 1.f},//position
                        0.f//rotation
                    }

            },
            "2DSquare"
        )
    );
}
void SpawnALotOfStuff(World* world)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dist(-100.f, 100.f);
    for(size_t i = 0; i < 2000; i++)
    {
        world->SpawnActor3D
        (
            MakeActor
            (
                MyActor3D
                {

                        CreateCube3DMesh({dist(gen), dist(gen), dist(gen)}),
                        {
                            {0, 0, .5f},
                            {.1f, .1f, .1f},//scale
                            {0,0,0},//position
                            {dist(gen), dist(gen), dist(gen)},//rotation
                        }


                }
            )
        );
    }
}