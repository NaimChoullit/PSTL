#include "texture.h"
#include "stb_image.h"
#include <iostream>
#include <cassert>
#include <GL/glew.h>


using std::cout;
using std::endl;
using std::cerr;
using std::ostringstream;

Texture::Texture(const std::string& fileName)
{
    int width, height, numComponents;
    unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);


    if(imageData == NULL)
    std::cerr << "Texture loadinf failed for texture " << fileName << std::endl;

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    stbi_image_free(imageData);

}

Texture::~Texture()
{
    glDeleteTextures(1,&m_texture);
}


void Texture::Bind(unsigned int unit)
{

    assert(unit >=0 && unit <= 31);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texture);

}


// partie ocaml 

void finalize_texture(value co) {
    cerr << __func__ << ": entered!" << endl;
    Texture* t = Texture_val(co);
    delete t;
  }

  // operations for the OCaml GC
  static struct custom_operations texture_custom_ops = {"texture_custom_ops", finalize_texture, NULL, NULL, NULL, NULL};


extern "C"
{
CAMLprim value createTexture(value file)
{
    CAMLparam1(file);
    char* fileName = String_val(file);
 

    Texture* texture;
    
    texture = new Texture(fileName);
    value v = caml_alloc_custom(&texture_custom_ops, sizeof(Texture*), 0, 1);
    Texture_val(v) = texture;
    CAMLreturn(v);
}
}

extern "C"
{
CAMLprim value bindTexture(value textr, value i)
{
    CAMLparam2(textr, i);
    Texture* texture = Texture_val(textr);
    texture->Bind(Int_val(i));
    return Val_unit;
}
}