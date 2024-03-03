module;
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "../../../private/headers/Core/GObject/GClass.hpp"
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

        DECLARE_DEFAULT_COPYING_AND_MOVING(CameraComponent);

        void SetOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        void SetPerspectiveProjection(float fovY, float aspect, float near, float far);

        void SetViewDirection(const Vector3& position, const Vector3& direction, const Vector3& up = { 0.f, -1.f, 0.f });
        void SetViewTarget(const Vector3& position, const Vector3& target, const Vector3& up = { 0.f, -1.f, 0.f });
        void SetViewYXZ(const Vector3& position, const Vector3& rotation);

        const Mat4& GetProjection() const;
        const Mat4& GetViewMatrix() const;
    private:
        Mat4 m_ProjectionMatrix;
        Mat4 m_ViewMatrix;
    };
}