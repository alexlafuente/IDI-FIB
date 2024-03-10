#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersCos();
  creaBuffersCano();
  creaBuffersRoda();
}


void MyGLWidget::modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posicio);  
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformQuadratCano(glm::vec3 posicio, glm::vec3 escala)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posicio);
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformQuadratRoda(int i, int j)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(posRoda[i]+movx,0.0,0.0));
  TG = glm::translate(TG, glm::vec3(0.0,-0.125,0.0));
  TG = glm::rotate(TG, float(M_PI*j/6), glm::vec3(0.0,0.0,1.0));
  TG = glm::rotate(TG, angleRoda, glm::vec3(0.0,0.0,0.1));
  TG = glm::scale(TG, glm::vec3(1.0f));
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::pintaTanc()
{
  glBindVertexArray(VAOCos);
  modelTransformQuadrat(glm::vec3(0.0), glm::vec3(1.0));;
  glm::vec4 bodyColor = glm::vec4(verd, 1);
  glUniform4fv(ColorLoc, 1, &bodyColor[0]);
  glDrawArrays(GL_TRIANGLES, 0, 12);
  glBindVertexArray(0);

  glBindVertexArray(VAOCano);
  modelTransformQuadratCano(glm::vec3(0.0), glm::vec3(1.0));
  glm::vec4 cannonColor = glm::vec4(gris, 1);
  glUniform4fv(ColorLoc, 1, &cannonColor[0]);
  glDrawArrays(GL_TRIANGLES, 0, 9);
  glBindVertexArray(0);

  glBindVertexArray(VAORoda);
  for (int i = 0; i < 4; ++i) {
    bool paintBlack = true;
    for (int j = 0; j < 12; ++j) {
      modelTransformQuadratRoda(i, j);
      if (paintBlack) {
        glUniform4f(ColorLoc, negre.r, negre.g, negre.b, 1.0f);
      }
      else {
        glUniform4f(ColorLoc, gris.r, gris.g, gris.b, 1.0f);
      }
      glDrawArrays(GL_TRIANGLES, 0, 9);
      paintBlack = not paintBlack;
    }
  }
  // glDrawArrays(GL_TRIANGLES, 0, 9);
  glBindVertexArray(0);
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer
  // Pintem tanc
  pintaTanc();
  // // Pintem un quadrat
  // glBindVertexArray(VAOQuadrat);
  // modelTransformQuadrat(glm::vec3(0.0), glm::vec3(1.0));
  // glDrawArrays(GL_TRIANGLES, 0, 6);
  // glBindVertexArray(0);

  // Desactivem el VAO
  // glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Left: 
    	break;
    case Qt::Key_Right: 
    	break;
    case Qt::Key_A: 
      movx -= 0.01;
      angleRoda -= M_PI/180;
    	break;
    case Qt::Key_D:
      movx += 0.01;
      angleRoda += M_PI/180;
    	break;		
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffersCos ()
{
  glm::vec3 VerticesCos[12];  // vèrtexs amb X, Y i Z
  
  // minX = -1.0
  // maxX = 0.0
  // minY = -1.0
  // maxY = 0.0
  
  VerticesCos[0] = glm::vec3(-0.5, -0.125, 0);
  VerticesCos[1] = glm::vec3( -0.5, 0.125, 0);
  VerticesCos[2] = glm::vec3( 0.5, -0.125, 0);
  VerticesCos[3] = glm::vec3( 0.5, 0.125, 0);
  VerticesCos[4] = glm::vec3( -0.5, 0.125, 0);
  VerticesCos[5] = glm::vec3( 0.5, -0.125, 0);

  VerticesCos[6] = glm::vec3(-0.125, 0.375, 0);
  VerticesCos[7] = glm::vec3( -0.125, 0.125, 0);
  VerticesCos[8] = glm::vec3( 0.375, 0.375, 0);
  VerticesCos[9] = glm::vec3( 0.375, 0.375, 0);
  VerticesCos[10] = glm::vec3( -0.125, 0.125, 0);
  VerticesCos[11] = glm::vec3( 0.375, -0.125, 0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAOCos);
  glBindVertexArray(VAOCos);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBOCos;
  glGenBuffers(1, &VBOCos);
  glBindBuffer(GL_ARRAY_BUFFER, VBOCos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesCos), VerticesCos, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);

  // // Creació del buffer amb les posicions dels vèrtexs
  // GLuint VBO;
  // glGenBuffers(1, &VBO);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // glEnableVertexAttribArray(vertexLoc);

  // // Desactivem el VAO
  // glBindVertexArray(0);
}

void MyGLWidget::creaBuffersCano()
{
  glm::vec3 VerticesCano[9];  // vèrtexs amb X, Y i Z

  // minX = -1.0
  // maxX = 0.0
  // minY = -1.0
  // maxY = 0.0

  VerticesCano[0] = glm::vec3(0.125, 0.25-(0.125/2), 0);
  VerticesCano[1] = glm::vec3( 0.125, 0.25+(0.125/2), 0);
  VerticesCano[2] = glm::vec3( 0.125+(0.75/2), 0.25-(0.125/2), 0);

  VerticesCano[3] = glm::vec3( 0.125+0.75, 0.25-(0.125/2), 0);
  VerticesCano[4] = glm::vec3( 0.125+0.75, 0.25+(0.125/2), 0 );
  VerticesCano[5] = glm::vec3( 0.125+(0.75/2), 0.25-(0.125/2), 0);

  VerticesCano[6] = glm::vec3( 0.125, 0.25+(0.125/2), 0);
  VerticesCano[7] = glm::vec3( 0.125+0.75, 0.25+(0.125/2), 0 );
  VerticesCano[8] = glm::vec3( 0.125+(0.75/2), 0.25-(0.125/2), 0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAOCano);
  glBindVertexArray(VAOCano);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBOCano;
  glGenBuffers(1, &VBOCano);
  glBindBuffer(GL_ARRAY_BUFFER, VBOCano);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesCano), VerticesCano, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // glm::vec4 bodyColor = glm::vec4(gris, 1);
  // glUniform4fv(ColorLoc, 1, &bodyColor[0]);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersRoda()
{
  glm::vec3 VerticesRectangle[9];

  VerticesRectangle[0] = glm::vec3(0-0.1-(0.1/2), -(0.05/2), 0);
  VerticesRectangle[1] = glm::vec3(0-0.1-(0.1/2), 0.05/2, 0);
  VerticesRectangle[2] = glm::vec3(0-0.1, -(0.05/2), 0);

  VerticesRectangle[3] = glm::vec3(0-0.1+(0.1/2), -(0.05/2), 0);
  VerticesRectangle[4] = glm::vec3(0-0.1+(0.1/2), 0.05/2, 0);
  VerticesRectangle[5] = glm::vec3(0-0.1, -(0.05/2), 0);

  VerticesRectangle[6] = glm::vec3(0-0.1-(0.1/2), 0.05/2, 0);
  VerticesRectangle[7] = glm::vec3(0-0.1+(0.1/2), 0.05/2, 0);
  VerticesRectangle[8] = glm::vec3(0-0.1, -(0.05/2), 0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAORoda);
  glBindVertexArray(VAORoda);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBORoda;
  glGenBuffers(1, &VBORoda);
  glBindBuffer(GL_ARRAY_BUFFER, VBORoda);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesRectangle), VerticesRectangle, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // glm::vec4 bodyColor = glm::vec4(gris, 1);
  // glUniform4fv(ColorLoc, 1, &bodyColor[0]);

  // Desactivem el VAO
  glBindVertexArray(0);
}


void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  
  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG"); 

  ColorLoc = glGetUniformLocation(program->programId(), "TancColor");
}
