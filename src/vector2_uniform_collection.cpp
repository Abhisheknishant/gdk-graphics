// © 2018 Joseph Cameron - All Rights Reserved

#include <gdk/glh.h>
#include <gdk/opengl.h>
#include <gdk/vector2.h>
#include <gdk/vector2_uniform_collection.h>

using namespace gdk;

static constexpr char TAG[] = "vector2_uniform_collection";

void vector2_uniform_collection::bind(const GLuint aProgramHandle) const
{
    for (auto &pair : m_Map) glh::Bind2FloatUniform(aProgramHandle, pair.first, *pair.second.get());
}

void vector2_uniform_collection::unbind(const GLuint aProgramHandle) const
{
    for (auto &pair : m_Map) glh::Bind2FloatUniform(aProgramHandle, pair.first, graphics_vector2_type::Zero);
}
