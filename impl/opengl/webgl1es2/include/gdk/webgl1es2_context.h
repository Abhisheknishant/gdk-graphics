// © 2019 Joseph Cameron - All Rights Reserved

#ifndef GDK_GFX_WEBGL1ES2_CONTEXT_H
#define GDK_GFX_WEBGL1ES2_CONTEXT_H

#include <gdk/graphics_context.h>

namespace gdk
{
    //! brief webgl1/gles2.0 context implementation
    class webgl1es2_context final : public graphics::context
    {
    public: 
        virtual scene_ptr_type make_scene() const override;

        virtual graphics::context::camera_ptr_type make_camera() const override;

        virtual camera_ptr_type make_camera(const camera &other) const override;

        virtual entity_ptr_type make_entity(model_shared_ptr_type pModel, material_shared_ptr_type pMaterial) const override;

        virtual entity_ptr_type make_entity(const entity& other) const override;

        virtual graphics::context::material_ptr_type make_material(shader_program_shared_ptr_type pShader) const override;

        virtual graphics::context::model_ptr_type make_model(const vertex_data_view &vertexDataView) const override;

        virtual shader_program_ptr_type make_shader(const std::string &aVertexGLSL, const std::string &aFragGLSL) const override;

        virtual graphics::context::texture_ptr_type make_texture(const texture::image_data_2d_view &imageView) const override;

        virtual graphics::context::built_in_shader_ptr_type get_alpha_cutoff_shader() const override;

        virtual built_in_shader_ptr_type get_pink_shader_of_death() const override;

        //! WebGLES2 impl of built in cube model
        virtual built_in_model_ptr_type get_cube_model() const override;
        
        //! WebGLES2 impl of built in quad model
        virtual built_in_model_ptr_type get_quad_model() const override;

        //! default ctor
        webgl1es2_context(); 

        //! default dtor
        virtual ~webgl1es2_context() override = default;
    };
}

#endif
