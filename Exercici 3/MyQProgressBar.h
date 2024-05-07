#include <QProgressBar>
#include <QTimer>
#include <math.h>

class MyQProgressBar: public QProgressBar
{
Q_OBJECT
public:
    MyQProgressBar(QWidget *parent);
protected:
    float x, fase, amplitud, value;
    QTimer timer;
    bool animated;
    void updateValue();
public slots:
    void animate();
    void toggleAnimation();
    void start();
    void canviaAmplitud(int);
    void canviaFase(int);
    void initialized(int);
signals:
    void initialize(int);
    void reset(int);
};