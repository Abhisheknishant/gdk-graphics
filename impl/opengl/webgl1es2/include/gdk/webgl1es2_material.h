// © 2019 Joseph Cameron - All Rights Reserved

#ifndef GDK_GFX_MATERIAL_H
#define GDK_GFX_MATERIAL_H

#include <gdk/webgl1es2_shader_program.h>

#include <unordered_map>

namespace gdk
{
    //! Decides how a model is rendered. Specifies shader effects, textures, etc.
    /// From the perspective of opengl: a collection of uniform values, opengl pipeline state, and the shader program to be used when rendering a model that uses this material
    //TODO think about how to model blend functionality.
    class webgl1es2_material
    {
    public:
        //! shaders can be shared among many webgl1es2_materials, therefore shared_ptr
        using shader_ptr_type = std::shared_ptr<gdk::webgl1es2_shader_program>;

        //! textures can be shared among many webgl1es2_materials.
        using texture_ptr_type = std::shared_ptr<gdk::webgl1es2_texture>;

        //! associative collection: uniform name to texture data
        using texture_uniform_collection_type = std::unordered_map<std::string, texture_ptr_type>;

    public: //private: //TODO: private, add getters if appropriate
        //! the shader used by the webgl1es2_material
        shader_ptr_type m_pShader;

        //! texture data provided to the shader stages
        texture_uniform_collection_type m_Textures;

    public:
        //! tries to assign a texture value to a texture uniform of the given name. fails silently
        void setTexture(const std::string &aTextureName, texture_ptr_type aTexture);

        //! modifies the opengl state, assigning the program, assigning values to the program's uniforms etc.
        void activate();

        //! support move semantics
        webgl1es2_material &operator=(webgl1es2_material &&) = default;
        //! support move semantics
        webgl1es2_material(webgl1es2_material &&) = default;

        //! support copy...
        webgl1es2_material &operator=(const webgl1es2_material &) = default;
        //! support copy...
        webgl1es2_material(const webgl1es2_material &) = default;

        //! constructs a webgl1es2_material.
        /// non-shader pipeline state and uniform values are 0 constructed, given default values specified by the opengl standard
        /// however a shader must be provided at ctor time, since the pipeline cannot be invoked without first using a shader
        /// logically, a webgl1es2_material without a shader_program implies a pipeline without a programmable vertex shader stage 
        /// nor a programmable fragment shader stage, which is not a valid pipeline
        webgl1es2_material(shader_ptr_type pShader);

        ~webgl1es2_material() = default;
    };
}

#endif