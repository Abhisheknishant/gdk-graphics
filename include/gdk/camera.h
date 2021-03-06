// © 2019 Joseph Cameron - All Rights Reserved

#ifndef GDK_GFX_CAMERA_H
#define GDK_GFX_CAMERA_H

#include <gdk/graphics_types.h>

namespace gdk
{
    /// \brief Position, orientation and perspective from which entity(s) are drawn
    class camera
    {
        //TODO: implement.
    public:
        /// \brief sets the projection matrix to a perspective projection
        virtual void setProjection(const float aFieldOfView, 
            const float aNearClippingPlane, 
            const float aFarClippingPlane, 
            const float aViewportAspectRatio) = 0;

        /// \brief rebuilds the view matrix from a 3d position and rotation
        virtual void set_view_matrix(const gdk::graphics_vector3_type &aWorldPos,
            const gdk::graphics_quaternion_type &aRotation) = 0;

        /// \brief gets view matrix
        virtual graphics_mat4x4_type getViewMatrix() const = 0;
        
        //! gets the projection matrix
        virtual graphics_mat4x4_type getProjectionMatrix() const = 0;

        virtual ~camera() = default;

    protected:
        camera() = default;
    };
}

#endif
