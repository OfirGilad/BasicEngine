#include "../Engine3D/Bezier1D.h"

Bezier1D::Bezier1D(int segNum, int res, int mode, int viewport) 
{
    segmentsNum = segNum;
    resT = res;
    segments_mode = mode;
}

IndexedModel Bezier1D::GetLine() const
{
    IndexedModel model;
    int num_of_dots_on_line = 10;
    int num_of_indices = segmentsNum * num_of_dots_on_line;

    for (int i = 0; i <= num_of_indices; i++) {
        model.indices.push_back(i);
    }

    glm::vec4 p_0 = GetControlPoint(0, 0);
    model.positions.push_back(glm::vec3(p_0.x, p_0.y, p_0.z));

    for (int i = 0; i < segmentsNum; i++) {
        for (int j = 0; j < num_of_dots_on_line; j++) {
            float t = (1.f / (float)num_of_dots_on_line) * (j + 1);
            glm::vec4 p_t = GetPointOnCurve(j, t);
            model.positions.push_back(glm::vec3(p_t.x, p_t.y, p_t.z));
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
glm::vec4 Bezier1D::GetPointOnCurve(int segment, int t) const
{
    glm::vec4 b_0 = segments[segment][0]; //p0
    glm::vec4 b_1 = segments[segment][1]; //p1
    glm::vec4 b_2 = segments[segment][2]; //p2
    glm::vec4 b_3 = segments[segment][3]; //p3

    glm::vec4 b_t = glm::vec4((1 - t) ^ 3) * b_0 
        + glm::vec4(3 * (1 - t) ^ 2 * t) * b_1 
        + glm::vec4(3 * (1 - t) * t ^ 2) * b_2 
        + glm::vec4(t ^ 3) * b_3;
    
    return b_t;
}

// b'(t) = -3(1-t)^2*b_0 + (3-12t+9t^2)*b_1 + (6t-9t^2)*b_2 + 3t^2*b_3
glm::vec3 Bezier1D::GetVelosity(int segment, int t)
{
    glm::vec4 b_0 = segments[segment][0]; //p0
    glm::vec4 b_1 = segments[segment][1]; //p1
    glm::vec4 b_2 = segments[segment][2]; //p2
    glm::vec4 b_3 = segments[segment][3]; //p3

    glm::vec4 db_t = glm::vec4(-3 * (1 - t) ^ 2) * b_0
        + glm::vec4(3 - 12 * t + 9 * t ^ 2) * b_1
        + glm::vec4(6 * t - 9 * t ^ 2) * b_2
        + glm::vec4(3 * t ^ 2) * b_3;

    return glm::vec3(db_t.x, db_t.y, db_t.z);
}

void Bezier1D::SplitSegment(int segment, int t)
{

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

void Bezier1D::ChangeSegment(int segIndx,glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments[segIndx-1][3];
    segments[segIndx] = glm::mat4(p0, p1, p2, p3);
}

float Bezier1D::MoveControlPoint(int segment, int indx, float dx,float dy,bool preserveC1)
{
    if (preserveC1) {

    }
    return 0; //not suppose to reach here
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