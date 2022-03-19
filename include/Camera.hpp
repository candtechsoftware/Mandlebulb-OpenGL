#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#define PI 3.14159265358979323846f
#define TWO_PI 6.28318530718f

struct Camera {
    const float m_default_radius = 3.0f;
    const float m_default_theta = 3.0f;
    const float m_default_phi = 3.0f;

    float m_radius = m_default_radius;
    float m_theta = m_default_radius;
    float m_phi = m_default_radius;

    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_z = 0.0f;

    float m_coord_turn_step = 0.1f;
    float m_coord_zoom_step = 0.008f;

    bool m_free_control_is_active = false;
    bool m_is_constant_zoom = false;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;
    glm::mat4 m_view_matrix;
    glm::mat4 m_projection_matrix;

    Camera(unsigned int w, unsigned int h, float near, float far);
    ~Camera() = default;

    void print_coordinates();

    void update_centered_view_matrix();
    void rotate_view_matrix_horizontally(float a);
    void rotate_view_matrix_vertically(float a);
    void translate_view_matrix(float v);

    glm::vec3 get_view_matrix_backward();
    glm::vec3 get_view_matrix_forward();

    void handle_key_press_w();
    void handle_key_press_a();
    void handle_key_press_s();
    void handle_key_press_d();
    void handle_key_press_z();
    void handle_key_press_x();

    void reset();

    void spherical_to_cartesian();
};
