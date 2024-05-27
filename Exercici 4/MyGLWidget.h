// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

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
    virtual void modelTransformCar(float radi, float angle, glm::mat4& TG);
    virtual void iniEscena();
    virtual void iniCamera();
    virtual void initializeGL();
    virtual void iniMaterialTerra();

    float angleX;

    // iniCamera
    float ra, fov, zn, zf;
    glm::vec3 obs, vrp;

    GLuint colorCotxeLoc, posFocusSCOLoc, colFocusSCOLoc, posFocusCar1Loc, posFocusCar2Loc, colFocusCar1Loc, colFocusCar2Loc;

    float angleCar1, angleCar2, radiCar1, radiCar2;

    bool cameraEncesa;

    glm::mat4 TGcar1, TGcar2;

    bool llumEncesa;
    glm::vec3 posicioFocusSCO, colorFocusSCO;

    QTimer timer;

  public slots:
    void animate();

  private:
    int printOglError(const char file[], int line, const char func[]);
};
