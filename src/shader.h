#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include "transform.h"
#include "camera.h"


extern "C"
{

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/callback.h>
#include <caml/fail.h>
}



#define Shader_val(v)   (*((Shader**)       Data_custom_val(v)))
#define Transform_val(v)   (*((Transform**)       Data_custom_val(v)))
#define Camera_val(v)   (*((Camera**)       Data_custom_val(v)))


class Shader
{
    public:
        Shader(const std::string& fileName);

        void Bind();
        void Update(const Transform& transform, const Camera& camera);
        virtual ~Shader();


    protected:

    private:
        static const unsigned int NUM_SHADER = 2;

        Shader(const Shader& other){}
        void operator=(const Shader& other){}

        enum
        {
            TRANSFORM_U,
            NUM_UNIFORMS

        };

        GLuint m_program;
        GLuint m_shaders[NUM_SHADER];
        GLuint m_uniforms[NUM_UNIFORMS];
};

#endif // SHADER_H
