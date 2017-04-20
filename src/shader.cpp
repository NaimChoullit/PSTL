#include "shader.h"
#include <iostream>
#include <fstream>


using std::cout;
using std::endl;
using std::cerr;
using std::ostringstream;

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& fileName);
static GLuint createShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName)
{
    m_program = glCreateProgram();

    m_shaders[0] = createShader(LoadShader(fileName + ".vs") , GL_VERTEX_SHADER);
    m_shaders[1] = createShader(LoadShader(fileName + ".fs") , GL_FRAGMENT_SHADER);


    for(unsigned int i=0; i < NUM_SHADER; i++)
    {
        glAttachShader(m_program, m_shaders[i]);
    }

     glBindAttribLocation(m_program, 0, "position");
     glBindAttribLocation(m_program, 1, "texCoord");
     glBindAttribLocation(m_program, 2, "normal");

    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Error : Program linking failed :  ");

    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error : Program is invalid ");

    m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program,"transform");

}

Shader::~Shader()
{

    for(unsigned int i=0; i < NUM_SHADER; i++)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
      }
    glDeleteProgram(m_program);
}


void Shader::Bind()
{
    glUseProgram(m_program);
}

void Shader::Update(const Transform& transform, const Camera& camera)
{
   
    glm::mat4 model = camera.GetViewProjection() * transform.GetModel();
    glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

static GLuint createShader(const std::string& text, GLenum shaderType)
{
     GLuint shader = glCreateShader(shaderType);

     if(shader == 0)
        std::cerr << "Error : Shader creation failed! " << std::endl;

     const GLchar* shaderSourceString[1];
     GLint shaderSourceStringLengths[1];

     shaderSourceString[0] = text.c_str();
     shaderSourceStringLengths[0] = text.length();

     glShaderSource(shader, 1, shaderSourceString, shaderSourceStringLengths);
     glCompileShader(shader);

     CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error : shader Compilation failed ");


     return shader;
}


static std::string LoadShader(const std::string& fileName)
{

    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;


    if(file.is_open())
    {
        while(file.good())
        {
            getline(file,line);
            output.append(line + "\n");
        }
    }
    else
    {

    std::cerr << "Unable to load shader " << fileName << std::endl;
    }

    return output;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = {0};

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);


    if(success == GL_FALSE)
    {

        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << " : '" << error << std::endl;

    }



}

// partie ocaml 

void finalize_shader(value co) {
    cerr << __func__ << ": entered!" << endl;
    Shader* w = Shader_val(co);
    delete w;
  }

  // operations for the OCaml GC
  static struct custom_operations shader_custom_ops = {"shader_custom_ops", finalize_shader, NULL, NULL, NULL, NULL};


void finalize_transform(value co) {
    cerr << __func__ << ": entered!" << endl;
    Transform* w = Transform_val(co);
    delete w;
  }

  // operations for the OCaml GC
  static struct custom_operations transform_custom_ops = {"transform_custom_ops", finalize_transform, NULL, NULL, NULL, NULL};

void finalize_camera(value co) {
    cerr << __func__ << ": entered!" << endl;
   Camera* c = Camera_val(co);
    delete c;
  }

  // operations for the OCaml GC
  static struct custom_operations camera_custom_ops = {"camera_custom_ops", finalize_camera, NULL, NULL, NULL, NULL};


extern "C"
{
    CAMLprim value createCamera(value pos, value fov, value aspect, value zNear, value zFar)
    {
        CAMLparam5(pos,fov, aspect, zNear, zFar);

        Camera* camera;

        camera = new Camera(
            glm::vec3((float) Double_field (pos, 0 ),(float) Double_field (pos , 1), (float) Double_field (pos, 2 )),
            Double_val(fov),
            Double_val(aspect),
            Double_val(zNear),
            Double_val(zFar));

        value v = caml_alloc_custom(&camera_custom_ops, sizeof(Camera*), 0, 1);
        Camera_val(v) = camera;
        CAMLreturn(v);
    }   

}


extern "C"
{
    CAMLprim value createTransform()
    {
        CAMLparam0();
        Transform* transform;
        transform = new Transform(glm::vec3(), glm::vec3(), glm::vec3(1.0f, 1.0f, 1.0f));
        value v = caml_alloc_custom(&transform_custom_ops, sizeof(Transform*), 0, 1);
        Transform_val(v) = transform;
        CAMLreturn(v);
    }   

}

extern "C"
{
CAMLprim value createShad(value fileName)
{
    CAMLparam1(fileName);
   Shader* shader;
    
    char* t =  String_val(fileName);

    shader = new Shader(t);
    value v = caml_alloc_custom(&shader_custom_ops, sizeof(Shader*), 0, 1);
    Shader_val(v) = shader;
    CAMLreturn(v);
}
}

extern "C"
{
CAMLprim value bindShad(value shad)
{
    CAMLparam1(shad);
    Shader* shader = Shader_val(shad);
    shader->Bind();
    return Val_unit;
}
}

extern "C"
{
CAMLprim value updateShader(value shad, value trans, value cam)
{
    CAMLparam3(shad, trans, cam);
    Shader* shader = Shader_val(shad);
    const Transform* transform = Transform_val(trans);
    const Camera* camera = Camera_val(cam);
    
    
    shader->Update(*transform, *camera);

    return Val_unit;
}
}


extern "C"
{
CAMLprim value setPosX(value trans, value x)
{
    CAMLparam2(trans, x); 
    Transform* transform = Transform_val(trans);   
    transform->GetPos()->x = (float) Double_val(x);
    return Val_unit;  
}
}


extern "C"
{
CAMLprim value setPosY(value trans, value x)
{
    CAMLparam2(trans, x);
    Transform* transform = Transform_val(trans); 
    transform->GetPos()->y = Double_val(x);
    return Val_unit;
}
}


extern "C"
{
CAMLprim value setPosZ(value trans, value x)
{
 CAMLparam2(trans, x);
    Transform* transform = Transform_val(trans); 
    transform->GetPos()->z = Double_val(x);
    return Val_unit; 
}
}


extern "C"
{
CAMLprim value setRotX(value trans, value x)
{
 CAMLparam2(trans, x);
    Transform* transform = Transform_val(trans); 
    transform->GetRot()->x = Double_val(x);
    return Val_unit;
    
}
}


extern "C"
{
CAMLprim value setRotY(value trans, value x)
{
 CAMLparam2(trans, x);
    Transform* transform = Transform_val(trans); 
    transform->GetRot()->y = Double_val(x);
    return Val_unit;
    
}
}


extern "C"
{
CAMLprim value setRotZ(value trans, value x)
{
 CAMLparam2(trans, x);
    Transform* transform = Transform_val(trans); 
    transform->GetRot()->z = Double_val(x);
    return Val_unit;
    
}
}


extern "C"
{
CAMLprim value setScaleX(value trans, value x)
{
 CAMLparam2(trans, x);
    Transform* transform = Transform_val(trans); 
    transform->GetScale()->x = Double_val(x);
    return Val_unit;
    
}
}

extern "C"
{
CAMLprim value setScaleY(value trans, value x)
{
 CAMLparam2(trans, x);
    Transform* transform = Transform_val(trans); 
    transform->GetScale()->y = Double_val(x);
    return Val_unit;
    
}
}

extern "C"
{
CAMLprim value setScaleZ(value trans, value x)
{
 CAMLparam2(trans, x);
    Transform* transform = Transform_val(trans); 
    transform->GetScale()->z = Double_val(x);
    return Val_unit;
    
}
}
