//
// Created by Hugo JEANNIN on 3/10/17.
//

#include "../inc/Core.hpp"

void
Core::handleKeyEvent(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(render_manager.window, GL_TRUE);
    }
    if (key == GLFW_KEY_C && action == GLFW_RELEASE)
    {
        render_manager.camera.writeConfig();
    }
    if (key == GLFW_KEY_V && action == GLFW_RELEASE)
    {
        render_manager.camera.loadConfig();
    }
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
    {
        render_manager.render_axes_on = !render_manager.render_axes_on;
    }
    if (key == GLFW_KEY_KP_MULTIPLY && action == GLFW_RELEASE)
    {
        render_manager.camera.toggleMegaBoostSpeed();
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        animation_manager.loadMinimal();
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        animation_manager.loadTruc();
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    {
        animation_manager.applyMinimalStep1();
    }
    if (key == GLFW_KEY_L && action == GLFW_RELEASE)
    {
        animation_manager.applyMinimalStep2();
    }
    if (key == GLFW_KEY_K && action == GLFW_RELEASE)
    {
        animation_manager.clearEverything();
    }
    if (key == GLFW_KEY_BACKSPACE && action == GLFW_RELEASE)
    {
        animation_manager.stopAllAnimation();
    }
}

void
Core::handleCursorEvent(double xpos, double ypos)
{
    render_manager.camera.vertical_angle -= ((ypos - render_manager.res_y * 0.5f) * render_manager.camera.rotation_speed * 0.2f);
    if (render_manager.camera.vertical_angle > M_PI_2 - 0.01f)
        render_manager.camera.vertical_angle = (float) M_PI_2 - 0.01f;
    else if (render_manager.camera.vertical_angle < -(M_PI_2 - 0.01f))
        render_manager.camera.vertical_angle = (float) -(M_PI_2 - 0.01f);
    render_manager.camera.horizontal_angle -= ((xpos - render_manager.res_x * 0.5f) * render_manager.camera.rotation_speed * 0.2f);
    render_manager.camera.horizontal_angle = (float) fmod(render_manager.camera.horizontal_angle, M_PI * 2);
    glfwSetCursorPos(render_manager.window, render_manager.res_x * 0.5f, render_manager.res_y * 0.5f);
}

void
Core::handleMouseButtonCallback(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
    }
}

static void
keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void) scancode;
    (void) mods;

    Core *core = reinterpret_cast<Core *>(glfwGetWindowUserPointer(window));

    core->handleKeyEvent(key, scancode, action, mods);
}

static void
cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{/*
    Core *core = reinterpret_cast<Core *>(glfwGetWindowUserPointer(window));

    camera.updateMouse(xpos, ypos);
    glfwSetCursorPos(window, res_x / 2, res_y / 2);*/

    Core		*core = static_cast<Core *>(glfwGetWindowUserPointer(window));

    core->handleCursorEvent(xpos, ypos);
}
static void
mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Core		*core = static_cast<Core *>(glfwGetWindowUserPointer(window));

    core->handleMouseButtonCallback(button, action, mods);
}
static void
tickCallback(void *core_ptr, RenderManager *render_manager)
{
    Core        *core = static_cast<Core *>(core_ptr);

    core->update(render_manager);
}

Core::Core() : _show_debug_info(false)
{

}

Core::~Core() = default;

void
Core::init()
{
    game_logic.init();
    cube.game_logic = &game_logic;
//    cl_wrapper.init();
    animation_manager.init();
    tree.linked_matrix = &animation_manager.v_model_attrib[0].matrix;
    tree_2.linked_matrix = &animation_manager.v_model_attrib[1].matrix;

    render_manager.init(this, keyCallback, cursorPosCallback, mouseButtonCallback);

    render_manager.addRenderedObject((IRenderedObject &)skybox);
    render_manager.addRenderedObject((IRenderedObject &)axes);
    render_manager.addRenderedObject((IRenderedObject &)fps_counter);
//    render_manager.addRenderedObject((IRenderedObject &)tree);
//    render_manager.addRenderedObject((IRenderedObject &)tree_2);
    render_manager.addRenderedObject((IRenderedObject &)cube);

    render_manager.start(this, tickCallback);
}

void
Core::update(RenderManager *render_manager)
{
    fps_counter.text = "FPS: " + std::to_string((int) render_manager->frame_per_second) + "\n";
    animation_manager.update();
}
