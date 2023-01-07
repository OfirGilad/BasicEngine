#include "../Engine3D/Bezier1D.h"

Bezier1D::Bezier1D(int segNum, int res, int mode, int viewport) 
{
    segmentsNum = segNum;
    resT = res;
}

IndexedModel Bezier1D::GetLine() const
{
    IndexedModel model;

    return model;
}

glm::vec4 Bezier1D::GetControlPoint(int segment, int indx) const
{
    if (segment < segmentsNum)
        return segments[segment][indx];
    return segments[segmentsNum - 1][3];
}

// b(t) = (1-t)^3*b_0 + 3t(1-t)^2*b_1 + 3t^2(1-t)*b_2 + t^3*b_3
glm::vec4 Bezier1D::GetPointOnCurve(int segment, int t)
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