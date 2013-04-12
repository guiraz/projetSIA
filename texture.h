#ifndef TEXTURE_H
#define TEXTURE_H
#include <QVectror>
#include <QImage>
#include <QGLWidget>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture
{
public:
    Texture();

private :
    QVector<GLuint> _texName;
    GLuint _samplerState;
};

#endif // TEXTURE_H
