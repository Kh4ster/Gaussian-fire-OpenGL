#pragma once

#include "vector.hh"

#include <cmath>

namespace scene
{
struct Camera final
{
    Camera(const space::Vector3& origin,
           const space::Vector3& y_axis,
           const space::Vector3& z_axis,
           const float z_min,
           const float z_max,
           const float alpha,
           const int width,
           const int height);

    Camera(const Camera&) = default;
    Camera& operator=(const Camera&) = default;

    ~Camera() = default;

	using Mat4 = space::Vector<16, float>;

    Mat4 get_projection_matrix() const
    {
		const float total  = z_min_ + z_max_;
    	const float distance = z_max_ - z_min_;
    	const float ratio   = static_cast<float>(width) / height;
    	const float fov   = 1.0f / std::tan(deg_to_rad(alpha_) / 2.0f);
    
		return Mat4( 
		fov / ratio, 0.0f,  0.0f, 0.0f,
		0.0f,  fov, 0.0f, 0.0f,
		0.0f,  0.0f, -total / distance, -1.0 ,
		0.0f,  0.0f, -2.0f * z_max_ * z_min_ / distance,  0.0f);
    }

	Mat4 get_model_view_matrix()
	{ 
    	const Vector3 z = Vector3(origin_[0] - z_axis_[0], origin_[1] - z_axis_[1], origin_[2] - z_axis_[2]).nomalized();
    	const Vector3 x = Cross( my, mz );
    Normalize( mx );
    my = Cross( mz, mx );

    TMat44 v{
        TVec4{ mx[0], my[0], mz[0], 0.0f },
        TVec4{ mx[1], my[1], mz[1], 0.0f },
        TVec4{ mx[2], my[2], mz[2], 0.0f },
        TVec4{ Dot(mx, pos), Dot(my, pos), Dot(TVec3{-mz[0], -mz[1], -mz[2]}, pos), 1.0f }
    };

    return v;
}

    // Origin of the camera `C`
    space::Vector3 origin_;

    // Three axis of the camera
    // Unit vectors
    space::Vector3 y_axis_;
    space::Vector3 z_axis_;

    // Focal distances
    float z_min_;
    float z_max_;

    // Fov y in degree
    float alpha_;

    // Image dimensions
    int width;
    int height;

	constexpr float deg_to_rad(float degree)
	{
		return (degree * (pi / 180));
	}
	static constexpr float pi = 3.14159265359f;
};

} // namespace scene