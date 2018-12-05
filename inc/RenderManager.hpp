//
// Created by Remi DAVID on 10/2/18.
//

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

# define CORE_HPP
# if defined(__APPLE_CC__)
#  ifndef GLFW_INCLUDE_GLCOREARB
#   define GLFW_INCLUDE_GLCOREARB
#  endif
#  ifndef GLFW_INCLUDE_GLEXT
#   define GLFW_INCLUDE_GLEXT
#  endif
# else
#  define GL_GLEXT_PROTOTYPES
# endif

#include <GLM/ext.hpp>
#include <GLM/detail/type_mat4x4.hpp>
#include <stack>
#include "IRenderedObject.hpp"
#include "Constant.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "ShaderManager.hpp"
#include "Core.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

class RenderManager;

typedef void (* CoreTickCallback)(void *core, RenderManager *render_manager);

class RenderManager
{
public:
    /// Attribute Locations for every shader:
    static const GLuint position_loc = 0;
    static const GLuint UV_loc = 1;
    static const GLuint normal_loc = 2;
    static const GLuint color_loc = 1;

    double          frame_per_second = 0.0f;
    glm::vec3       light_pos;
    bool            render_axes_on = false;

    GLFWwindow      *window;

    GLuint          res_x;
    GLuint          res_y;

    Camera          camera;

    RenderManager();
    ~RenderManager();

    void init(void *core_ptr, GLFWkeyfun keyCallback, GLFWcursorposfun cursorPosCallback, GLFWmousebuttonfun mouseButtonCallback);

    void start(void *core_ptr, CoreTickCallback coreTickCallback);

    int  initGLFW(void *core_ptr, GLFWkeyfun keyCallback, GLFWcursorposfun cursorPosCallback, GLFWmousebuttonfun mouseButtonCallback);

    void cyclePolygonRenderMode();

    void updateCamera();

    bool loadShaders();

    void loadTextures();

    int addRenderedObject(IRenderedObject &rendered_object);

    void removeRenderedObject(int rendered_object_id);

    void renderObjects();

    glm::vec3 const &getCameraPosition() const;

    Camera const *getCamera() const;

    GLuint    getTextureIDs(GLuint) const;

    GLuint    getShaderProgramID(GLuint index) const;

    GLenum const & getPolygonMode() const;

private:
    ShaderManager   _shader_manager;
    GLuint          _shader_program_ids[SHADER_NUM];

    TextureManager  _texture_manager;
    GLuint          _texture_ids[TEX_NUM]; /// Do not use as texture index, use enum.

    std::vector<IRenderedObject *> rendered_objects;

    GLenum          _polygon_mode = GL_FILL;
};


#endif //RENDER_MANAGER_H
