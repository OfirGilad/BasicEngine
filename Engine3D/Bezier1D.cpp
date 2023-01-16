#include "../Engine3D/Bezier1D.h"
#include <iostream>
#include <stdexcept>

Bezier1D::Bezier1D()
{
}

Bezier1D::Bezier1D(int segNum, int res, int mode, int viewport)
{
    segmentsNum = segNum;
    resT = res;
    this->SetMode(mode);
}

IndexedModel Bezier1D::GetLine() const
{
    IndexedModel model;
    int num_of_dots_on_line = (resT - 1) / segmentsNum;

    // Verifying that resT value is valid
    if ((resT - 1) % segmentsNum != 0) {
        std::cout << "Error: 'resT' isn't divisible by 'segmentsNum' with a remainder of '1'!" << std::endl;
        throw std::invalid_argument("");
    }

    for (int i = 0; i < resT; i++) {
        model.indices.push_back(i);
    }

    glm::vec4 p_0 = GetControlPoint(0, 0);
    model.positions.push_back(glm::vec3(p_0.x, p_0.y, p_0.z));

    for (int i = 0; i < segmentsNum; i++) {
        for (int j = 0; j < num_of_dots_on_line; j++) {
            float t = (1.f / (float)num_of_dots_on_line) * (j + 1);
            glm::vec4 p_t = GetPointOnCurve(i, t);

            // Normal to the Curve (2D)
            glm::vec3 dt = GetVelosity(i, t);
            float sqrt_t = sqrt(pow(dt.x, 2) + pow(dt.y, 2));
            glm::vec3 normal = glm::vec3(-dt.y / sqrt_t, dt.x / sqrt_t, 0);
            
            model.positions.push_back(glm::vec3(p_t.x, p_t.y, p_t.z));
            model.colors.push_back(glm::vec3(1.f, 1.f, 1.f));
            model.normals.push_back(normal);
        }
    }

    return model;
}

glm::vec4 Bezier1D::GetControlPoint(int segment, int indx) const
{
    if (segment < segmentsNum)
        return segments[segment][indx];
    return segments[segmentsNum - 1][3];
}

// b(t) = (1-t)^3*b_0 + 3t(1-t)^2*b_1 + 3t^2(1-t)*b_2 + t^3*b_3
glm::vec4 Bezier1D::GetPointOnCurve(int segment, float t) const
{
    glm::vec4 b_0 = segments[segment][0]; //p0
    glm::vec4 b_1 = segments[segment][1]; //p1
    glm::vec4 b_2 = segments[segment][2]; //p2
    glm::vec4 b_3 = segments[segment][3]; //p3

    float a_0 = pow(1 - t, 3);
    float a_1 = 3 * pow(1 - t, 2) * t;
    float a_2 = 3 * (1 - t) * pow(t, 2);
    float a_3 = pow(t, 3);

    glm::vec4 b_t = a_0 * b_0 + a_1 * b_1 + a_2 * b_2 + a_3 * b_3;
    
    return b_t;
}

// b'(t) = -3(1-t)^2*b_0 + (3-12t+9t^2)*b_1 + (6t-9t^2)*b_2 + 3t^2*b_3
glm::vec3 Bezier1D::GetVelosity(int segment, float t) const
{
    glm::vec4 b_0 = segments[segment][0]; //p0
    glm::vec4 b_1 = segments[segment][1]; //p1
    glm::vec4 b_2 = segments[segment][2]; //p2
    glm::vec4 b_3 = segments[segment][3]; //p3

    float a_0 = -3 * pow(1 - t, 2);
    float a_1 = 3 - 12 * t + 9 * pow(t, 2);
    float a_2 = 6 * t - 9 * pow(t, 2);
    float a_3 = 3 * pow(t, 2);

    glm::vec4 db_t = a_0 * b_0 + a_1 * b_1 + a_2 * b_2 + a_3 * b_3;

    return glm::vec3(db_t.x, db_t.y, db_t.z);
}

void Bezier1D::AddFirstSegment(glm::vec4 p0, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3) {
    segments.clear();
    segments.push_back(glm::mat4(p0, p1, p2, p3));
}

void Bezier1D::AddSegment(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments.back()[3];
    segments.push_back(glm::mat4(p0, p1, p2, p3));
}

void Bezier1D::ChangeFirstSegment(glm::vec4 p0, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    segments[0] = glm::mat4(p0, p1, p2, p3);
}

void Bezier1D::ChangeSegment(int segIndx,glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments[segIndx-1][3];
    segments[segIndx] = glm::mat4(p0, p1, p2, p3);
}

float Bezier1D::MoveControlPoint(int segment, int indx, float dx,float dy,bool preserveC1)
{
    return 0; //not suppose to reach here
}

void Bezier1D::SplitSegment(int segment, float t)
{
}

void Bezier1D::CurveUpdate(int pointIndx, float dx, float dy, bool preserveC1)
{
}

void Bezier1D::ResetCurve(int segNum)
{
}

Bezier1D::~Bezier1D(void)
{
}
