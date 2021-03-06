// © 2018 Joseph Cameron - All Rights Reserved

#include <gdk/glh.h>
#include <gdk/webgl1es2_texture.h>

#include <stb/stb_image.h>

#include <cmath>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <stdexcept>
#include <type_traits>
#include <vector>

using namespace gdk;

static constexpr char TAG[] = "texture";

static inline bool isPowerOfTwo(const long a)
{
    return std::ceil(std::log2(a)) == std::floor(std::log2(a));
}

static inline GLint textureFormatToGLint(const webgl1es2_texture::format a)
{
    switch(a)
    {
        case webgl1es2_texture::format::rgba: return GL_RGBA;
        case webgl1es2_texture::format::rgb: return GL_RGB;
        case webgl1es2_texture::format::luminance_alpha: return GL_LUMINANCE_ALPHA;
        case webgl1es2_texture::format::luminance: return GL_LUMINANCE;
        case webgl1es2_texture::format::a: return GL_ALPHA;
    }
    
    throw std::runtime_error("unhandled format type");
}

static inline GLint minification_filter_to_glint(const webgl1es2_texture::minification_filter a)
{
    switch(a)
    {
        case webgl1es2_texture::minification_filter::linear: return GL_LINEAR;
        case webgl1es2_texture::minification_filter::nearest: return GL_NEAREST;
        case webgl1es2_texture::minification_filter::nearest_mipmap_nearest: return GL_NEAREST_MIPMAP_NEAREST;
        case webgl1es2_texture::minification_filter::linear_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
        case webgl1es2_texture::minification_filter::nearest_mipmap_linear: return GL_NEAREST_MIPMAP_LINEAR;
        case webgl1es2_texture::minification_filter::linear_mipmap_linear: return GL_LINEAR_MIPMAP_LINEAR;
    }
    
    throw std::runtime_error("unhandled minification filter");
}

static inline GLint magnification_filter_to_glint(const webgl1es2_texture::magnification_filter a)
{
    switch(a)
    {
        case webgl1es2_texture::magnification_filter::linear: return GL_LINEAR;
        case webgl1es2_texture::magnification_filter::nearest: return GL_NEAREST;
    }
    
    throw std::runtime_error("unhandled magnification filter");
}

static inline GLint wrap_mode_to_glint(const webgl1es2_texture::wrap_mode a)
{
    switch(a)
    {
        case webgl1es2_texture::wrap_mode::clamp_to_edge: return GL_CLAMP_TO_EDGE;
        case webgl1es2_texture::wrap_mode::repeat: return GL_REPEAT;
        case webgl1es2_texture::wrap_mode::mirrored_repeat: return GL_MIRRORED_REPEAT;
    }
    
    throw std::runtime_error("unhandled wrap mode");
}

static inline GLenum bind_target_to_glenum(const webgl1es2_texture::bind_target a)
{
    switch(a)
    {
        case webgl1es2_texture::bind_target::texture_2d: return GL_TEXTURE_2D;
        case webgl1es2_texture::bind_target::cube_map: return GL_TEXTURE_CUBE_MAP;
    }

    throw std::runtime_error("unhandled bind target");
}

const std::shared_ptr<gdk::webgl1es2_texture> webgl1es2_texture::GetTestTexture()
{
    static std::once_flag initFlag;

    static std::shared_ptr<gdk::webgl1es2_texture> ptr;

    std::call_once(initFlag, []()
    {
        std::vector<std::underlying_type<std::byte>::type> webgl1es2_textureData(
        {
            0xff, 0x00, 0x00, 0xff,
            0x00, 0xff, 0x00, 0xff,
            0x00, 0x00, 0xff, 0xff,
            0x00, 0xff, 0xff, 0xff,
        });
       
        webgl1es2_texture_2d_data_view_type data;
        data.width = 2;
        data.height = 2;
        data.format = format::rgba;
        data.data = reinterpret_cast<std::byte *>(&webgl1es2_textureData[0]);

        ptr = std::make_shared<gdk::webgl1es2_texture>(data);
    });
    
    return ptr;
}

const std::shared_ptr<gdk::webgl1es2_texture> webgl1es2_texture::GetCheckerboardOfDeath()
{
    static std::once_flag initFlag;

    static std::shared_ptr<gdk::webgl1es2_texture> ptr;

    std::call_once(initFlag, []()
    {
        std::vector<GLubyte> webgl1es2_textureData( //TODO think about replacing this with much less data. This data repeats; its redundant //TODO replace this with a generated raw image. Move this png to a test.
        {
            0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
            0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08,
            0x08, 0x02, 0x00, 0x00, 0x00, 0x4b, 0x6d, 0x29, 0xdc, 0x00, 0x00, 0x00,
            0x01, 0x73, 0x52, 0x47, 0x42, 0x00, 0xae, 0xce, 0x1c, 0xe9, 0x00, 0x00,
            0x00, 0x04, 0x67, 0x41, 0x4d, 0x41, 0x00, 0x00, 0xb1, 0x8f, 0x0b, 0xfc,
            0x61, 0x05, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00,
            0x0e, 0xc3, 0x00, 0x00, 0x0e, 0xc3, 0x01, 0xc7, 0x6f, 0xa8, 0x64, 0x00,
            0x00, 0x00, 0x1b, 0x49, 0x44, 0x41, 0x54, 0x18, 0x57, 0x63, 0xf8, 0xff,
            0xff, 0xff, 0xcc, 0x9b, 0xaf, 0x30, 0x49, 0x06, 0xac, 0xa2, 0x40, 0x72,
            0x30, 0xea, 0xf8, 0xff, 0x1f, 0x00, 0xd3, 0x06, 0xab, 0x21, 0x92, 0xd9,
            0xa4, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42,
            0x60, 0x82
        });
        
        ptr = std::make_shared<gdk::webgl1es2_texture>(webgl1es2_texture::make_from_png_rgba32(webgl1es2_textureData));
    });
    
    return ptr;
};

webgl1es2_texture webgl1es2_texture::make_from_png_rgba32(const std::vector<GLubyte> awebgl1es2_textureData)
{
    //decode the png rgba32 data
    int width, height, components;

    if (std::unique_ptr<GLubyte, std::function<void(GLubyte *)>> decodedData(
        stbi_load_from_memory(&awebgl1es2_textureData[0]
            , static_cast<int>(awebgl1es2_textureData.size())
            , &width
            , &height
            , &components
            , STBI_rgb_alpha)
        , [](GLubyte *p)
        {
            stbi_image_free(p);
        }); decodedData)
    {
        webgl1es2_texture_2d_data_view_type data;
        data.width = width;
        data.height = height;
        data.format = format::rgba;
        data.data = reinterpret_cast<std::byte *>(decodedData.get());
        
        return webgl1es2_texture(data);
    }
    
    throw std::runtime_error(std::string(TAG).append(": could not decode RGBA32 data provided to webgl1es2_texture"));
}

webgl1es2_texture::webgl1es2_texture(const webgl1es2_texture_2d_data_view_type &textureData2d,
    const minification_filter minFilter,
    const magnification_filter magFilter,
    const wrap_mode wrapMode)
: m_BindTarget(bind_target_to_glenum(bind_target::texture_2d))    
, m_Handle([&]()
{
    // TODO Should reenable this
    /*if (!isPowerOfTwo(textureData2d.width) || !isPowerOfTwo(textureData2d.height)) 
        throw std::invalid_argument(std::string(TAG).append(": webgl1es2_texture dimensions must be power of 2"));*/

    //TODO
    //get max texture size, assert we are less than that.

    GLuint handle;

    glGenTextures(1, &handle);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(m_BindTarget, handle);

    glTexImage2D(m_BindTarget, 
        0, 
        textureFormatToGLint(textureData2d.format), 
        textureData2d.width, 
        textureData2d.height, 
        0, 
        textureFormatToGLint(textureData2d.format), 
        GL_UNSIGNED_BYTE, 
        const_cast<GLubyte *>(reinterpret_cast<const GLubyte *>(&textureData2d.data[0])));

    //Select webgl1es2_texture filter functions
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnification_filter_to_glint(magFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minification_filter_to_glint(minFilter));
   
    //Set wrap modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode_to_glint(wrapMode));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode_to_glint(wrapMode));

    return handle;
}(),
[](const GLuint handle)
{
    glDeleteTextures(1, &handle);
})
{}

GLuint webgl1es2_texture::getHandle() const
{
    return m_Handle.get();
}

bool webgl1es2_texture::operator==(const webgl1es2_texture &b) const
{
    return m_Handle == b.m_Handle;
}
bool webgl1es2_texture::operator!=(const webgl1es2_texture &b) const { return !(*this == b); }

