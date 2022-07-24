#include "mesh.h"
#include "vec4.h"
#include <cstdint>
#include <fstream>
#include <string>
#include <strstream>

bool mesh::selectMeshFromObj(const char * fileName)
{
    std::ifstream file;
    file.open(fileName);

    //if the file cant be found
    if(!file.is_open())
        return false;

    //temporary local verticies
    std::vector<vec4> verts;

    for(std::string currentLine; std::getline(file, currentLine); )
    {       
        std::strstream ss;
        ss << currentLine;

        if(currentLine[0] == 'v')
        {
            vec4 v;
            char firstChar;
            ss >> firstChar >> v.x >> v.y >> v.z;
            verts.push_back(v);
        }
        else if(currentLine[0] == 'f')
        {
            uint64_t triangle[3];
            char firstChar;
            ss >> firstChar 
               >> triangle[0] 
               >> triangle[1] 
               >> triangle[2];

            tris.push_back({verts[triangle[0]-1], 
                                  verts[triangle[1]-1], 
                                  verts[triangle[2]-1]});
        }
    }

    file.close();
    return true;
}

//turn current *this mesh into a unit cube shape
void mesh::unitCube()
{
    tris =
    {
        //this face points in the -Z direction
        {vec4{0.0f,0.0f,0.0f},  vec4{0.0f,1.0f,0.0f},  vec4{1.0f,1.0f,0.0f}},
        {vec4{0.0f,0.0f,0.0f},  vec4{1.0f,1.0f,0.0f},  vec4{1.0f,0.0f,0.0f}},
         
        //this face points in the +X direction   
        {vec4{1.0f,0.0f,0.0f},  vec4{1.0f,1.0f,0.0f},  vec4{1.0f,1.0f,1.0f}},
        {vec4{1.0f,0.0f,0.0f},  vec4{1.0f,1.0f,1.0f},  vec4{1.0f,0.0f,1.0f}},
         
        //this face points in the +Z direction   
        {vec4{1.0f,0.0f,1.0f},  vec4{1.0f,1.0f,1.0f},  vec4{0.0f,1.0f,1.0f}},
        {vec4{1.0f,0.0f,1.0f},  vec4{0.0f,1.0f,1.0f},  vec4{0.0f,0.0f,1.0f}},
            
        //this face points in the -X direction
        {vec4{0.0f,0.0f,1.0f},  vec4{0.0f,1.0f,1.0f},  vec4{0.0f,1.0f,0.0f}},
        {vec4{0.0f,0.0f,1.0f},  vec4{0.0f,1.0f,0.0f},  vec4{0.0f,0.0f,0.0f}},

        //this face points in the +Y direction
        {vec4{0.0f,1.0f,0.0f},  vec4{0.0f,1.0f,1.0f},  vec4{1.0f,1.0f,1.0f}},
        {vec4{0.0f,1.0f,0.0f},  vec4{1.0f,1.0f,1.0f},  vec4{1.0f,1.0f,0.0f}},

        //this face points in the -Y direction
        {vec4{1.0f,0.0f,1.0f},  vec4{0.0f,0.0f,1.0f},  vec4{0.0f,0.0f,0.0f}},
        {vec4{1.0f,0.0f,1.0f},  vec4{0.0f,0.0f,0.0f},  vec4{1.0f,0.0f,0.0f}}
    };
}