module;
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
export module GuelderEngine.Actors:CameraComponent;

import :ActorComponent;
import GuelderEngine.Core.Types;

export namespace GuelderEngine
{
    class CameraComponent : public SceneComponent<3>
    {
    public:
        CameraComponent();
        ~CameraComponent() override = default;

        void SetOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        void SetPerspectiveProjection(float fovY, float aspect, float near, float far);

        const Mat4& GetProjection() const;
    private:
        Mat4 m_ProjectionMatrix;
    };
}