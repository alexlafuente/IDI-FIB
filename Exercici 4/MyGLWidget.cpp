// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent) : LL4GLWidget(parent) {

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

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL()
{
  LL4GLWidget::initializeGL();
  colorCotxeLoc = glGetUniformLocation(program->programId(), "colorCotxe");
}

void MyGLWidget::iniEscena ()
{
  glm::vec3 pmin = glm::vec3(-12.5, 0, -12.5);
  glm::vec3 pmax = glm::vec3(12.5, 0, 12.5);
  centreEsc = (pmin + pmax) / 2.0f;
  radiEsc = glm::distance(pmin, centreEsc);
  posRoads[0] = glm::vec3(6, 0.01, 0);
  posRoads[1] = glm::vec3(0, 0.01, -4.5);
  posRoads[2] = glm::vec3(-4.5, 0.01, 0);
  posRoads[3] = glm::vec3(0, 0.01, 4.5);
}

void MyGLWidget::iniCamera ()
{
  angleY = 0.0;
  angleX = 0.0f;
  ra = float(width())/height();
  fov = 2*asin(radiEsc/2*radiEsc);
  zn = radiEsc;
  zf = 3*radiEsc;

  vrp = glm::vec3(centreEsc);
  obs = vrp + glm::vec3(0, 2*radiEsc, 0);
  up = glm::vec3(0, 0, -1);

  projectTransform();
  viewTransform ();
}

void MyGLWidget::viewTransform()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, +angleX, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform()
{
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(fov, ra, zn, zf);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::paintGL ()
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // Car
  glm::vec3 color = glm::vec3(0, 1, 0);
  glBindVertexArray (VAO_models[CAR]);

  modelTransformCar (6.5f, 0.0f);
  glUniform3fv(colorCotxeLoc, 1, &color[0]);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

  modelTransformCar (9.0f, 0.0f);
  color = glm::vec3(1, 0, 0);
  glUniform3fv(colorCotxeLoc, 1, &color[0]);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  modelTransformPipe ();
  glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

  // Road
  glBindVertexArray (VAO_models[ROAD]);
  // for (int i = 0; i < 4; ++i) {
  //   float angleRoad = (2*M_PI*i)/4.0f;
  //   modelTransformRoad (glm::vec3(10-4.2, 0.01, 0), angleRoad);
  //   glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  // }
  for (int i = 0; i < 4; ++i) {
    modelTransformRoad (glm::vec3(1, 0.01, -1), (2*M_PI*i)/4.0f);
      glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  }

  glBindVertexArray(0);
}

void MyGLWidget::modelTransformRoad(glm::vec3 pos, float angle)
{
  glm::mat4 TG(1.0f);

  TG = glm::rotate(TG, angle, glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3(5, 5, 5));
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(escalaModels[ROAD]));
  TG = glm::translate(TG, -centreBaseModels[ROAD]);

  // // LL4GLWidget::modelTransformRoad(pos, angle);
  // glm::mat4 TG(1.0f);
  // TG = glm::translate(TG, pos);
  // TG = glm:: translate(TG, glm::vec3(-(10-4.2), 0, 0)); // tornar a l'origen
  //
  // TG = glm::rotate(TG, angle, glm::vec3(0, 1, 0)); // Rotació angle y de l'angle de la carretera (rotació respecte a radi 10)
  // TG = glm:: translate(TG, glm::vec3(10-4.2, 0, 0)); // Posició (10,0,0) per tenir radi 10 respecte l'origen
  //
  // TG = glm::rotate(TG, glm::radians(-45.0f), glm::vec3(0, 1, 0));
  // TG = glm::scale(TG, glm::vec3(4.2,4.2,4.2));
  // TG = glm::scale(TG, glm::vec3(escalaModels[ROAD]));
  // TG = glm::translate(TG, -centreBaseModels[ROAD]);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCar(float radi, float angle)
{
  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(radi,0,0)); // Per rotar amb el radi corresponent

  TG = glm::scale(TG, glm::vec3(escalaModels[CAR]));
  TG = glm::translate(TG, -centreBaseModels[CAR]);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPipe()
{
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(5, 5, 5));
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.0f);
  // TG = glm::translate(TG, glm::vec3(5, 0, 5)); // Desplaçar a l'origen
  TG = glm::scale(TG, glm::vec3(float(25/10), float(25/10), float(25/10)));
  TG = glm::translate(TG, glm::vec3(-5, 0, -5)); // Desplaçar a l'origen
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX -= (e->y() - yClick) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      // ...
    break;
	}
  case Qt::Key_L: {
      // ...
    break;
	}
  case Qt::Key_S: {
      // ...
    break;
	}
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}
