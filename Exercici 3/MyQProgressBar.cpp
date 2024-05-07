#include "MyQProgressBar.h"

MyQProgressBar::MyQProgressBar(QWidget *parent = 0): QProgressBar(parent) {
    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
}

void MyQProgressBar::animate() {
    x += 0.1f; // una decima cada vegada
    updateValue();
}

void MyQProgressBar::toggleAnimation() {
    if (timer.isActive()) {
        timer.stop();
        return;
    }
    timer.start(100);
}

void MyQProgressBar::start() {
    x = 0.0f;
    fase = 0.0f;
    amplitud = 0.5f;
    setMinimum(-100);
    setMaximum(100);
    updateValue();
    timer.stop();
    emit reset(50); // 50 in slider == 0 in progressbar (w == 1)
    emit initialize(int(x+1));
}

void MyQProgressBar::updateValue() {
    value = amplitud * sin(x - fase); //acoplar primera barra con la ultima
    setValue(int(value * 100));
}

void MyQProgressBar::canviaAmplitud(int ampUpdate) {
    amplitud = float(ampUpdate) / 100.0f;
    updateValue();
}

void MyQProgressBar::canviaFase(int faseUpdate) {
    fase = (float(faseUpdate) - 50.0f) / 100.0f; // 0 in slider == -50 in progressbar [-50, 50], 100 in slider in progressbar == 50
    updateValue();
}

void MyQProgressBar::initialized(int xUpdate) {
    x = xUpdate;
    fase = 0.0f;
    amplitud = 0.5f;
    setMinimum(-100);
    setMaximum(100);
    updateValue();
    timer.stop();
    emit initialize(int(x+1));
}