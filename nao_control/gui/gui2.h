#ifndef TELECAMERA_GUI_H
#define TELECAMERA_GUI_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QLCDNumber>
#include <QDial>
#include <QPushButton>
#include <iostream>
#include "../shared.h"

#include <QtOpenGL/QGLWidget>
#include <QWidget>
#include "glWidget.h"
#include "LCDNumber.h"

//Values for a range between 10 and -10. Eq = (sliValue - mid_range)*step so if slider value is 101 the equation will convert it
//into 0.1 due to the step.   
const float step = 0.1;
const int max_range = 200;
const float mid_range = max_range/2;
const float PI = 3.14159265;


namespace telecamera {
class Gui;
}

class Gui : public QWidget
{
    Q_OBJECT

public:
    explicit Gui(Shared* sm);
    ~Gui();
    void updateThreadGui();
	int crearFichero(std::string);


private:

    //Control
    Shared* sm;
    GLWidget window;

     QWidget *gridLayoutWidget;
    QGridLayout *gridGazebo;
    QPushButton *abrirGazebo;
    QPushButton *cerrarGazebo;
    QGroupBox *modelos;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridModelos;
    QPushButton *cargarModelo;
    QListWidget *listaModelos;
    QPushButton *eliminarModelo;
    QGroupBox *simulacion;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridSimulacion;
    QPushButton *paso;
    QPushButton *stop;
    QPushButton *play;
    QPushButton *reiniciar;
    QPushButton *pausa;
    LCDNumber *cronometro;
    QGridLayout *estadisticas;
    QLabel *desviacion;
    QLabel *distancia;
    QLabel *tiempo;
    QLabel *valDistancia;
    QLabel *valTiempo;
    QLabel *valDesviacion;

    
private slots:
	void on_abrir_buttonpressed();
	void on_cerrar_buttonpressed();
	void on_cargar_buttonpressed();
	void on_eliminar_buttonpressed();
	void on_paso_buttonpressed();
	void on_pausa_buttonpressed();
	void on_play_buttonpressed();
	void on_stop_buttonpressed();
	void on_reiniciar_buttonpressed();
    void updateAll();
	void sleep(unsigned long);


signals:
    void update();

};
#endif // MAINWINDOW_H

