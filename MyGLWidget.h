#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    
    void creaBuffersCos();

    void creaBuffersCano();

    void creaBuffersRoda();
    
    void carregaShaders();
    
    void modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala);

    void modelTransformQuadratCano(glm::vec3 escala);

    void modelTransformQuadratRoda(int i, int j);

    void pintaCos();

    void pintaCano();

    void pintaRodas();

    void pintaTanc(); // Cridat per PaintGL
    
    // program
    QOpenGLShaderProgram *program;
    // attribute locations
    GLuint vertexLoc;
    // uniform locations
    GLuint TGLoc;
    GLuint ColorLoc;

    // VAOs
    GLuint VAOQuadrat;
    GLuint VAOCos;
    GLuint VAOCano;
    GLuint VAORoda;

    // viewport
    GLint ample, alt;    
    
    // colors
    glm::vec3 verd = glm::vec3(0.0,0.6,0.0);
    glm::vec3 negre = glm::vec3(0.0);    
    glm::vec3 gris = glm::vec3(0.5,0.5,0.5);

    float moureRoda = 0.0;
    float movx = 0.0;
    float angleRoda = 0.0;
    glm::vec3 pivotCano = glm::vec3(0.125, 0.25, 0.0);
    float angleCano = 0.0;

    QVector<float> posRoda = {-0.375, -0.125, 0.125, 0.375};

};
