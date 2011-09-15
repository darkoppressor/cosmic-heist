#include "background_layer.h"
#include "world.h"
#include "render.h"
#include "pixels.h"

using namespace std;

Background_Layer::Background_Layer(double get_width,double get_height,double get_parallax_speed_x,double get_parallax_speed_y){
    width=get_width;
    height=get_height;

    parallax_speed_x=get_parallax_speed_x;
    parallax_speed_y=get_parallax_speed_y;

    position_x_a=0;
    position_x_b=0;
    position_y_a=0;
    position_y_b=0;
}

GLuint Background_Layer::generate_texture_background(double width,double height,short layer_number){
    //This texture will be returned at the end of this function.
    GLuint texture;

    //This will temporarily store the name of the image we are loading.
    SDL_Surface *surface=NULL;

    Uint32 rmask,gmask,bmask,amask;

    if(SDL_BYTEORDER==SDL_BIG_ENDIAN){
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    }
    else{
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    }

    surface=SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,rmask,gmask,bmask,amask);

    //If the surface must be locked.
    if(SDL_MUSTLOCK(surface)){
        //Lock the surface.
        SDL_LockSurface(surface);
    }

    Uint32 pixel;

    //Set the background color.
    pixel=SDL_MapRGBA(surface->format,0,0,0,0);

    //Apply the background color.
    for(int x=0;x<surface->w;x++){
        for(int y=0;y<surface->h;y++){
            surface_put_pixel(surface,x,y,pixel);
        }
    }

    //Create the stars:

    //Keep track of the number of star creation attempts.
    int star_attempts=0;

    //The number of star attempts to make.
    int max_star_attempts=0;

    max_star_attempts=50-layer_number*10;

    //Set the star color.
    pixel=SDL_MapRGBA(surface->format,255,255,255,255);

    while(star_attempts<max_star_attempts){
        int x=random_range(1,surface->w-2);
        int y=random_range(1,surface->h-2);

        surface_put_pixel(surface,x,y,pixel);

        //Another star attempt has been made.
        star_attempts++;
    }

    //If the surface was locked.
    if(SDL_MUSTLOCK(surface)){
        //Unlock surface.
        SDL_UnlockSurface(surface);
    }

    //Have OpenGL generate a texture object handle for us.
    glGenTextures(1,&texture);

    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,texture);

    //Set the texture's properties:
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //Edit the texture object's image data using the information our image gives us:
    glTexImage2D(GL_TEXTURE_2D,0,surface->format->BytesPerPixel,surface->w,surface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);

    //We have copied the image data into the new texture, so delete the image data.
    SDL_FreeSurface(surface);

    //Return the new texture we have created.
    return texture;
}

void Background_Layer::update(double camera_delta_x,double camera_delta_y){
    //Update the x position.

    position_x_a-=camera_delta_x/parallax_speed_x;

    if(position_x_a+width<0){
        position_x_a=0;
    }
    else if(position_x_a>0){
        position_x_a=-width;
    }

    position_x_b=position_x_a+width;

    //Update the y position.

    position_y_a-=camera_delta_y/parallax_speed_y;

    if(position_y_a+height<0){
        position_y_a=0;
    }
    else if(position_y_a>0){
        position_y_a=-height;
    }

    position_y_b=position_y_a+height;
}

void Background_Layer::render(short layer_number){
    render_texture((int)position_x_a,(int)position_y_a,image.background_layer_images[layer_number]);
    render_texture((int)position_x_b,(int)position_y_a,image.background_layer_images[layer_number]);
    render_texture((int)position_x_a,(int)position_y_b,image.background_layer_images[layer_number]);
    render_texture((int)position_x_b,(int)position_y_b,image.background_layer_images[layer_number]);
}
