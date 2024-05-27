// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();
    
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformTerra();
    virtual void paintGL();
    virtual void modelTransformRoad(glm::vec3 pos, float angle);
    virtual void modelTransformPipe();
    virtual void projectTransform();
    virtual void viewTransform();
    virtual void modelTransformCar(float radi, float angle);
    virtual void iniEscena();
    virtual void iniCamera();
    virtual void initializeGL();

    QVector<glm::vec3> posRoads = QVector<glm::vec3>(4);

    float angleX;

    // iniCamera
    float ra, fov, zn, zf;
    glm::vec3 obs, vrp, up;

    GLuint colorCotxeLoc;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
