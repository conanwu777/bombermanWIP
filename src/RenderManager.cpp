//
// Created by Remi DAVID on 10/2/18.
//

#define _USE_MATH_DEFINES
#include <cmath>

#include "../inc/RenderManager.hpp"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
    glDeleteTextures(TEX_NUM, _texture_ids);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void
RenderManager::init(void *core_ptr, GLFWkeyfun keyCallback, GLFWcursorposfun cursorPosCallback, GLFWmousebuttonfun mouseButtonCallback)
{
    res_x = INITIAL_RES_X;
    res_y = INITIAL_RES_Y;
    camera.init();
    light_pos = camera.pos;

    initGLFW(core_ptr, keyCallback, cursorPosCallback, mouseButtonCallback);

    loadTextures();
    if (!loadShaders())
        return;
    glEnable(GL_DEPTH_TEST);

    // Face Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _polygon_mode = GL_FILL;
}

void
RenderManager::start(void *core_ptr, CoreTickCallback coreTickCallback)
{
    double begin_time, update_time, frame_time;
    double next_string_update, next_env_update;
    double frame_counter, update_counter;
    double tick_length;
    int loops;
    int max_frame_skip;
    uint32_t current_tick;
    std::string title;

    update_time = 0.0f;
    frame_counter = 0.0f;
    update_counter = 0.0f;
    tick_length = 1.0f / TICK_PER_SECOND;
    max_frame_skip = 5;
    next_env_update = glfwGetTime();
    next_string_update = glfwGetTime();
    current_tick = 0;

    while (!glfwWindowShouldClose(window))
    {
        ////////////
        // Update //
        ////////////

        loops = 0;
        // Tant que tu depasse le timing d'update tu skip des frames et au max 5 frame.
        while (glfwGetTime() > next_env_update && loops < max_frame_skip)
        {
            begin_time = glfwGetTime();

            glfwPollEvents();
            update_counter++;
            next_env_update += tick_length;
            loops++;

            coreTickCallback(core_ptr, this);

            updateCamera();

            /// Put torchlight in front of the camera
            light_pos = camera.pos;

            current_tick++;
            update_time = glfwGetTime();
            update_time -= begin_time;
        }

        //////////////////
        // Render Frame //
        //////////////////

        begin_time = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderObjects();

        glfwSwapBuffers(window);

        frame_counter++;
        frame_time = glfwGetTime();
        frame_time -= begin_time;

        //////////////////
        // Title Update //
        //////////////////

        if (glfwGetTime() - next_string_update >= 1.0f)
        {
            frame_per_second = frame_counter;
            next_string_update += 1.0f;
            title.clear();
            title.append("FPS: ");
            title.append(std::to_string((int) frame_counter));
            title.append(" - ");
            title.append(std::to_string(frame_time));
            title.append("ms");
            title.append(" ");
            title.append("UPS: ");
            title.append(std::to_string((int) update_counter));
            title.append(" - ");
            title.append(std::to_string(update_time));
            title.append("ms");
            title.append(" ");
            title.append("Camera: [");
            title.append(std::to_string(camera.pos.x));
            title.append(", ");
            title.append(std::to_string(camera.pos.y));
            title.append(", ");
            title.append(std::to_string(camera.pos.z));
            title.append("] ");
            title.append("Front: [");
            title.append(std::to_string(camera.front.x));
            title.append(", ");
            title.append(std::to_string(camera.front.y));
            title.append(", ");
            title.append(std::to_string(camera.front.z));
            title.append("] ");
            title.append(" Time: ");
            title.append(std::to_string(glfwGetTime()));
            glfwSetWindowTitle(window, title.c_str());
            frame_counter = 0.0;
            update_counter = 0.0;
        }
    }
}

int
RenderManager::initGLFW(void *core_ptr, GLFWkeyfun keyCallback, GLFWcursorposfun cursorPosCallback, GLFWmousebuttonfun mouseButtonCallback)
{
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, SAMPLE_RATE);
    if (FULL_SCREEN)
        window = glfwCreateWindow(res_x, res_y, "Bomberman (PROBABLY WANT TO BE MORE CREATIVE)", glfwGetPrimaryMonitor(), nullptr);
    else
        window = glfwCreateWindow(res_x, res_y, "Bomberman (PROBABLY WANT TO BE MORE CREATIVE)", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowUserPointer(window, core_ptr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED | GLFW_CURSOR_HIDDEN);

#if defined(_WIN32)
    if (glewInit() != GLEW_OK) {
        std::cerr << "Could not initialize Glew!" << std::endl;
        return false;
    }
#endif
    return 0;
}

void
RenderManager::cyclePolygonRenderMode()
{
    switch (_polygon_mode)
    {
        case GL_FILL:
            _polygon_mode = GL_LINE;
            break;
        case GL_LINE:
            _polygon_mode = GL_POINT;
            break;
        case GL_POINT:
            _polygon_mode = GL_FILL;
            break;
        default:
            _polygon_mode = GL_FILL;
            break;
    }
}

void
RenderManager::updateCamera()
{
    camera.enableBoostSpeed(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveBackward();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.strafeLeft();
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.strafeRight();
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        camera.moveUp();
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        camera.moveDown();
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.rotateLeft();
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.rotateRight();
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.rotateUp();
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.rotateDown();

    camera.updateMatrix();
}

bool
RenderManager::loadShaders()
{
    if (!_shader_manager.read("../shaders/axes.vertex.glsl", 0, "../shaders/axes.fragment.glsl"))
        return false;
    _shader_manager.compile();
    _shader_program_ids[AXES_SHADER] = glCreateProgram();
    _shader_manager.linkProgram(&_shader_program_ids[AXES_SHADER]);

    if (!_shader_manager.read("../shaders/skybox.vertex.glsl", 0, "../shaders/skybox.fragment.glsl"))
        return false;
    _shader_manager.compile();
    _shader_program_ids[SKYBOX_SHADER] = glCreateProgram();
    _shader_manager.linkProgram(&_shader_program_ids[SKYBOX_SHADER]);

    if (!_shader_manager.read("../shaders/text.vertex.glsl", 0, "../shaders/text.fragment.glsl"))
        return false;
    _shader_manager.compile();
    _shader_program_ids[TEXT_SHADER] = glCreateProgram();
    _shader_manager.linkProgram(&_shader_program_ids[TEXT_SHADER]);

    if (!_shader_manager.read("../shaders/obj.vertex.glsl", 0, "../shaders/obj.fragment.glsl"))
        return false;
    _shader_manager.compile();
    _shader_program_ids[OBJ_SHADER] = glCreateProgram();
    _shader_manager.linkProgram(&_shader_program_ids[OBJ_SHADER]);

    return true;
}

void
RenderManager::loadTextures()
{
    // Can't load texture before creating buffers.
    glGenTextures(TEX_NUM, _texture_ids);

    glActiveTexture(GL_TEXTURE0);
    _texture_manager.generateDefaultTexture(_texture_ids[DEFAULT_GENERATED_TEXTURE]);
    glActiveTexture(GL_TEXTURE1);
    std::vector<const GLchar*> faces_sleepyhollow;
    faces_sleepyhollow.push_back("../assets/texture/skybox/desertblue/mystic_rt.jpg");
    faces_sleepyhollow.push_back("../assets/texture/skybox/desertblue/mystic_lf.jpg");
    faces_sleepyhollow.push_back("../assets/texture/skybox/desertblue/mystic_up.jpg");
    faces_sleepyhollow.push_back("../assets/texture/skybox/desertblue/mystic_dn.jpg");
    faces_sleepyhollow.push_back("../assets/texture/skybox/desertblue/mystic_bk.jpg");
    faces_sleepyhollow.push_back("../assets/texture/skybox/desertblue/mystic_ft.jpg");
    _texture_manager.loadCubemap(_texture_ids[SKYBOX_TEXTURE], faces_sleepyhollow);
    glActiveTexture(GL_TEXTURE2);
    _texture_manager.loadImageFile(_texture_ids[CHECKER_TEXTURE], "../assets/texture/checker.png");
}

int
RenderManager::addRenderedObject(IRenderedObject &rendered_object)
{
    rendered_object.initGL(*this);
    rendered_objects.push_back(&rendered_object);
    return 0;
}

void
RenderManager::removeRenderedObject(int rendered_object_id)
{

}

void
RenderManager::renderObjects()
{
    for (std::vector<IRenderedObject *>::iterator it = rendered_objects.begin(); it != rendered_objects.end(); ++it)
    {
        glUseProgram((*it)->getProgramID());
        (*it)->preRender(*this);
        (*it)->render(*this);
    }
}

glm::vec3 const &RenderManager::getCameraPosition() const
{
    return camera.pos;
}

const Camera *RenderManager::getCamera() const
{
    return &camera;
}

GLuint
RenderManager::getShaderProgramID(GLuint index) const
{
    return _shader_program_ids[index];
}

GLuint
RenderManager::getTextureIDs(GLuint index) const
{
    return _texture_ids[index];
}

GLenum const &
RenderManager::getPolygonMode() const
{
    return _polygon_mode;
}
