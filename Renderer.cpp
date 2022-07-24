#include <Windows.h> //drawTriangle() needs to use wingdi to draw lines
#include <cmath>     //trig functions and sqrt()
#include <algorithm>//std::sort()
#include "vec4.h"
#include "Renderer.h"
#define PI 3.14159f

Renderer Renderer::renderer;

Renderer::Renderer(){}

Renderer::~Renderer()
{
    //if release hasnt been called by
    //the time main() exits then call it
    if(isInit)
        release();
}

static float clamp(const float val, const float low, const float high)
{
    if(val < low)
        return low;
    else if(val > high)
        return high;
    else 
        return val;
}

void Renderer::scaleIntoScreenSpace(triangle& tri, const int sw, const int sh)
{
    for(int i = 0; i < 3; ++i)
    {      
        tri.verticies[i].x += 1.0f; 
        tri.verticies[i].y += 1.0f;
                    
        tri.verticies[i].x *= .5f * (float)sw;
        tri.verticies[i].y *= .5f * (float)sh;
    }
}

void Renderer::drawWireFrame(const triangle& t, HDC& dc)
{
    SetDCPenColor(dc, RGB(0,255,0));
    
    //draw line from second point to first point
    MoveToEx(dc,(int)t.verticies[1].x, (int)t.verticies[1].y, nullptr);
    LineTo(dc,  (int)t.verticies[0].x, (int)t.verticies[0].y);
    
    //draw line from third point to second point
    MoveToEx(dc,(int)t.verticies[2].x, (int)t.verticies[2].y, nullptr);
    LineTo(dc,  (int)t.verticies[1].x, (int)t.verticies[1].y);
    
    //draw line from first point to third point
    MoveToEx(dc,(int)t.verticies[0].x, (int)t.verticies[0].y, nullptr);
    LineTo(dc,  (int)t.verticies[2].x, (int)t.verticies[2].y);
}

void Renderer::drawTriangle(const triangle& tri, HDC& dc)
{
     POINT cords[3] = {{(LONG)tri.verticies[0].x,(LONG)tri.verticies[0].y }, 
                      { (LONG)tri.verticies[1].x,(LONG)tri.verticies[1].y },
                      { (LONG)tri.verticies[2].x,(LONG)tri.verticies[2].y}}; 

    SetDCBrushColor(dc, RGB(tri.greyScale,tri.greyScale,tri.greyScale));
    SetDCPenColor(dc, RGB(tri.greyScale,tri.greyScale,tri.greyScale));
    Polygon(dc, cords, 3);
}

//update function called once per loop
void Renderer::update(const int screenWidth, const int screenHeight, HDC& dc)
{
    static float angle;   
    angle += deltaTime * 0.0003f;

    //rotation * translation
    //so translation happens then rotation
    mat4 transform1, transform2;
    transform1 = mat4::matMul(mat4::xRotationMatrix(angle),
                              mat4::translationMatrix(0.0f, 0.0f, 5.0f));   
    //transform2 = mat4::matMul(transform1, mat4::translationMatrix(0.0f, 0.0f, 6.0f));

    std::vector<triangle> drawTris;
    drawTris.reserve(m.tris.size());

    for(const triangle& t : m.tris)
    {   
        triangle transformed;          
        
        for(int i=0; i<3; ++i)
            transformed.verticies[i] = mat4::mat4MulByVector4(t.verticies[i], transform1);

        //get the vector from the first vertex to the second
        vec4 vec1 = transformed.verticies[1] - transformed.verticies[0];

        //get the vector from the first vertex to the third
        vec4 vec2 = transformed.verticies[2] - transformed.verticies[0];
                      
        //get the cross product between two above vectors
        vec4 norm = vec4::cross(vec1, vec2);

        //normalize the triangles normal vector
        norm = vec4::normalize(norm);
        
        //get the vector from the camera to the triangle
        vec4 cameraToTri = transformed.verticies[0] - camera;

        //normalize the camToTri vector
        cameraToTri = vec4::normalize(cameraToTri);

        //get the dot product of the camera to triangle vector with the triangles normal vector
        float dotProduct = vec4::dot(cameraToTri, norm);

        //back face culling
        //if the face of the tri is pointing < 90 degrees away from the camera then draw it
        if(dotProduct < 0.0f)
        {
            triangle projected;
            vec4 lightDir = {0.0f, 0.0f, -1.0f};
            float lightDp = vec4::dot(norm, lightDir);
            lightDp *= 255;           
            projected.greyScale = clamp(lightDp, 10.0f, 250.0f);

            //project triangle
            for(int i=0; i<3; ++i)
                projected.verticies[i] = mat4::mat4MulByVector4(transformed.verticies[i], 
                                                                projectionMatrix);          
            for(int i=0; i<3; ++i)
                projected.verticies[i] /= projected.verticies[i].w;
            
            //scale from normalized/clip space to screen size
            scaleIntoScreenSpace(projected, screenWidth, screenHeight);
            
            //drawTris already has space reserved
            //so as to avoid constant reallocs
            drawTris.push_back(projected);
        }
    }

    //sort the triangles back to front (painters algorithm)
    //no triangle clipping. so it can look not great in some cases
    std::sort(drawTris.begin(),drawTris.end(),
    [](triangle& t0, triangle& t1) -> bool
    {
        float avgZ0 = (t0.verticies[0].z +
                       t0.verticies[1].z +
                       t0.verticies[2].z)/3.0f;

        float avgZ1 = (t1.verticies[0].z +
                       t1.verticies[1].z +
                       t1.verticies[2].z)/3.0f;

        return (avgZ0 > avgZ1);
    });

    //send to triangles to winGDI to rasterize
    for(const triangle& t : drawTris)
    {
        drawTriangle(t, dc);
        //drawWireFrame(triProjected, dc);
    }
}

void Renderer::init(const mat4& proj)
{
    //if init has already been called return
    if(isInit) 
        return;

    isInit = true;
}

void Renderer::release() 
{
    isInit = false;
}

Renderer& Renderer::get()
{   
    return renderer;
}

void Renderer::setProjection(const mat4& mat)
{
    projectionMatrix = mat;
}

void Renderer::selectMesh(const mesh& mesh)
{
    m = mesh;
}
