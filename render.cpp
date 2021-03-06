/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "render.h"
#include "world.h"

using namespace std;

SDL_Surface *load_image_opengl(string filename){
    //Temporary storage for the image being loaded.
    SDL_Surface* loaded_image=NULL;

    //Load the image.
    loaded_image=IMG_Load(filename.c_str());

    //Return the loaded image.
    return loaded_image;
}

GLuint load_texture(string filename){
    //This texture will be returned at the end of this function.
    GLuint texture;

    //This will temporarily store the name of the image we are loading.
    SDL_Surface *surface=NULL;

    //Load the image.
    surface=load_image_opengl(filename.c_str());

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

void render_texture(double x,double y,image_data image_source,double opacity,short color_name){
    glPushMatrix();
    //Bind the texture object.
    if(image.current_texture!=image_source.texture){
        glBindTexture(GL_TEXTURE_2D,image_source.texture);
        image.current_texture=image_source.texture;
    }

    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    color_data color=color_name_to_doubles(color_name);
    glColor4d(color.red,color.green,color.blue,opacity);

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(1,0);  glVertex3d(image_source.w,0,0);
    glTexCoord2d(1,1);  glVertex3d(image_source.w,image_source.h,0);
    glTexCoord2d(0,1);  glVertex3d(0,image_source.h,0);

    //End quad.
    glEnd();

    //Reset.
    glPopMatrix();
}

void render_sprite(double x,double y,image_data image_source,SDL_Rect* texture_clip,double opacity,double scale_x,double scale_y,double angle,short color_name){
    glPushMatrix();
    //Bind the texture object.
    if(image.current_texture!=image_source.texture){
        glBindTexture(GL_TEXTURE_2D,image_source.texture);
        image.current_texture=image_source.texture;
    }

    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    glTranslated(texture_clip->w/2.0,texture_clip->h/2.0,0);

    glRotated(angle,0,0,-1);

    glTranslated(-texture_clip->w/2.0,-texture_clip->h/2.0,0);

    glScaled(scale_x,scale_y,1.0);

    color_data color=color_name_to_doubles(color_name);
    glColor4d(color.red,color.green,color.blue,opacity);

    //Start quad.
    glBegin(GL_QUADS);

    //Bottom left corner.
    glTexCoord2d((texture_clip->x)/image_source.w,(texture_clip->y+texture_clip->h)/image_source.h);
    glVertex3d(0,texture_clip->h,0);

    //Bottom right corner.
    glTexCoord2d((texture_clip->x+texture_clip->w)/image_source.w,(texture_clip->y+texture_clip->h)/image_source.h);
    glVertex3d(texture_clip->w,texture_clip->h,0);

    //Top right corner.
    glTexCoord2d((texture_clip->x+texture_clip->w)/image_source.w,(texture_clip->y)/image_source.h);
    glVertex3d(texture_clip->w,0,0);

    //Top left corner.
    glTexCoord2d((texture_clip->x)/image_source.w,(texture_clip->y)/image_source.h);
    glVertex3d(0,0,0);

    //End quad.
    glEnd();

    //Reset.
    glPopMatrix();
}

void render_rectangle(double x,double y,double w,double h,double opacity,short color_name){
    glPushMatrix();
    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,NULL);
    image.current_texture=0;

    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    color_data color=color_name_to_doubles(color_name);
    glColor4d(color.red,color.green,color.blue,opacity);

    glRectd(0,0,w,h);

    //Reset.
    glPopMatrix();
}

void render_rectangle_empty(double x,double y,double w,double h,double opacity,short color_name,short border_width){
    glPushMatrix();
    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,NULL);
    image.current_texture=0;

    glTranslated(x,y,0);

    color_data color=color_name_to_doubles(color_name);
    glColor4d(color.red,color.green,color.blue,opacity);

    glRectd(0,0,w,border_width);
    glRectd(0,0,border_width,h);
    glPopMatrix();
    glPushMatrix();
    glTranslated(x,y+h,0);
    glRectd(0,0,w+border_width,border_width);
    glPopMatrix();
    glPushMatrix();
    glTranslated(x+w,y,0);
    glRectd(0,0,border_width,h+border_width);

    //Reset.
    glPopMatrix();
}

void render_font(double x,double y,image_data image_source,SDL_Rect* texture_clip,short color_name,double opacity,double scale_x,double scale_y){
    glPushMatrix();
    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    glScaled(scale_x,scale_y,1.0);

    //Set the font color.
    color_data color=color_name_to_doubles(color_name);
    glColor4d(color.red,color.green,color.blue,opacity);

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:

    //Bottom left corner.
    glTexCoord2d((texture_clip->x)/image_source.w,(texture_clip->y+texture_clip->h)/image_source.h);

    //Bottom left corner.
    glVertex3d(0,texture_clip->h,0);

    //Bottom right corner.
    glTexCoord2d((texture_clip->x+texture_clip->w)/image_source.w,(texture_clip->y+texture_clip->h)/image_source.h);

    //Bottom right corner.
    glVertex3d(texture_clip->w,texture_clip->h,0);

    //Top right corner.
    glTexCoord2d((texture_clip->x+texture_clip->w)/image_source.w,(texture_clip->y)/image_source.h);

    //Top right corner.
    glVertex3d(texture_clip->w,0,0);

    //Top left corner.
    glTexCoord2d((texture_clip->x)/image_source.w,(texture_clip->y)/image_source.h);

    //Top left corner.
    glVertex3d(0,0,0);

    //End quad.
    glEnd();

    //Reset.
    glPopMatrix();
}

color_data color_name_to_doubles(short color_number){
    color_data new_color={1.0,1.0,1.0};

    switch(color_number){

    case COLOR_RAINBOW:
        new_color=color_name_to_doubles(random_range_render(COLOR_BLACK,COLOR_SPRING));
        break;

    case COLOR_BLACK:
        new_color=color_shorts_to_doubles(0,0,0);
        break;

    case COLOR_GRAY:
        new_color=color_shorts_to_doubles(128,128,128);
        break;
    case COLOR_LIGHT_GRAY:
        new_color=color_shorts_to_doubles(211,211,211);
        break;
    case COLOR_DARK_GRAY:
        new_color=color_shorts_to_doubles(105,105,105);
        break;
    case COLOR_PAYNES_GRAY:
        new_color=color_shorts_to_doubles(64,64,72);
        break;
    case COLOR_DARKER_GRAY:
        new_color=color_shorts_to_doubles(76,76,76);
        break;

    case COLOR_WHITE:
        new_color=color_shorts_to_doubles(255,255,255);
        break;

    case COLOR_BROWN:
        new_color=color_shorts_to_doubles(150,75,0);
        break;
    case COLOR_DARK_BROWN:
        new_color=color_shorts_to_doubles(101,67,33);
        break;
    case COLOR_COPPER:
        new_color=color_shorts_to_doubles(184,115,51);
        break;
    case COLOR_BRONZE:
        new_color=color_shorts_to_doubles(205,127,50);
        break;
    case COLOR_SEPIA:
        new_color=color_shorts_to_doubles(112,66,20);
        break;
    case COLOR_SANDY_BROWN:
        new_color=color_shorts_to_doubles(244,164,96);
        break;
    case COLOR_WHEAT:
        new_color=color_shorts_to_doubles(245,222,179);
        break;
    case COLOR_TAN:
        new_color=color_shorts_to_doubles(210,180,140);
        break;
    case COLOR_PALE_BROWN:
        new_color=color_shorts_to_doubles(152,118,84);
        break;

    case COLOR_YELLOW:
        new_color=color_shorts_to_doubles(255,205,0);
        break;
    case COLOR_SYSTEM:
        new_color=color_shorts_to_doubles(255,216,0);
        break;
    case COLOR_GOLD:
        new_color=color_shorts_to_doubles(212,175,55);
        break;

    case COLOR_ORANGE:
        new_color=color_shorts_to_doubles(255,159,0);
        break;
    case COLOR_PUMPKIN:
        new_color=color_shorts_to_doubles(255,117,24);
        break;

    case COLOR_RED:
        new_color=color_shorts_to_doubles(255,0,0);
        break;
    case COLOR_DARK_RED:
        new_color=color_shorts_to_doubles(206,22,32);
        break;
    case COLOR_FLAME:
        new_color=color_shorts_to_doubles(226,88,34);
        break;

    case COLOR_PINK:
        new_color=color_shorts_to_doubles(255,192,203);
        break;
    case COLOR_ROSE:
        new_color=color_shorts_to_doubles(255,0,127);
        break;
    case COLOR_SHOCKING_PINK:
        new_color=color_shorts_to_doubles(252,15,192);
        break;

    case COLOR_PURPLE:
        new_color=color_shorts_to_doubles(128,0,128);
        break;
    case COLOR_VIOLET:
        new_color=color_shorts_to_doubles(143,0,255);
        break;
    case COLOR_INDIGO:
        new_color=color_shorts_to_doubles(111,0,255);
        break;

    case COLOR_BLUE:
        new_color=color_shorts_to_doubles(0,0,255);
        break;
    case COLOR_SKY_BLUE:
        new_color=color_shorts_to_doubles(135,206,250);
        break;
    case COLOR_UN_BLUE:
        new_color=color_shorts_to_doubles(75,146,219);
        break;

    case COLOR_GREEN:
        new_color=color_shorts_to_doubles(0,255,0);
        break;
    case COLOR_JUNGLE:
        new_color=color_shorts_to_doubles(41,171,135);
        break;
    case COLOR_SPRING:
        new_color=color_shorts_to_doubles(0,255,127);
        break;

    }

    return new_color;
}

color_data color_shorts_to_doubles(short red,short green,short blue){
    color_data new_color={1.0,1.0,1.0};

    new_color.red=(double)red/255.0;
    new_color.green=(double)green/255.0;
    new_color.blue=(double)blue/255.0;

    return new_color;
}
