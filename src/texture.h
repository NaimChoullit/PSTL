#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include<GL/glew.h>


extern "C"
{

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/callback.h>
#include <caml/fail.h>
}



#define Texture_val(v)   (*((Texture**)       Data_custom_val(v)))

class Texture
{
    public:
        Texture(const std::string& fileName);

        void Bind(unsigned int unit);

        virtual ~Texture();


    protected:

    private:
        Texture(const Texture& other){}
        void operator=(const Texture& other){}

        GLuint m_texture;
};

#endif // TEXTURE_H
