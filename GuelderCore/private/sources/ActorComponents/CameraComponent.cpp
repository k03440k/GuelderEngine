module;
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Actors;
import :CameraComponent;

import :ActorComponent;

import <limits>;

namespace GuelderEngine
{
    CameraComponent::CameraComponent()
        : m_ProjectionMatrix(1.f)
    {
    }
    void CameraComponent::SetOrthographicProjection(float left, float right, float top, float bottom, float near, float far)
    {
        m_ProjectionMatrix = glm::mat4{ 1.0f };
        m_ProjectionMatrix[0][0] = 2.f / (right - left);
        m_ProjectionMatrix[1][1] = 2.f / (bottom - top);
        m_ProjectionMatrix[2][2] = 1.f / (far - near);
        m_ProjectionMatrix[3][0] = -(right + left) / (right - left);
        m_ProjectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        m_ProjectionMatrix[3][2] = -near / (far - near);
    }
    void CameraComponent::SetPerspectiveProjection(float fovY, float aspect, float near, float far)
    {
        GE_ASSERT(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFovY = tan(fovY / 2.f);
        m_ProjectionMatrix = glm::mat4{ 0.0f };
        m_ProjectionMatrix[0][0] = 1.f / (aspect * tanHalfFovY);
        m_ProjectionMatrix[1][1] = 1.f / (tanHalfFovY);
        m_ProjectionMatrix[2][2] = far / (far - near);
        m_ProjectionMatrix[2][3] = 1.f;
        m_ProjectionMatrix[3][2] = -(far * near) / (far - near);
    }
    const Mat4& CameraComponent::GetProjection() const
    {
        return m_ProjectionMatrix;
    }
}