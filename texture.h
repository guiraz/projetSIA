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
    void load(QString & url);
    void next();
    void setCurrent(int index);
    void setSamplerState();
    GLuint getText();

private :
    QStringList _url;
    QVector<GLuint> _texName;
    GLuint _samplerState;
    GLuint _current;

    QImage loadImage(QString & filename);
};

#endif // TEXTURE_H
