// © 2018 Joseph Cameron - All Rights Reserved

#ifndef GDK_GLH_H
#define GDK_GLH_H

//! OpenGL header for gdk
/// includes gl functions from glew and declares C++ friendly helpers in the GLH namespace

#include <gdk/opengl.h>
#include <gdk/graphics_types.h>

#include <string>
#include <string_view>

namespace gdk
{
    struct Color;
}

namespace glh
{
    // gdk-type friendly conveniences
    void ClearColor(const gdk::Color &aColor);
    void Viewport(const gdk::graphics_intvector2_type &aPos, const gdk::graphics_intvector2_type &aSize);
    void Scissor(const gdk::graphics_intvector2_type &aPos, const gdk::graphics_intvector2_type &aSize);
    
    // Error detection & logging
    std::string GetShaderInfoLog(const GLuint aShaderStageHandle);
    std::string GetProgramInfoLog(const GLuint aShaderProgramHandle);
    bool GetError(std::string *aErrorCode = nullptr);
/*    //std::vector<std::string> GetErrors();
    //void LogErrors(const bool &aDoNotLogIfNoErrors = false);
    void ClearErrors();*/

    //! Vertex binding
    bool EnableVertexAttribute(const std::string_view &aAttributeName, const GLuint aProgramHandle, 
        const int aAttributeSize, const int aAttributeOffset, const int aTotalNumberOfVertexAttributeComponents);
    
    bool BindTextureUniform(const GLuint aShaderHandle, const std::string_view &aUniformName, const GLuint aTextureHandle,
        const int aTextureUnit);//, final GLenum &aTextureType);

    bool Bind1FloatUniform (const GLuint aShaderHandle, const std::string_view &aUniformName, const float aScalar);
    bool Bind2FloatUniform (const GLuint aShaderHandle, const std::string_view &aUniformName, 
        const gdk::graphics_vector2_type &agraphics_vector2_type);

    bool Bind3FloatUniform (const GLuint aShaderHandle, const std::string_view &aUniformName, 
        const gdk::graphics_vector3_type &agraphics_vector3_type);

    bool Bind4FloatUniform (const GLuint aShaderHandle, const std::string_view &aUniformName, 
        const gdk::graphics_vector4_type &agraphics_vector4_type);

    bool BindMatrix4x4(const GLuint aShaderHandle, const std::string_view &aUniformName, const gdk::graphics_mat4x4_type &aMatrix4x4);
}

#endif
