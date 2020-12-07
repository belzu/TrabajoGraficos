#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "display.h"
#include "io.h"
#include "definitions.h"
extern iluminacion;
//gcc main.c  display.c io.c load_obj_joseba.c -lGL -lGLU -lglut -lm
//cp objects/abioia.obj ./a para crear direcciones directas

/** GLOBAL VARIABLES **/

GLdouble _window_ratio;                     /*Control of window's proportions */
GLdouble _ortho_x_min,_ortho_x_max;         /*Variables for the control of the orthographic projection*/
GLdouble _ortho_y_min ,_ortho_y_max;        /*Variables for the control of the orthographic projection*/
GLdouble _ortho_z_min,_ortho_z_max;         /*Variables for the control of the orthographic projection*/

object3d * _first_object= 0;                /*List of objects*/
object3d * _selected_object = 0;            /*Object currently selected*/
//-------------------------------------------------------------
//Inicializamos la camara global
camara_l *camaraG=0;
camara_l *camaraG2=0;
camara_l *camaraG3=0;
camara_l *camaraG4=0;
//-------------------------------------------------------------




/** GENERAL INITIALIZATION **/
void initialization (){

    /*Initialization of all the variables with the default values*/
    _ortho_x_min = KG_ORTHO_X_MIN_INIT;
    _ortho_x_max = KG_ORTHO_X_MAX_INIT;
    _ortho_y_min = KG_ORTHO_Y_MIN_INIT;
    _ortho_y_max = KG_ORTHO_Y_MAX_INIT;
    _ortho_z_min = KG_ORTHO_Z_MIN_INIT;
    _ortho_z_max = KG_ORTHO_Z_MAX_INIT;

    _window_ratio = (GLdouble) KG_WINDOW_WIDTH / (GLdouble) KG_WINDOW_HEIGHT;

    //Vamos a inicializar los valores de las camaras globales A MANO:
    camaraG = (camara_l *) malloc(sizeof (camara_l)); //Asignamos un espacio de memoria para la lista de CAMARAS GLOBALES
    camaraG->matrix=(matrix_l *) malloc(sizeof (matrix_l));
    camaraG2 = (camara_l *) malloc(sizeof (camara_l)); //Asignamos un espacio de memoria para la lista de CAMARAS GLOBALES
    camaraG2->matrix=(matrix_l *) malloc(sizeof (matrix_l));
    camaraG3 = (camara_l *) malloc(sizeof (camara_l)); //Asignamos un espacio de memoria para la lista de CAMARAS GLOBALES
    camaraG3->matrix=(matrix_l *) malloc(sizeof (matrix_l));
    camaraG4 = (camara_l *) malloc(sizeof (camara_l)); //Asignamos un espacio de memoria para la lista de CAMARAS GLOBALES
    camaraG4->matrix=(matrix_l *) malloc(sizeof (matrix_l));

    //Camara 4
    camaraG4->numero=4;
    camaraG4->matrix->next=0;
    glMatrixMode(GL_MODELVIEW);
    camaraG4->posicionC.x = 10.0;
    camaraG4->posicionC.y = 10.0;
    camaraG4->posicionC.z = 10.0;  
    gluLookAt(10.0,10.0,10.0,
              4.0,4.0,4.0,
                0.0,1.0,1.0);
    glGetDoublev(GL_MODELVIEW_MATRIX,camaraG4->matrix->matrix);
    glLoadIdentity();
    camaraG4->previous=camaraG3;
    camaraG4->next=camaraG;
    camaraG4->left=-0.1;
    camaraG4->right=0.1;
    camaraG4->bottom=-0.1;
    camaraG4->top=0.1;
    camaraG4->near=0.1;
    camaraG4->far=1000.0;

    camaraG4->ortho_x_min=KG_ORTHO_X_MIN_INIT;
	camaraG4->ortho_x_max=KG_ORTHO_X_MAX_INIT;
	camaraG4->ortho_y_min=KG_ORTHO_Y_MIN_INIT;
	camaraG4->ortho_y_max=KG_ORTHO_Y_MAX_INIT;

    //Camara 3
    camaraG3->numero=3;
    camaraG3->matrix->next=0;
    glMatrixMode(GL_MODELVIEW);
    camaraG3->posicionC.x = 5.0;
    camaraG3->posicionC.y = 5.0;
    camaraG3->posicionC.z = 5.0;
    gluLookAt(5.0,5.0,5.0,
              10.0,10.0,10.0,
                0.0,10.0,5.0);
    glGetDoublev(GL_MODELVIEW_MATRIX,camaraG3->matrix->matrix);
    glLoadIdentity();
    camaraG3->previous=camaraG2;
    camaraG3->next=camaraG4;
    camaraG3->left=-0.1;
    camaraG3->right=0.1;
    camaraG3->bottom=-0.1;
    camaraG3->top=0.1;
    camaraG3->near=0.1;
    camaraG3->far=1000.0;    

    camaraG3->ortho_x_min=KG_ORTHO_X_MIN_INIT;
	camaraG3->ortho_x_max=KG_ORTHO_X_MAX_INIT;
	camaraG3->ortho_y_min=KG_ORTHO_Y_MIN_INIT;
	camaraG3->ortho_y_max=KG_ORTHO_Y_MAX_INIT;

    //Camara 2
    camaraG2->numero=2;
    camaraG2->matrix->next=0;
    camaraG2->posicionC.x = 1.0;
    camaraG2->posicionC.y = 1.0;
    camaraG2->posicionC.z = 1.0;
    glMatrixMode(GL_MODELVIEW);    
    gluLookAt(1.0,1.0,1.0,
              5.0,5.0,5.0,
                0.0,1.0,0.0);
    glGetDoublev(GL_MODELVIEW_MATRIX,camaraG2->matrix->matrix);
    glLoadIdentity();
    camaraG2->previous=camaraG;
    camaraG2->next=camaraG3;
    camaraG2->left=-0.1;
    camaraG2->right=0.1;
    camaraG2->bottom=-0.1;
    camaraG2->top=0.1;
    camaraG2->near=0.1;
    camaraG2->far=1000.0;

    camaraG2->ortho_x_min=KG_ORTHO_X_MIN_INIT;
	camaraG2->ortho_x_max=KG_ORTHO_X_MAX_INIT;
	camaraG2->ortho_y_min=KG_ORTHO_Y_MIN_INIT;
	camaraG2->ortho_y_max=KG_ORTHO_Y_MAX_INIT;

    //Camara 1
    camaraG->numero=1;
    camaraG->matrix->next=0;
    glMatrixMode(GL_MODELVIEW);
    camaraG->posicionC.x = 0.0;
    camaraG->posicionC.y = 0.0;
    camaraG->posicionC.z = 0.0;
    glLoadIdentity();
    glGetDoublev(GL_MODELVIEW_MATRIX,camaraG->matrix->matrix);
    camaraG->previous=0;
    camaraG->next=camaraG2;
    camaraG->left=-0.1;
    camaraG->right=0.1;
    camaraG->bottom=-0.1;
    camaraG->top=0.1;
    camaraG->near=0.1;
    camaraG->far=1000.0;

    camaraG->ortho_x_min=KG_ORTHO_X_MIN_INIT;
	camaraG->ortho_x_max=KG_ORTHO_X_MAX_INIT;
	camaraG->ortho_y_min=KG_ORTHO_Y_MIN_INIT;
	camaraG->ortho_y_max=KG_ORTHO_Y_MAX_INIT;


    //-----------------------------------------------------------------------------------------------------------


    /*Definition of the background color*/
    glClearColor(KG_COL_BACK_R, KG_COL_BACK_G, KG_COL_BACK_B, KG_COL_BACK_A);

    /*Definition of the method to draw the objects*/
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//Dibujamos objetos opacos
}


/** MAIN FUNCTION **/
int main(int argc, char** argv) {

    /*First of all, print the help information*/
    print_help();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(KG_WINDOW_WIDTH, KG_WINDOW_HEIGHT);
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);

    if (iluminacion==2) glShadeModel(GL_SMOOTH);  // hacen falta los vectores normales de cada poligono y vertice
    else
        if(iluminacion==1) glShadeModel(GL_FLAT);  // basta con vector normal del poligono

    /* set the callback functions */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keyboard); //Funcion especial para cargar las flechas

    /* this initialization has to be AFTER the creation of the window */
    initialization();

    /* start the main loop */
    glutMainLoop();
    return 0;
}
