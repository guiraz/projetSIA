#include<vector>
#include "GLSLShader.h"
#include "texture.h"


Texture text;               // Objet gerant les textures
int shaderNum;              // Retient le shader a utiliser
GLuint vao;                 // Le handle du VAO.
GLuint vbo_positions;       // Le handle du VBO.
GLuint vbo_indices;         // Le handle du VBO.
GLSLShader shader;          // Objet gerant les shaders
GLint winHeight, winWidth;  // Resolution de la fenetre
GLfloat mouseCoord[4];

float elapsed_time, time0;  // temps en secondes

// Fonction d'initialisation, appelée une fois en début de programme.
static void init(void)
{
  //On charge les images
  text.load(QString("images/brick_grise.jpg"));
  text.load(QString("images/brick_rouge.jpg"));
  text.load(QString("images/uppa.gif"));
  text.load(QString("images/om.gif"));

  //On definie les valeurs par defaut des uniform
  winHeight = 1000;
  winWidth = 500;
  mouseCoord[0] = 0.;
  mouseCoord[1] = 0.;
  mouseCoord[2] = 0.;
  mouseCoord[3] = 0.;

  //On choisit le fragment shader
  QString fs;
  switch(shaderNum)
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
  shader.LoadFromFile(GL_FRAGMENT_SHADER, fs.toStdString());
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
  glUniform4f(shader("iMouse"), mouseCoord[0], mouseCoord[1], mouseCoord[2], mouseCoord[3]);
  glUniform1i(shader("iChannel0"), 3);
  glBindSampler(3, text.getSamplerState()); // utilise samplerState pour UT3
  glActiveTexture(GL_TEXTURE3);   // Active l'unité de texture 3
  glBindTexture(GL_TEXTURE_2D, text.getText());

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glutSwapBuffers(); // Echange des buffers FRONT et BACK.

  shader.UnUse();
}


//Procedure appele si la fenetre est redimensionne
void reshape(GLint width, GLint height)
{
  glViewport(0, 0, width, height);
  winHeight = height;
  winWidth = width;
}

//Calcul du temps ecoule
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

//Event clavier
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'g':
            text.setCurrent(0);
        break;
        case 'r':
            text.setCurrent(1);
        break;
        case '+':
            text.next();
        break;
        case 27:
            freeResources();
            exit(0);
        break;
    }
}


//Event souris
void mouse(int button, int state, int x, int y)
{
    if((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
    {
        mouseCoord[0]=0.;
        mouseCoord[1]=0.;
        mouseCoord[2]=0.;
        mouseCoord[3]=0.;
    }
}

//Event mouvement souris
void motion(int x, int y)
{
    mouseCoord[0]=(float)x;
    mouseCoord[1]=(float)(winHeight-y);
    mouseCoord[2]=1.;
    mouseCoord[3]=0.;
}

int main(int argc, char* argv[])
{
    //On verifie que le nombre d'arguments soit correct
    if(argc == 1)
        shaderNum == 1;
    else
    {
        if(argc == 2)
        {
            //On verifie que la variable envoye est un entier compris entre 0 et 5 non inclus sinon on met une valeur par defaut
            int valeur = 0;
            valeur = QString(QString::fromStdString(argv[1])).toInt();
            if((valeur < 5) && (valeur > 0))
                shaderNum = valeur;
            else
                shaderNum == 1;
        }
        else
        {
            cerr<<"Erreur : Nombre d'arguments incorrects."<<endl;
            exit(0);
        }
    }

  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glewExperimental=true;
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
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();
  freeResources();
  return 0;
}
