#include "definitions.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/** EXTERNAL VARIABLES **/

extern GLdouble _window_ratio;
extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern object3d *_first_object;
extern object3d *_selected_object;
extern camara_l *camaraG;

extern int modo;
extern int modog;
extern int modop;
extern int iluminacion;

float mat_specular[] = { 0.1, 0.5, 0.5, 1.0 };
float mat_shininess[] = { 50.0 };
float sun_position[] = { 1.0, 1.0, 1.0, 0.0 };

float dfoco[]={0.0, 0.0, -1.0};
float angulofoco=25.0;

float pbombilla[]={1.0, 1.0, 1.0, 1.0};

float angulofoco2=15.0;
float pfoco2[]={-1.0, -1.0, -1.0, 1.0};
/**
 * @brief Function to draw the axes
 */
void draw_axes()
{
    /*Draw X axis*/
    glColor3f(KG_COL_X_AXIS_R,KG_COL_X_AXIS_G,KG_COL_X_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(KG_MAX_DOUBLE,0,0);
    glVertex3d(-1*KG_MAX_DOUBLE,0,0);
    glEnd();
    /*Draw Y axis*/
    glColor3f(KG_COL_Y_AXIS_R,KG_COL_Y_AXIS_G,KG_COL_Y_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,KG_MAX_DOUBLE,0);
    glVertex3d(0,-1*KG_MAX_DOUBLE,0);
    glEnd();
    /*Draw Z axis*/
    glColor3f(KG_COL_Z_AXIS_R,KG_COL_Z_AXIS_G,KG_COL_Z_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,0,KG_MAX_DOUBLE);
    glVertex3d(0,0,-1*KG_MAX_DOUBLE);
    glEnd();
}


/**
 * @brief Callback reshape function. We just store the new proportions of the window
 * @param width New width of the window
 * @param height New height of the window
 */
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    /*  Take care, the width and height are integer numbers, but the ratio is a GLdouble so, in order to avoid
     *  rounding the ratio to integer values we need to cast width and height before computing the ratio */
    _window_ratio = (GLdouble) width / (GLdouble) height;
}



double randfrom(double min, double max) 
{
    srand ( time ( NULL));
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

//Genera un número aleatorio entre 0 y max
/*double random( int max)
{
    double random_value;

    srand ( time ( NULL));

    random_value = (double)rand()%RAND_MAX*max;//float in range 0 to max

    //printf ( "Valor aleatorio generado: %f\n", random_value);

    return random_value;
}/*


/**
 * @brief Callback display function
 */
void display(void) {
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    GLint v_index, v, f;
    object3d *aux_obj = _first_object; //Lista de objetos cargados

    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    /* Define the projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Tenemos que saber si esta activado el modo ortogonal o no, si esta activado realiza el codigo siguiente, de lo contrario tendremos que modificar la matriz de proyeccion a traves de la funcion frustrum
    if(modop==0) {
        /*When the window is wider than our original projection plane we extend the plane in the X axis*/
          if ((_ortho_x_max - _ortho_x_min) / (_ortho_y_max - _ortho_y_min) < _window_ratio) {
            /* New width */
              GLdouble wd = (_ortho_y_max - _ortho_y_min) * _window_ratio;
            /* Midpoint in the X axis */
              GLdouble midpt = (_ortho_x_min + _ortho_x_max) / 2;
            /*Definition of the projection*/
              glOrtho(midpt - (wd / 2), midpt + (wd / 2), _ortho_y_min, _ortho_y_max, _ortho_z_min, _ortho_z_max);
          } 
          else {/* In the opposite situation we extend the Y axis */
            /* New height */
              GLdouble he = (_ortho_x_max - _ortho_x_min) / _window_ratio;
            /* Midpoint in the Y axis */
              GLdouble midpt = (_ortho_y_min + _ortho_y_max) / 2;
              /*Definition of the projection*/
              //glOrtho(_ortho_x_min, _ortho_x_max, midpt - (he / 2), midpt + (he / 2), _ortho_z_min, _ortho_z_max);
              glOrtho(camaraG->ortho_x_min,camaraG->ortho_x_max,camaraG->ortho_y_min,camaraG->ortho_y_max,camaraG->near,camaraG->far);
          }
    	printf("-------------------------------\n");
    	printf("Left:%lf  ",camaraG->ortho_x_min);
    	printf("Right:%lf  ",camaraG->ortho_x_max);
    	printf("Bottom%lf  \n",camaraG->ortho_y_min);
    	printf("Top:%lf  ",camaraG->ortho_y_max);
    	printf("Near:%lf  ",camaraG->near);
    	printf("Far:%lf\n",camaraG->far);
    	printf("-------------------------------\n");
    }
    else {
    //---------------------------------------------------------------
        glFrustum(camaraG->left,camaraG->right,camaraG->bottom,camaraG->top,camaraG->near,camaraG->far);
        if (modog==1 && modo==3){
            printf("Left:%lf  ",camaraG->left);
            printf("Right:%lf  ",camaraG->right);
            printf("Bottom%lf  \n",camaraG->bottom);
            printf("Top:%lf  ",camaraG->top);
            printf("Near:%lf  ",camaraG->near);
            printf("Far:%lf\n",camaraG->far);
        }
    //---------------------------------------------------------------
    }

    /* Now we start drawing the object */
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(camaraG->matrix->matrix);
    //glLoadIdentity();//Aqui tenemos que cargar la matrix de la camara
    
    /*First, we draw the axes*/
    draw_axes();

    //Establecemos las luces
    
    float light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    float light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    float light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    //Sol
    glLightfv(GL_LIGHT0, GL_POSITION, sun_position);

    //Bombilla
    glLightfv(GL_LIGHT1, GL_POSITION, pbombilla);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);

    //Foco externo
    glLightfv(GL_LIGHT3, GL_POSITION, pfoco2);
    glLightfv(GL_LIGHT3, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light1_specular);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, angulofoco2);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dfoco);
    
    /*Now each of the objects in the list*/
    while (aux_obj != 0) {
        float pfoco[]={_selected_object->matrix->matrix[12],
                            _selected_object->matrix->matrix[13],
                            _selected_object->matrix->matrix[14], 1.0};

        float dbombilla[]={_selected_object->matrix->matrix[12],
                            _selected_object->matrix->matrix[13],
                            _selected_object->matrix->matrix[14]};

        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dbombilla);

        //Foco relacionado al objeto
        glLightfv(GL_LIGHT2, GL_POSITION, pbombilla);
        glLightfv(GL_LIGHT2, GL_AMBIENT, light1_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light1_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light1_specular);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dbombilla);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, angulofoco);


        glPushMatrix();//Vamos a ir de la camara global a la del objeto?
        /* Select the color, depending on whether the current object is the selected one or not */
        if (aux_obj == _selected_object){
            glColor3f(KG_COL_SELECTED_R,KG_COL_SELECTED_G,KG_COL_SELECTED_B);
        }else{
            glColor3f(KG_COL_NONSELECTED_R,KG_COL_NONSELECTED_G,KG_COL_NONSELECTED_B);
        }

        
        //---------------------------------------------------------------------------------
        //Cargar la primera matriz que tenemos guardada en la pila de matrices
        //Aqui tenemos que multiplicar la matriz del objeto por la cargada que es la de la camara
        glMultMatrixd(aux_obj->matrix->matrix);
        GLfloat mat_specular[] = { 0.63,0.65,0.37,1.0};
        GLfloat mat_ambient[] = { 0.25,0.20,0.07,1.0};
        GLfloat mat_diffuse[] = { 0.75,0.61,0.23,1.0};
        GLfloat mat_shininess[] = { 51.2 };
        //---------------------------------------------------------------------------------
        /* Draw the object; for each face create a new polygon with the corresponding vertices */
        for (f = 0; f < aux_obj->num_faces; f++) {
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
            glBegin(GL_POLYGON);
            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                v_index = aux_obj->face_table[f].vertex_table[v];
                //Especificamos cual va a ser el vector normal
                glNormal3d(aux_obj->vertex_table[aux_obj->face_table[f].vertex_table[v]].normalV.x,
                    aux_obj->vertex_table[aux_obj->face_table[f].vertex_table[v]].normalV.y,
                    aux_obj->vertex_table[aux_obj->face_table[f].vertex_table[v]].normalV.z);

                glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                        aux_obj->vertex_table[v_index].coord.y,
                        aux_obj->vertex_table[v_index].coord.z);

            }
            glEnd();
        }
        
        glPopMatrix();//Volvemos a la camara global?
        aux_obj = aux_obj->next;
    }
    glLoadIdentity ();
    /*Do the actual drawing*/
    glFlush();
}
