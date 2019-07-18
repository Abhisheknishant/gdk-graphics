// © 2019 Joseph Cameron - All Rights Reserved

#include <string>

#include <jfc/catch.hpp>
#include <jfc/types.h>

#include "test_include.h"

#include <gdk/texture.h>
#include <gdk/shaderprogram.h>

using namespace gdk;

TEST_CASE("texture constructors", "[texture]")
{
    initGL();

    auto blar = std::shared_ptr<ShaderProgram>(ShaderProgram::AlphaCutOff);

    SECTION("Texture texture texture")
    {
        auto pTexture = static_cast<std::shared_ptr<Texture>>(Texture::CheckeredTextureOfDeath);

        REQUIRE(pTexture->getName() == "CheckerboardOfDeath");
    }
}

