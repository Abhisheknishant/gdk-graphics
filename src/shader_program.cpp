// © 2018 Joseph Cameron - All Rights Reserved

#include <gdkgraphics/buildinfo.h>

#include <gdk/glh.h>
#include <gdk/shader_program.h>

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace gdk;

static constexpr char TAG[] = "shader_program";

const jfc::lazy_ptr<gdk::shader_program> shader_program::PinkShaderOfDeath([]()
{
    const std::string vertexShaderSource(R"V0G0N(    
        //Uniforms
        uniform mat4 _MVP;
        #if defined Emscripten
        //VertIn
        attribute highp vec3 a_Position;

        #elif defined Darwin || defined Windows || defined Linux
        //VertIn
        attribute vec3 a_Position;
        #endif

        void main ()
        {
            gl_Position = _MVP * vec4(a_Position,1.0);
        }
    )V0G0N");

    const std::string fragmentShaderSource(R"V0G0N(
        #if defined Emscripten
        precision mediump float;
        #endif

        const vec4 DEATHLY_PINK = vec4(1,0.2,0.8,1);

        void main()
        {
            gl_FragColor = DEATHLY_PINK;
        }
    )V0G0N");

    return new gdk::shader_program(vertexShaderSource, fragmentShaderSource);
});

const jfc::lazy_ptr<gdk::shader_program> shader_program::AlphaCutOff([]()
{
    const std::string vertexShaderSource(R"V0G0N(
    //Uniforms
    uniform mat4 _MVP;
    uniform mat4 _Model; //separate mats arent used. should probably delete. they are useful though
    uniform mat4 _View;
    uniform mat4 _Projection;

    // Programmable stage input formats. Consider how to clean this up.. the webgl requirement of precison prefixes.
    #if defined Emscripten
    //VertIn
    attribute highp   vec3 a_Position;
    attribute mediump vec2 a_UV;
    //FragIn
    varying mediump vec2 v_UV;

    #elif defined Darwin || defined Windows || defined Linux
    //VertIn
    attribute vec3 a_Position;
    attribute vec2 a_UV;
    //FragIn
    varying vec2 v_UV;
    #endif

    void main ()
    {
        gl_Position = _MVP * vec4(a_Position,1.0);

        v_UV = a_UV;
    }
)V0G0N");

    const std::string fragmentShaderSource(R"V0G0N(
        #if defined Emscripten
        precision mediump float;
        #endif

        //Uniforms
        uniform sampler2D _Texture;
        #if defined Emscripten
        //FragIn
        varying lowp vec2 v_UV;

        #elif defined Darwin || defined Windows || defined Linux
        //FragIn
        varying vec2 v_UV;
        #endif

        void main()
        {
            vec4 frag = texture2D(_Texture, v_UV);

            if (frag[3] < 1.0) discard;

            gl_FragColor = frag;                        
        }
    )V0G0N");

    return new gdk::shader_program(vertexShaderSource, fragmentShaderSource);
});

static inline void setUpFaceCullingMode(shader_program::FaceCullingMode a)
{
    if (a == shader_program::FaceCullingMode::None)
    {
        glDisable(GL_CULL_FACE);

        return;
    }

    glEnable(GL_CULL_FACE);

    switch(a)
    {
        case shader_program::FaceCullingMode::Front: glCullFace(GL_FRONT); break;
        case shader_program::FaceCullingMode::Back: glCullFace(GL_BACK); break;
        case shader_program::FaceCullingMode::FrontAndBack: glCullFace(GL_FRONT_AND_BACK); break;

        case shader_program::FaceCullingMode::None: break;
    }
}

shader_program::shader_program(std::string aVertexSource, std::string aFragmentSource)
: m_ProgramHandle([&]()
{
    aVertexSource.insert  (0, std::string("#define ").append(gdkgraphics_BuildInfo_TargetPlatform).append("\n"));
    aFragmentSource.insert(0, std::string("#define ").append(gdkgraphics_BuildInfo_TargetPlatform).append("\n"));

#if defined JFC_TARGET_PLATFORM_Emscripten // version must be the first line in source. version must be present for WebGL platforms
    aVertexSource.insert  (0, std::string("#version 100\n"));
    aFragmentSource.insert(0, std::string("#version 100\n"));
#endif

    // Compile vertex stage
    const char *const vertex_shader = aVertexSource.c_str();
    const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, 0);
    glCompileShader(vs);
 
    // Compile fragment stage
    const char *const fragment_shader = aFragmentSource.c_str();
    const GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, 0);
    glCompileShader(fs);
    
    // Link the program
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vs);
    glAttachShader(programHandle, fs);
    glLinkProgram(programHandle);
    
    GLint status(-1);
    glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
    
    if (status == GL_FALSE)
    {
        std::ostringstream message;
        
        message 
            << "The shader has failed to compile!\n" 
            << "program compilation log: " << glh::GetProgramInfoLog(programHandle) << "\n"
            << "vertex shader compilation log:\n=-=-==-=-=--=\n" << glh::GetShaderInfoLog(vs) << "\n=-=-==-=-=--=\n" 
            << "fragment shader compilation log:\n=-=-==-=-=--=\n" << glh::GetShaderInfoLog(fs) << "\n=-=-==-=-=--=\n";

        throw std::runtime_error(std::string(TAG).append(message.str()));
    }

    return jfc::unique_handle<GLuint>(programHandle,
        [](const GLuint handle)
        {
            glDeleteProgram(handle);
        });
}())
{}

GLuint shader_program::useProgram() const 
{
    setUpFaceCullingMode(m_FaceCullingMode);

    glUseProgram(m_ProgramHandle.get());

    return m_ProgramHandle.get();
}

bool shader_program::operator==(const shader_program &b) const
{
    return m_ProgramHandle == b.m_ProgramHandle;
}
bool shader_program::operator!=(const shader_program &b) const {return !(*this == b);}
