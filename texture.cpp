#include "texture.h"

Texture::Texture()
{
    _current = 0;
}

void Texture::load(QString url)
{
    _url.append(url);
    _texName = new GLuint[_url.size()];
    glGenTextures(_url.size(), _texName);
    for(int i=0; i<_url.size(); i++)
    {
        QImage* img = loadImage(_url[i]);
        if (img != NULL) {
          glBindTexture(GL_TEXTURE_2D, _texName[i]);
          glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
          glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, img->width(),img->height(),0, GL_RGBA,GL_UNSIGNED_BYTE, img->bits());
        }
        delete img;
    }
}

void Texture::next()
{
    _current = (_current + 1) % _url.size();
}

void Texture::setCurrent(int index)
{
    _current = index;
}

void Texture::setSamplerState()
{
    glGenSamplers(3, &_samplerState);
    glSamplerParameteri(_samplerState, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(_samplerState, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(_samplerState, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(_samplerState, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameterf(_samplerState, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
}

GLuint Texture::getText()
{
    return _texName[_current];
}

GLuint Texture::getSamplerState()
{
    return _samplerState;
}

QImage* Texture::loadImage(QString & filename)
{
    QImage img(filename);
    if (img.isNull())
    {
        cerr << "Unable to load " << filename.toStdString() << endl;
        return NULL;
    }
    cout << "Loading " << filename.toStdString() << ", "<< img.width() << "x" << img.height() << " pixels" << endl;
    return new QImage(QGLWidget::convertToGLFormat(img));
}
