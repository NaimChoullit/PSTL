#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "obj_loader.h"

class Vertex
{
    public:
        Vertex(const glm::vec3& pos, const glm::vec2& texCoord,const glm::vec3& normal = glm::vec3(0,0,0))
            {
            this->pos = pos;
            this->texCoord = texCoord;
            this->normal = normal;
            }

            inline glm::vec3* GetPos() {return &pos;}
            inline glm::vec2* GetTextCoord() {return &texCoord;}
            inline glm::vec3* GetNormal() {return &normal;}

    protected:

    private:

        glm::vec3 pos;
        glm::vec2 texCoord;
        glm::vec3 normal;

};


extern "C"
{

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/callback.h>
#include <caml/fail.h>
}



#define Mesh_val(v)   (*((Mesh**)       Data_custom_val(v)))


class Mesh
{
    public:
        Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
        Mesh(const std::string& fileName);
        void Draw();

        virtual ~Mesh();

    protected:
    private:
        Mesh(const Mesh& other);
        void operator=(const Mesh& other);
            
        void InitMesh(const IndexedModel& model);
        enum
        {
            POSITION_VB,
            TEXCOORD_VB,
            NORMAL_VB,

            INDEX_VB,

            NUM_BUFFERS
        };

        GLuint m_vertexArrayObject;
        GLuint m_vertexArrayBuffers[NUM_BUFFERS];

        unsigned int m_drawCount;
};

#endif // MESH_H
