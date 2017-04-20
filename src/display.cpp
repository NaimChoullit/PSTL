#include "display.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <iostream>

using std::cout;
using std::endl;
using std::cerr;
using std::ostringstream;





Display::Display(int width, int height,const std::string& title)
{
    
    m_width = width;
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);



    GLenum status = glewInit();

    if(status != GLEW_OK)
    {
        
         cerr << "Glew failed to initialize !" << endl;
    }

    m_isClosed = false;

    glEnable(GL_DEPTH_TEST);
       glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
   
}

 Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Display::Clear(float r, float g, float b, float a)
{
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
}

bool Display::IsClosed()
{

return m_isClosed;

}

void Display:: Display::Update()
{
  
    SDL_GL_SwapWindow(m_window);

    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            m_isClosed = true;
        }
    }

}

// functions to call from ocaml

void finalize_window(value co) {
    cerr << __func__ << ": entered!" << endl;
    Display* w = Display_val(co);
    delete w;
  }

  // operations for the OCaml GC
  static struct custom_operations window_custom_ops = {"window_custom_ops", finalize_window, NULL, NULL, NULL, NULL};



extern "C"
{
CAMLprim value createWindow(value width, value height, value title)
{
    CAMLparam3(width, height, title);
    Display* window;
    int w =  Long_val(width);
    int h = Long_val(height);
    char* t =  String_val(title);

    window = new Display(w,h,t);
    value v = caml_alloc_custom(&window_custom_ops, sizeof(Display*), 0, 1);
    Display_val(v) = window;
    CAMLreturn(v);

}
}


// for call from ocaml
extern "C"
{
    CAMLprim value clear(value windw, value r, value g, value b, value a)
    {
        CAMLparam5(windw, r, g, b, a);
        Display* disp = Display_val(windw);
        float rg =  Double_val(r);
        float bl =  Double_val(b);
        float gr =  Double_val(g);
        float ax =  Double_val(a);

        disp->Clear(rg,gr,bl,ax);
        return Val_unit;
    }

}


extern "C"
{
     CAMLprim value isClosed(value windw)
    {
        CAMLparam1(windw);
        Display* disp = Display_val(windw);
        return Val_bool(disp->IsClosed());
    }
    

}

extern "C"
{
     CAMLprim value update(value windw)
    {
        CAMLparam1(windw);
        Display* disp = Display_val(windw);
        disp->Update();
        return Val_unit;
    }
    

}