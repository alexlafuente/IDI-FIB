#include "LL2GLWidget.h"

#include <vector>

#include <QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  protected:

    virtual void keyPressEvent (QKeyEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *e);

    // funció per comprovar si una nova posició de la serp és vàlida
    virtual bool checkPosition(glm::vec3 pos);
    virtual void paintGL();

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);

    virtual void iniCamera();
    virtual void iniEscena();

    virtual void viewTransform();
    virtual void projectTransform();

    virtual void TerraTransform();
    virtual void PipeTransform(glm::vec3 pos);
    virtual void SnakeTailTransform();
    virtual void SnakeHeadTransform();
    virtual void MarbleTransform();

  public slots:
    void animar();

  private:
  
    int printOglError(const char file[], int line, const char func[]);
    float dist, left, right, bottom, top;

    float psi, theta;

    bool perspectiveOn = true;
    bool timerOn = false;

    QTimer timer;
};
