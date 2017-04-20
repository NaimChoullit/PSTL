#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <string>
#include <iostream>

extern "C"
{

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/callback.h>
#include <caml/fail.h>
}



#define Display_val(v)   (*((Display**)       Data_custom_val(v)))

class Display
{
   public:
        Display(int width, int height,const std::string& title);
        void Clear(float r, float g, float b, float a);
        void Update();
        bool IsClosed();
        int m_width;
        virtual ~Display();
    protected:
    private:
         Display(const Display& other);
         void operator=(const Display& other);

        SDL_Window* m_window;
        SDL_GLContext m_glContext;

        bool m_isClosed;
     
        
};



#endif // DISPLAY_H
