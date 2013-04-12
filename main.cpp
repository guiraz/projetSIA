#include <iostream>
#include<vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GLSLShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QImage>
#include <QGLWidget>

#define NUMFRAG 2

GLuint textNum;
GLuint vao;             // Le handle du VAO.
GLuint vbo_positions;   // Le handle du VBO.
GLuint vbo_indices;     // Le handle du VBO.
GLSLShader shader;
GLint winHeight, winWidth;//resolution de la fenetre

float elapsed_time, time0;  // temps en secondes

QImage* loadImage(std::string filename)
{
  QImage img(filename.c_str());

  if (img.isNull()) {
    cerr << "Unable to load " << filename << endl; return NULL; }

  cout << "Loading " << filename << ", "
       << img.width() << "x" << img.height() << " pixels" << endl;

  // Convertit l'image dans un format utilisable par OpenGL
  return new QImage(QGLWidget::convertToGLFormat(img));
}

// Fonction d'initialisation, appelée une fois en début de programme.
static void init(void)
{
  // Charge la texture
  QImage* img = loadImage("images/brick_grise.jpg");
  // Generate one texture objects
  glGenTextures(2, texName);
  if (img != NULL) {
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, img->width(),img->height(),0, GL_RGBA,GL_UNSIGNED_BYTE, img->bits());
  }
  img=NULL;
  img = loadImage("images/brick_rouge.jpg");
  if (img != NULL) {
    glBindTexture(GL_TEXTURE_2D, texName[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, img->width(),img->height(),0, GL_RGBA,GL_UNSIGNED_BYTE, img->bits());
  }
  delete img;

  glGenSamplers(3, &samplerState);
  glSamplerParameteri(samplerState, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameteri(samplerState, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glSamplerParameteri(samplerState, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(samplerState, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glSamplerParameterf(samplerState, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

  textNum = 0;

  winHeight = 1000;
  winWidth = 500;

  string fs;
  switch(NUMFRAG)
  {
    case 1:
      fs="shaders/shadertoy_1.frag.glsl";
    break;
    case 2:
      fs="shaders/shadertoy_2.frag.glsl";
    break;
    case 3:
      fs="shaders/shadertoy_3.frag.glsl";
    break;
    case 4:
      fs="shaders/shadertoy_4.frag.glsl";
    break;
    default:
      fs="shaders/shadertoy_1.frag.glsl";
    break;
  }

  // Création des shaders depuis des fichiers sur le disque
  shader.LoadFromFile(GL_VERTEX_SHADER,   "shaders/shadertoy.vert.glsl");
  shader.LoadFromFile(GL_FRAGMENT_SHADER, fs);
  shader.CreateAndLinkProgram();
  shader.AddUniform("iGlobalTime");
  shader.AddUniform("iResolution");
  shader.AddUniform("iMouse");
  shader.AddUniform("iChannel0");
  shader.AddAttribute("position");



  // Créations des buffers
  GLfloat positions[] = {-2,2, -2,-2, 2,2, 2,-2};
  GLuint    indices[] = {0,1,2,3};

  glGenBuffers(1, &vbo_positions);               // VBO pour contenir les positions.
  glBindBuffer(GL_ARRAY_BUFFER, vbo_positions);  // Il devient le VBO courant.
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

  glGenBuffers(1, &vbo_indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);   // VAO pour contenir les états de VBO/attributs.
  glBindVertexArray(vao);       // Il devient le VAO courant.

  glBindBuffer(GL_ARRAY_BUFFER, vbo_positions);
  glEnableVertexAttribArray(shader["position"]);
  glVertexAttribPointer(shader["position"], 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);

  glBindVertexArray(0);  // On détache tous les buffers pour l'instant
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glClearColor(0.5, 0.3, 0.3, 1.0);  // Couleur d'effacement de l'écran
  time0 = glutGet(GLUT_ELAPSED_TIME)/1000.; // initialisation du temps d'origine
}

// Fonction d'affichage, appelée régulièrement, pour réafficher le contenu.
void display(void)
{
  shader.Use();

  glClear(GL_COLOR_BUFFER_BIT); // On efface la fenêtre.
  glUniform1f(shader("iGlobalTime"), elapsed_time);
  glUniform3f(shader("iResolution"), winWidth, winHeight, 1.0);

  glUniform1i(shader("iChannel0"), 3);
  glBindSampler(3, samplerState); // utilise samplerState pour UT3
  glActiveTexture(GL_TEXTURE3);   // Active l'unité de texture 3
  glBindTexture(GL_TEXTURE_2D, texName[textNum]);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glutSwapBuffers(); // Echange des buffers FRONT et BACK.

  shader.UnUse();
}

void reshape(GLint width, GLint height)
{
  glViewport(0, 0, width, height);
  winHeight = height;
  winWidth = width;
}

void animate()
{
  elapsed_time = glutGet(GLUT_ELAPSED_TIME)/1000. - time0;
  glutPostRedisplay();
}

// Fonction de libération des ressources, appélée en fin de programme.
static void freeResources(void)
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);   // On indique qu'aucun VBO n'est le courant.
  glDeleteBuffers(1, &vbo_positions); // Et on supprime les nôtres.
  glBindVertexArray(0);               // On indique qu'aucun VAO n'est le courant.
  glDeleteVertexArrays(1, &vao);      // Et on supprime le nôtre.
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'g':
            textNum = 0;
        break;
        case 'r':
            textNum = 1;
        break;
        case 27:
            freeResources();
            exit(0);
        break;
    }
}

int main(int argc, char* argv[])
{
  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutInitContextProfile(GLUT_CORE_PROFILE); glewExperimental=true; // Astuce en attendant que GLEW
                                                                    // supporte le profile "core"
  //glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);             // ou ligne à utiliser sinon
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(1000, 500);
  glutCreateWindow("Un triangle de couleur changeante en rotation");
  glewInit(); // Initialisation de la bibliothèque GLEW.
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(animate);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  freeResources();
  return 0;
}
