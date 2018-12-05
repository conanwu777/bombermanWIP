//
// Created by Hugo JEANNIN on 3/11/17.
//

#ifndef CAMERA_HPP
# define CAMERA_HPP
# define GLM_ENABLE_EXPERIMENTAL

# include <iostream>
# include <GLM/mat4x4.hpp>
# include <GLM/ext.hpp>
# include <GLM/vec3.hpp>
# include <GLM/gtx/rotate_vector.hpp>
# include <GLM/gtx/string_cast.hpp>
# include <fstream>
# include <string>
# include <iostream>
# include "../inc/Constant.hpp"
#include "Vertex.hpp"

enum Plane
{
    PLANE_BACK,
    PLANE_FRONT,
    PLANE_RIGHT,
    PLANE_LEFT,
    PLANE_TOP,
    PLANE_BOTTOM
};

class Camera
{
public:
    glm::mat4       view_matrix;
    glm::mat4       perspective_matrix;

    glm::vec3       pos;
    glm::vec3       front;
	glm::vec3		pos_and_front;
    glm::vec3       up;

	glm::vec3		move_forward;
	glm::vec3		move_left;
	glm::vec3		move_up;

    float           normal_speed;
    float           boost_speed;
    float           mega_boost_speed;
    float           movement_speed;
    float           rotation_speed;

    float           horizontal_angle;
    float           vertical_angle;

    glm::vec2       screen_center;
	float 			mouse_speed;
    glm::vec4       m_planes[6];

    Camera();
    ~Camera();

    void            init();
    void            updateMatrix();
    void 			enableBoostSpeed(bool enable);
    void            toggleMegaBoostSpeed();
    void            moveForward();
    void            moveBackward();
    void            strafeLeft();
    void            strafeRight();
    void            moveUp();
    void            moveDown();
    void            rotateLeft();
    void            rotateRight();
    void            rotateUp();
    void            rotateDown();

	void 			frustum(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

    void            print();
	void 			writeConfig();
	void 			loadConfig();
};

#endif //CAMERA_HPP
