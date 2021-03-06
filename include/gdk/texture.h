// © 2019 Joseph Cameron - All Rights Reserved

#ifndef GDK_GFX_TEXTURE_H
#define GDK_GFX_TEXTURE_H

#include <cstddef>
#include <vector>

namespace gdk
{
    /// \brief 2d color data ususally used to color the surfaces of a 3d model.
    /// more accurately: used to color fragments produced from the rasterization of primitive surfaces.
    /// ultimately textures are just a uniform type, they can be used in any number of ways and may not contribute to the color of any fragments (e.g: heightmaps).
    class texture
    {
    public:
        enum class data_format //!< format of data in the byte array
        {
            rgb, //!< data is a sequence of bytes representing red, green, blue, ...
            rgba //!< data is a sequence of bytes representing red, green, blue, alpha, ...
        };

        /// \brief pod struct for 2d texture data
        /// \warn a view does not own its data.
        struct image_data_2d_view
        {
            size_t width; //!< number of texels wide
            size_t height; //!< number of texels tall
            
            data_format format; //!< format of the data

            //! raw image data
            /// \warn non-owning pointer
            std::byte *data;
        };

        //TODO image_data_2d. owns data. vec<byte>

        //! trivial destructor
        virtual ~texture() = default;

    protected:
        //! interface type cannot be instantiated
        texture() = default;
    };
}

#endif
