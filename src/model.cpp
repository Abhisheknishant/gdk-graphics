// © 2018 Joseph Cameron - All Rights Reserved

#include <jfc/default_ptr.h>
#include <gdk/glh.h>
#include <gdk/mat4x4.h>
#include <gdk/model.h>
#include <gdk/nlohmann_json_util.h>
#include <gdk/opengl.h>
#include <gdk/shader_program.h>
#include <gdk/vertex_data.h>

#include <nlohmann/json.hpp>

#include <iostream>

using namespace gdk;

static constexpr char TAG[] = "model";

model::model(const std::shared_ptr<vertex_data> avertex_data, const std::shared_ptr<shader_program> ashader_program)
: m_vertex_data(avertex_data)
, m_ShaderProgram(ashader_program)
{}

void model::draw(const double aTimeSinceStart, const double aDeltaTime, const graphics_mat4x4_type &aViewMatrix, const graphics_mat4x4_type &aProjectionMatrix) const
{
    auto pvertex_data = m_vertex_data;
        
    const GLuint programHandle = m_ShaderProgram->useProgram();

    //bind this model's uniforms
    m_textures.bind(programHandle);
    m_Floats.bind(programHandle);
    m_Vector2Uniforms.bind(programHandle);
    m_Vector3Uniforms.bind(programHandle);
    m_Vector4Uniforms.bind(programHandle);
    m_Mat4x4Uniforms.bind(programHandle);

    //bind standard uniforms
    const float time = aTimeSinceStart;
    const float deltaTime = aDeltaTime;

    const graphics_mat4x4_type p = aProjectionMatrix;
    const graphics_mat4x4_type v = aViewMatrix;
    const graphics_mat4x4_type m = getmodelMatrix();

    const auto mvp = p * v * m;

    // these uniforms belong to a higherlevel abstraction.
    glh::Bind1FloatUniform(programHandle, "_DeltaTime",  deltaTime);
    glh::Bind1FloatUniform(programHandle, "_Time",       time     );

    // these uniforms are good
    glh::BindMatrix4x4(programHandle,     "_model",      m        );
    glh::BindMatrix4x4(programHandle,     "_View",       v        );
    glh::BindMatrix4x4(programHandle,     "_Projection", p        );
    glh::BindMatrix4x4(programHandle,     "_MVP",        mvp      );

    pvertex_data->draw(programHandle);
}

// Accessors
void model::set_texture(const std::string &aUniformName, const jfc::default_ptr<texture> &atexture)
{
    m_textures.insert(aUniformName, atexture);
}

void model::setFloat(const std::string &aUniformName, const std::shared_ptr<float> &aFloat)
{
    m_Floats.insert(aUniformName, aFloat);
}

void model::setVector2(const std::string &aUniformName, const std::shared_ptr<graphics_vector2_type> &agraphics_vector2_type)
{
    m_Vector2Uniforms.insert(aUniformName, agraphics_vector2_type);
}

void model::setVector3(const std::string &aUniformName, const std::shared_ptr<graphics_vector3_type> &agraphics_vector3_type)
{
    m_Vector3Uniforms.insert(aUniformName, agraphics_vector3_type);
}

void model::setVector4(const std::string &aUniformName, const std::shared_ptr<graphics_vector4_type> &agraphics_vector4_type)
{
    m_Vector4Uniforms.insert(aUniformName, agraphics_vector4_type);
}

void model::setMat4x4(const std::string &aUniformName, const graphics_mat4x4_type &agraphics_mat4x4_type )
{
    m_Mat4x4Uniforms.insert(aUniformName, agraphics_mat4x4_type);
}

const graphics_mat4x4_type& model::getmodelMatrix() const
{
    return m_modelMatrix;
}

void model::set_model_matrix(const graphics_vector3_type &aWorldPos, const graphics_quaternion_type &aRotation, const graphics_vector3_type &aScale)
{
    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(aWorldPos);
    m_modelMatrix.rotate(aRotation);
    m_modelMatrix.scale(aScale);
}

void model::setvertex_data(const std::shared_ptr<vertex_data> a)
{
    m_vertex_data = a;
}

