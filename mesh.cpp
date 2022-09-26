#include "mesh.h"
#include "vec4.h"
#include "Renderer.h"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

bool mesh::tryToLoadMesh(char const* fileName)
{
    auto& renderer = Renderer::get();

    auto const now = std::chrono::system_clock::now();
    std::time_t const time = std::chrono::system_clock::to_time_t(now);
    char dataAndTime[256]{};
    ctime_s(dataAndTime, 256, &time);

    //try to load the mesh
    mesh m;
    try//try to load the object file specified
    {
        m.selectMeshFromObj(fileName);
    }
    catch(std::exception& e)
    {
        std::ofstream ofs("errorLog.txt", std::ios_base::app);

        std::cerr << e.what()  
                  << "execption thrown trying to load "
                  << fileName << '\n';

        ofs << e.what()
            << "execption thrown trying to load "
            << fileName
            << " at "
            << dataAndTime << '\n';

        return false;
    }
    catch(char const* throwMsg)
    {
        std::cerr << throwMsg << "(error logged in errorLog.txt)\n";

        std::ofstream ofs("errorLog.txt", std::ios_base::app);
        ofs << throwMsg << " at " << dataAndTime << '\n';

        return false;
    }
    catch(...)
    {
        char const* msg = "an exeption was thrown\n"
            " (that isnt a const char* throwMessage and also didnt derive from std::exception)\n"
            " while trying to load ";

        std::cerr << msg << fileName << '\n';

        std::ofstream ofs("errorLog.txt", std::ios_base::app);
        ofs << msg << fileName << " at " << dataAndTime << '\n';

        return false;
    }

    renderer.selectMesh(m);
    return true;
}

//removes slashes from a face line in an obj file
//example: f 17/18/1 2/4/12 1/19/15 -> f 17 2 1
static void removeExtraData(std::string& faceLine, std::size_t slashPos)
{
    int i = 0;
    do
    {
        std::size_t nextSpace = faceLine.find(' ', slashPos);//find the space after the slash
        faceLine.erase(slashPos, nextSpace - slashPos);//erase from slash up to the space
        slashPos = faceLine.find('/');//find the next slash

        ++i;
        if(i > 3)
        {
            throw "the faces in the .obj file that the parser tried to load"
                  " contained more than three verticies. This renderer only supports triangles";
        }
    }
    while(slashPos != std::string::npos);
}

void mesh::selectMeshFromObj(const char * fileName)
{
    std::ifstream file;
    file.open(fileName);

    //if the file cant be found
    if(!file.is_open()) 
        throw "couldnt find the obj file";

    //temporary local verticies
    std::vector<vec4> verts;
    verts.reserve(4000u);//might as well reserve some space here

    for(std::string currentLine; std::getline(file, currentLine); )
    {       
        if(currentLine[0] == 'v')
        {
            std::stringstream ss(currentLine);
            char firstChar;
            ss >> firstChar;//discard the v character
            vec4 v;
            ss >> v.x >> v.y >> v.z;
            verts.push_back(v);
        }
        else if(currentLine[0] == 'f')
        {   
            //these three vertexes are the first numbers in: f 17/18/1 2/4/12 1/5/9 as and example.
            //the first numbers in the n/n/n sequences are indecies (in the above ex it would be 17 2 1)
            //into the vector of vertecies called verts created above. the three vertecies in the verts 
            //vector that will be located by these indecies will go on to be the three verticies that define
            //a triangle in the mesh that we are creating with this function
            uint64_t vertexIndecies[3]{};

            std::size_t slashPos = currentLine.find('/');
            if(slashPos != std::string::npos)
            {
                removeExtraData(currentLine, slashPos);           
            }

            std::stringstream ss(currentLine);

            char fCharacter{};
            ss >> fCharacter;//discard the f character.
            ss >> vertexIndecies[0];//put the indecies from the obj file 
            ss >> vertexIndecies[1];//into the array of three indecies that will 
            ss >> vertexIndecies[2];//then point to vectices in verts
            
            tris.push_back
            (
                Triangle
                {
                    verts[vertexIndecies[0]-1],
                    verts[vertexIndecies[1]-1],
                    verts[vertexIndecies[2]-1]
                }
            );
        }
    }

    file.close();
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