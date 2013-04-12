#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QImage>
#include <QGLWidget>

#include <iostream>
using namespace std;

class Texture
{
public:
    /**
      Constructeur de la classe Texture
      **/
    Texture();
    /**
      Procedure chargeant les images en memoire
      **/
    void load(QString url);
    /**
      Procedure qui change la texture courante a la suivante
      **/
    void next();
    /**
      Procedure qui change la texture courante par celle indexe
      **/
    void setCurrent(int index);
    /**
      Proceure initialisant le Sampler State
      **/
    void setSamplerState();
    /**
      Fonction qui recupere la texture courante
      **/
    GLuint getText();
    /**
      Fonction qui recupere le Sampler State
      **/
    GLuint getSamplerState();

private :
    QStringList _url;       //Liste des url des images
    GLuint * _texName;      //Liste des textures
    GLuint _samplerState;   //Le Sampler State
    GLuint _current;        //Index de la texture courante

    /**
      Fonction qui retourne l'image grace a son url
      **/
    QImage * loadImage(QString & filename);
};

#endif // TEXTURE_H
