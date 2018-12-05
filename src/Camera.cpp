//
// Created by Hugo JEANNIN on 3/11/17.
//

#define _USE_MATH_DEFINES
#include <cmath>

#include "../inc/Camera.hpp"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void
Camera::init()
{
    pos = {0.0f, 0.0, 0.0f};
    front = {0.0f, 0.0f, -1.0f};
    up = {0.0f, 1.0f, 0.0f};
    normal_speed = 1.0f / TPS;
    boost_speed = 20.0f / TPS;
    mega_boost_speed = 200.0f / TPS;
    movement_speed = boost_speed;
    rotation_speed = 0.02f;
    horizontal_angle = 0.0f;
    vertical_angle = 0.0f;
    mouse_speed = 0.002f;
    perspective_matrix = glm::perspective((float) M_PI_4, 16.0f / 9.0f, 0.5f, 100.0f);
    screen_center.x = INITIAL_RES_X / 2.0f;
    screen_center.y = INITIAL_RES_Y / 2.0f;
    if (CAMERA_CFG_AUTO_LOAD)
        loadConfig();
    updateMatrix();
}

void
Camera::updateMatrix()
{
    front = {0.0f, 0.0f, -1.0f};
    front = glm::rotateX(front, vertical_angle);
    front = glm::rotateY(front, horizontal_angle);
    pos_and_front = pos;
    pos_and_front += front;
    view_matrix = glm::lookAt(pos, pos_and_front, up);
    frustum(view_matrix, perspective_matrix);
}

void
Camera::enableBoostSpeed(bool enable)
{
    if (movement_speed != mega_boost_speed)
        movement_speed = enable ? normal_speed : boost_speed;
}

void
Camera::toggleMegaBoostSpeed()
{
    movement_speed = movement_speed == mega_boost_speed ? normal_speed : mega_boost_speed;
}

void
Camera::moveForward()
{
    move_forward = front;
    move_forward *= movement_speed;
    pos += move_forward;
}

void
Camera::moveBackward()
{
    move_forward = front;
    move_forward *= movement_speed;
    pos -= move_forward;
}

void
Camera::strafeLeft()
{
    move_left = glm::normalize(glm::cross(front, up));
    move_left *= movement_speed;
    pos -= move_left;
}

void
Camera::strafeRight()
{
    move_left = glm::normalize(glm::cross(front, up));
    move_left *= movement_speed;
    pos += move_left;
}

void
Camera::moveUp()
{
    move_up = up;
    move_up *= movement_speed;
    pos += move_up;
}

void
Camera::moveDown()
{
    move_up = up;
    move_up *= movement_speed;
    pos -= move_up;
}

void
Camera::rotateLeft()
{
    horizontal_angle += rotation_speed;
}

void
Camera::rotateRight()
{
    horizontal_angle -= rotation_speed;
}

void
Camera::rotateUp()
{
    vertical_angle += rotation_speed;
    if (vertical_angle >= 1.4f)
        vertical_angle = 1.4f;
}

void
Camera::rotateDown()
{
    vertical_angle -= rotation_speed;
    if (vertical_angle <= -1.4f)
        vertical_angle = -1.4f;
}

void
Camera::frustum(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
        const glm::mat4 &v = viewMatrix;
        const glm::mat4 &p = projectionMatrix;

        glm::mat4 clipMatrix;

        clipMatrix[0][0] = v[0][0]*p[0][0]+v[0][1]*p[1][0]+v[0][2]*p[2][0]+v[0][3]*p[3][0];
        clipMatrix[1][0] = v[0][0]*p[0][1]+v[0][1]*p[1][1]+v[0][2]*p[2][1]+v[0][3]*p[3][1];
        clipMatrix[2][0] = v[0][0]*p[0][2]+v[0][1]*p[1][2]+v[0][2]*p[2][2]+v[0][3]*p[3][2];
        clipMatrix[3][0] = v[0][0]*p[0][3]+v[0][1]*p[1][3]+v[0][2]*p[2][3]+v[0][3]*p[3][3];
        clipMatrix[0][1] = v[1][0]*p[0][0]+v[1][1]*p[1][0]+v[1][2]*p[2][0]+v[1][3]*p[3][0];
        clipMatrix[1][1] = v[1][0]*p[0][1]+v[1][1]*p[1][1]+v[1][2]*p[2][1]+v[1][3]*p[3][1];
        clipMatrix[2][1] = v[1][0]*p[0][2]+v[1][1]*p[1][2]+v[1][2]*p[2][2]+v[1][3]*p[3][2];
        clipMatrix[3][1] = v[1][0]*p[0][3]+v[1][1]*p[1][3]+v[1][2]*p[2][3]+v[1][3]*p[3][3];
        clipMatrix[0][2] = v[2][0]*p[0][0]+v[2][1]*p[1][0]+v[2][2]*p[2][0]+v[2][3]*p[3][0];
        clipMatrix[1][2] = v[2][0]*p[0][1]+v[2][1]*p[1][1]+v[2][2]*p[2][1]+v[2][3]*p[3][1];
        clipMatrix[2][2] = v[2][0]*p[0][2]+v[2][1]*p[1][2]+v[2][2]*p[2][2]+v[2][3]*p[3][2];
        clipMatrix[3][2] = v[2][0]*p[0][3]+v[2][1]*p[1][3]+v[2][2]*p[2][3]+v[2][3]*p[3][3];
        clipMatrix[0][3] = v[3][0]*p[0][0]+v[3][1]*p[1][0]+v[3][2]*p[2][0]+v[3][3]*p[3][0];
        clipMatrix[1][3] = v[3][0]*p[0][1]+v[3][1]*p[1][1]+v[3][2]*p[2][1]+v[3][3]*p[3][1];
        clipMatrix[2][3] = v[3][0]*p[0][2]+v[3][1]*p[1][2]+v[3][2]*p[2][2]+v[3][3]*p[3][2];
        clipMatrix[3][3] = v[3][0]*p[0][3]+v[3][1]*p[1][3]+v[3][2]*p[2][3]+v[3][3]*p[3][3];

        m_planes[PLANE_RIGHT].x = clipMatrix[3][0]-clipMatrix[0][0];
        m_planes[PLANE_RIGHT].y = clipMatrix[3][1]-clipMatrix[0][1];
        m_planes[PLANE_RIGHT].z = clipMatrix[3][2]-clipMatrix[0][2];
        m_planes[PLANE_RIGHT].w = clipMatrix[3][3]-clipMatrix[0][3];

        m_planes[PLANE_LEFT].x = clipMatrix[3][0]+clipMatrix[0][0];
        m_planes[PLANE_LEFT].y = clipMatrix[3][1]+clipMatrix[0][1];
        m_planes[PLANE_LEFT].z = clipMatrix[3][2]+clipMatrix[0][2];
        m_planes[PLANE_LEFT].w = clipMatrix[3][3]+clipMatrix[0][3];

        m_planes[PLANE_BOTTOM].x = clipMatrix[3][0]+clipMatrix[1][0];
        m_planes[PLANE_BOTTOM].y = clipMatrix[3][1]+clipMatrix[1][1];
        m_planes[PLANE_BOTTOM].z = clipMatrix[3][2]+clipMatrix[1][2];
        m_planes[PLANE_BOTTOM].w = clipMatrix[3][3]+clipMatrix[1][3];

        m_planes[PLANE_TOP].x = clipMatrix[3][0]-clipMatrix[1][0];
        m_planes[PLANE_TOP].y = clipMatrix[3][1]-clipMatrix[1][1];
        m_planes[PLANE_TOP].z = clipMatrix[3][2]-clipMatrix[1][2];
        m_planes[PLANE_TOP].w = clipMatrix[3][3]-clipMatrix[1][3];

        m_planes[PLANE_BACK].x = clipMatrix[3][0]-clipMatrix[2][0];
        m_planes[PLANE_BACK].y = clipMatrix[3][1]-clipMatrix[2][1];
        m_planes[PLANE_BACK].z = clipMatrix[3][2]-clipMatrix[2][2];
        m_planes[PLANE_BACK].w = clipMatrix[3][3]-clipMatrix[2][3];

        m_planes[PLANE_FRONT].x = clipMatrix[3][0]+clipMatrix[2][0];
        m_planes[PLANE_FRONT].y = clipMatrix[3][1]+clipMatrix[2][1];
        m_planes[PLANE_FRONT].z = clipMatrix[3][2]+clipMatrix[2][2];
        m_planes[PLANE_FRONT].w = clipMatrix[3][3]+clipMatrix[2][3];

        for (int i = 0; i < 6; i++)
            m_planes[i] = glm::normalize(m_planes[i]);
}

void
Camera::print()
{
    std::cout << glm::to_string(pos) << std::endl;
    std::cout << glm::to_string(front) << std::endl;
    std::cout << glm::to_string(up) << std::endl;
    std::cout << horizontal_angle << std::endl;
    std::cout << vertical_angle << std::endl;
    std::cout << mouse_speed << std::endl;
    std::cout << glm::to_string(perspective_matrix) << std::endl;
}

void Camera::writeConfig()
{
    std::ofstream cacfg;

    cacfg.open("../config/camera.cfg");
    if (cacfg.is_open())
    {
        cacfg << pos.x << std::endl;
        cacfg << pos.y << std::endl;
        cacfg << pos.z << std::endl;
        cacfg << front.x << std::endl;
        cacfg << front.y << std::endl;
        cacfg << front.z << std::endl;
        cacfg << horizontal_angle << std::endl;
        cacfg << vertical_angle << std::endl;
        cacfg.close();
    }
    else
    {
        std::cerr << "Failed to write Camera config." << std::endl;
    }
}

void Camera::loadConfig()
{
    std::string line;
    std::ifstream cacfg("../config/camera.cfg");

    if (cacfg.is_open())
    {
        getline(cacfg, line);
        pos.x = std::stof(line, 0);
        getline(cacfg, line);
        pos.y = std::stof(line, 0);
        getline(cacfg, line);
        pos.z = std::stof(line, 0);
        getline(cacfg, line);
        front.x = std::stof(line, 0);
        getline(cacfg, line);
        front.y = std::stof(line, 0);
        getline(cacfg, line);
        front.z = std::stof(line, 0);
        getline(cacfg, line);
        horizontal_angle = std::stof(line, 0);
        getline(cacfg, line);
        vertical_angle = std::stof(line, 0);
        cacfg.close();
    }
    else
    {
        std::cerr << "Failed to load Camera config." << std::endl;
    }
}
