#include "Camera.hpp"
#include "glm/ext/vector_float3.hpp"
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

Camera::Camera(unsigned int w, unsigned int h, float near, float far)
{
    m_eye = glm::vec3(0.0f, 0.0f, 1.0f);
    m_center = glm::vec3(0.0f, 0.0f, 0.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_view_matrix = glm::lookAt(m_eye, m_center, m_up);
    m_projection_matrix = glm::perspective(90.0f, (GLfloat)w / (GLfloat)h, near, far);
}

void Camera::print_coordinates()
{

    printf("\nr: %.3f, theta: %.3f, phi: %.3f and x: %.3f, y: %.3f, z: %.3f", m_radius, m_theta, m_phi, m_x, m_y, m_z);
}

void Camera::update_centered_view_matrix() { m_view_matrix = glm::lookAt(m_eye, m_center, m_up); }

glm::vec3 Camera::get_view_matrix_backward()
{
    return glm::vec3(m_view_matrix[1][3], m_view_matrix[2][3], m_view_matrix[3][3]);
}

glm::vec3 Camera::get_view_matrix_forward() { return -get_view_matrix_backward(); }

void Camera::rotate_view_matrix_horizontally(float a)
{
    m_view_matrix = glm::rotate(a, glm::vec3(0.0, 1.0, 0.0)) * m_view_matrix;
}

void Camera::rotate_view_matrix_vertically(float a)
{
    m_view_matrix = glm::rotate(a, glm::vec3(1.0, 0.0, 0.0)) * m_view_matrix;
}

void Camera::translate_view_matrix(float v)
{
    glm::vec3 tmp_vec = glm::normalize(get_view_matrix_forward());
    tmp_vec = glm::vec3(float(tmp_vec.x * v), float(tmp_vec.y * v), float(tmp_vec.z * v));
    m_view_matrix = glm::translate(tmp_vec) * m_view_matrix;
}

void Camera::handle_key_press_w()
{
    m_theta -= m_coord_turn_step;
    m_theta = std::max(0.0f, m_theta);
    m_theta = std::min(PI, m_theta);

    if (m_free_control_is_active) {
        rotate_view_matrix_vertically(-m_coord_turn_step);
    }
}

void Camera::handle_key_press_a()
{
    m_phi -= m_coord_turn_step * 2.0f;
    m_phi = std::max(0.0f, m_phi);
    m_phi = std::min(TWO_PI, m_phi);
    if (m_free_control_is_active) {
        rotate_view_matrix_horizontally(-m_coord_turn_step);
    }
}

void Camera::handle_key_press_s()
{
    m_theta += m_coord_turn_step;
    m_theta = std::max(0.0f, m_theta);
    m_theta = std::min(PI, m_theta);

    if (m_free_control_is_active) {
        rotate_view_matrix_vertically(m_coord_turn_step);
    }
}
void Camera::handle_key_press_d()
{

    m_phi -= m_coord_turn_step * 2.0f;
    m_phi = std::max(0.0f, m_phi);
    m_phi = std::min(TWO_PI, m_phi);
    if (m_free_control_is_active) {
        rotate_view_matrix_horizontally(m_coord_turn_step);
    }
}

void Camera::handle_key_press_z()
{
    m_radius -= m_coord_zoom_step;
    if (m_free_control_is_active) {
        translate_view_matrix(-m_coord_zoom_step);
    }
}

void Camera::handle_key_press_x()
{

    m_radius += m_coord_zoom_step;
    if (m_free_control_is_active) {
        translate_view_matrix(m_coord_zoom_step);
    }
}

void Camera::reset()
{
    m_radius = m_default_radius;
    m_theta = m_default_theta;
    m_phi = m_default_phi;
}

void Camera::spherical_to_cartesian()
{
    m_x = m_radius * sinf(m_theta) * cosf(m_phi);
    m_y = m_radius * sinf(m_theta) * sinf(m_phi);
    m_z = m_radius * cosf(m_theta);
}
