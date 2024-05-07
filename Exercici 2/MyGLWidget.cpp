#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent)
{
}

int MyGLWidget::printOglError(const char file[], int line, const char func[])
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502:
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL()
{
  LL2GLWidget::initializeGL();
  connect(&timer,SIGNAL(timeout()),this, SLOT(animar()));
}

void MyGLWidget::resizeGL(int w, int h)
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
  ra = float(ample)/float(alt);
  factorAngleY = M_PI / ample;
  factorAngleX = M_PI / alt;
  if (ra > 1.0) {
    left = -radiEscena*ra;
    right = radiEscena*ra;
  }
  else {
    bottom = -radiEscena/ra;
    top = radiEscena/ra;
  }
  projectTransform();
}

bool MyGLWidget::checkPosition(glm::vec3 pos)
{
  bool valid = true;

  for(int i = 0; i < bodyPos.size() + 1; ++i){
    if (pos == tailPos or pos == headPos) {
      return false;
    }
    if(i < bodyPos.size() and pos == bodyPos[i]) {
      return false;
    }
  }
  if (pos.x < -13.9 or pos.x > 13.9 or pos.z < -13.9 or pos.z > 13.9) {
    return false;
  }

  return valid;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: {
      direction = glm::vec3(0, 0, -1);
      if (not timerOn) {
        updateSnakeGame();
      }
      break;
    }
    case Qt::Key_Down: {
      direction = glm::vec3(0, 0, 1);
      if (not timerOn) {
        updateSnakeGame();
      }
      break;
    }
    case Qt::Key_Left: {
      direction = glm::vec3(-1, 0, 0);
      if (not timerOn) {
        updateSnakeGame();
      }
      break;
    }
    case Qt::Key_Right: {
      direction = glm::vec3(1, 0, 0);
      if (not timerOn) {
        updateSnakeGame();
      }
      break;
    }
    case Qt::Key_C: {
      perspectiveOn = not perspectiveOn;
      projectTransform();
      viewTransform();
      break;
    }
    case Qt::Key_R: {
      if (timerOn) {
        timer.stop();
      }
      timerOn = false;
      perspectiveOn = true;
      iniEscena();
      iniCamera();
      projectTransform();
      break;
    }
    case Qt::Key_T: {
      if (timerOn) {
        timer.stop();
      }
      else {
        timer.start(200);
      }
      timerOn = not timerOn;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::iniCamera()
{
  dist = 2.5f * radiEscena;
  fov = 2.0f * asin(radiEscena/dist);
  znear = dist - radiEscena;
  zfar = dist + radiEscena;

  if(ra == 1) {
    left = -radiEscena;
    right = radiEscena;
    bottom = -radiEscena;
    top = radiEscena;
  }
  else if (ra > 1.0) {
    left = -radiEscena*ra;
    right = radiEscena*ra;
  }
  else {
    bottom = -radiEscena/ra;
    top = radiEscena/ra;
  }
  factorAngleX = 0.0;
  factorAngleY = 0.0;
  psi = 0.0;
  theta = float(M_PI/4.0); // 45º
  viewTransform();
}

void MyGLWidget::viewTransform()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 VM(1.0f);
  if (perspectiveOn){
    VM = glm::translate(VM, glm::vec3(0, 0, -dist));
    VM = glm::rotate(VM, theta + factorAngleY, glm::vec3(1, 0, 0));
    VM = glm::rotate(VM, -psi + factorAngleX, glm::vec3(0, 1, 0));
    VM = glm::translate(VM, -centreEscena);
  }
  else {
    VM = lookAt(glm::vec3(0, dist, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
  }
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &VM[0][0]);
}

void MyGLWidget::projectTransform()
{
  glm::mat4 PM(1.0f);
  if (perspectiveOn) {
    PM = glm::perspective(fov, ra, znear, zfar);
  }
  else {
    PM = glm::ortho(left, right, bottom, top, znear, zfar);
  }
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &PM[0][0]);
}

void MyGLWidget::iniEscena()
{
  float minX = -14.5;
  float maxX = 14.5;
  float minY = 0.0;
  float maxY = 3.0;
  float minZ = -14.5;
  float maxZ = 14.5;
  centreEscena = glm::vec3((minX + maxX)/2.0f, (minY + maxY)/2.0f, (minZ + maxZ)/2.0f);
  radiEscena = glm::distance(centreEscena, glm::vec3(maxX, maxY, maxZ));

  headAngle = 0.0;
  headPos = glm::vec3(0, 0, 0);
  bodyPos.clear();
  bodyPos.push_back(glm::vec3(-1, 0, 0));
  tailAngle = 0;
  tailPos = glm::vec3(-2, 0, 0);
  marblePos = glm::vec3(10, 0, 0);
  direction = glm::vec3(1, 0, 0);
}

void MyGLWidget::paintGL()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // SnakeHead
  glBindVertexArray (VAO_models[SNAKE_HEAD]);
  SnakeHeadTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_HEAD].faces().size()*3);

  // SnakeBody
  glBindVertexArray (VAO_models[SNAKE_BODY]);
  for (unsigned int i = 0; i < bodyPos.size(); i++)
  {
    SnakeBodyTransform(bodyPos[i]);
    glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_BODY].faces().size()*3);
  }

  // SnakeTail
  glBindVertexArray (VAO_models[SNAKE_TAIL]);
  SnakeTailTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_TAIL].faces().size()*3);

  // Marble
  glBindVertexArray (VAO_models[MARBLE]);
  MarbleTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MARBLE].faces().size()*3);

  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  for (int i = 0; i < 30; ++i) {
    PipeTransform(glm::vec3(14.5, 0, 14.5 - i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }
  for (int i = 0; i < 30; ++i) {
    PipeTransform(glm::vec3(-14.5, 0, 14.5 - i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }
  for (int i = 0; i < 30; ++i) {
    PipeTransform(glm::vec3(-14.5 + i, 0, -14.5));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }
  for (int i = 0; i < 30; ++i) {
    PipeTransform(glm::vec3(-14.5 + i, 0, 14.5));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }

  // Terra
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::MarbleTransform()
{
  glm::mat4 transf(1.0f);
  transf = glm::translate(transf, marblePos);
  transf = glm::scale(transf, glm::vec3(0.5, 0.5, 0.5));
  transf = glm::scale(transf, glm::vec3(escalaModels[MARBLE]));
  transf = glm::translate(transf, -centreBaseModels[MARBLE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transf[0][0]);
}

void MyGLWidget::SnakeTailTransform()
{
  glm::mat4 transf(1.0f);
  transf = glm::translate(transf, tailPos);
  transf = glm::scale(transf, glm::vec3(escalaModels[SNAKE_TAIL]));
  transf = glm::rotate(transf, glm::radians(tailAngle), glm::vec3(0, 1, 0));
  transf = glm::translate(transf, -centreBaseModels[SNAKE_TAIL]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transf[0][0]);
}

void MyGLWidget::SnakeHeadTransform()
{
  glm::mat4 transf(1.0f);
  transf = glm::translate(transf, headPos);
  transf = glm::scale(transf, glm::vec3(escalaModels[SNAKE_HEAD]));
  transf = glm::rotate(transf, glm::radians(headAngle) + float(-M_PI/2.0f), glm::vec3(0, 1, 0));
  transf = glm::translate(transf, -centreBaseModels[SNAKE_HEAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transf[0][0]);
}

void MyGLWidget::PipeTransform(glm::vec3 pos)
{
  glm::mat4 transf(1.0f);
  transf = glm::translate(transf, pos);
  transf = glm::scale(transf, glm::vec3(1.0, 3.0, 1.0));
  transf = glm::scale(transf, glm::vec3(escalaModels[PIPE]));
  transf = glm::translate(transf, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transf[0][0]);
}

void MyGLWidget::TerraTransform()
{
  glm::mat4 transf(1.0f);
  transf = glm::scale(transf, glm::vec3(3.0, 1.0, 3.0));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &transf[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  if (perspectiveOn) {
    makeCurrent();

    int diffx = e->x() - xClick;
    int diffy = e->y() - yClick;

    xClick = e->x();
    yClick = e->y();

    if (DoingInteractive == ROTATE) {
      theta += diffy * factorAngleX;
      psi += diffx * factorAngleY;
    }
    viewTransform();
    update();
  }
}

void MyGLWidget::animar()
{
  makeCurrent();
  updateSnakeGame();
  update();
}
