#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <GL/gl.h>

/** DEFINITIONS **/

#define KG_WINDOW_TITLE                     "Práctica GPO"
#define KG_WINDOW_WIDTH                     600
#define KG_WINDOW_HEIGHT                    400
#define KG_WINDOW_X                         50
#define KG_WINDOW_Y                         50


#define KG_MSSG_SELECT_FILE                 "Idatz ezazu fitxategiaren path-a: "
#define KG_MSSG_FILENOTFOUND                "Fitxategi hori ez da existitzen!!"
#define KG_MSSG_INVALIDFILE                 "Arazoren bat egon da fitxategiarekin ..."
#define KG_MSSG_EMPTYFILE                   "Fitxategia hutsik dago"
#define KG_MSSG_FILEREAD                    "Fitxategiaren irakurketa buruta"

#define KG_STEP_SCALE                       2.0f
#define KG_STEP_MOVE                        2.5f
#define KG_STEP_ROTATE                      10.0f
#define KG_STEP_ZOOM                        0.75
#define KG_STEP_CAMERA_ANGLE                5.0f

#define KG_ORTHO_X_MIN_INIT                -5
#define KG_ORTHO_X_MAX_INIT                 5
#define KG_ORTHO_Y_MIN_INIT                -5
#define KG_ORTHO_Y_MAX_INIT                 5
#define KG_ORTHO_Z_MIN_INIT                -100
#define KG_ORTHO_Z_MAX_INIT                 10000

#define KG_COL_BACK_R                       0.30f
#define KG_COL_BACK_G                       0.30f
#define KG_COL_BACK_B                       0.30f
#define KG_COL_BACK_A                       1.00f

#define KG_COL_SELECTED_R                   1.00f
#define KG_COL_SELECTED_G                   0.75f
#define KG_COL_SELECTED_B                   0.00f

#define KG_COL_NONSELECTED_R                1.00f
#define KG_COL_NONSELECTED_G                1.00f
#define KG_COL_NONSELECTED_B                1.00f

#define KG_COL_X_AXIS_R                     1.0f
#define KG_COL_X_AXIS_G                     0.0f
#define KG_COL_X_AXIS_B                     0.0f

#define KG_COL_Y_AXIS_R                     0.0f
#define KG_COL_Y_AXIS_G                     1.0f
#define KG_COL_Y_AXIS_B                     0.0f

#define KG_COL_Z_AXIS_R                     0.0f
#define KG_COL_Z_AXIS_G                     0.0f
#define KG_COL_Z_AXIS_B                     1.0f

#define KG_MAX_DOUBLE                       10E25

/** STRUCTURES **/

/****************************
 * Structure to store the   *
 * coordinates of 3D points *
 ****************************/
typedef struct {
    GLdouble x, y, z;
} point3;

/*****************************
 * Structure to store the    *
 * coordinates of 3D vectors *
 *****************************/
typedef struct {
    GLdouble x, y, z;
} vector3;

/****************************
 * Structure to store the   *
 * colors in RGB mode       *
 ****************************/
typedef struct {
    GLfloat r, g, b;
} color3;

/****************************
 * Structure to store       *
 * objects' vertices         *
 ****************************/
typedef struct {
    point3 coord;                       /* coordinates,x, y, z */
    GLint num_faces;                    /* number of faces that share this vertex */
    
    vector3 normalV;					/* Vector normal del vertice*/
} vertex;

/****************************
 * Structure to store       *
 * objects' faces or        *
 * polygons                 *
 ****************************/
typedef struct {
    GLint num_vertices;                 /* number of vertices in the face */

    vector3 normalF;					/* Vector normal del polígono*/

    GLint *vertex_table;                /* table with the index of each vertex */
} face;


/****************************
 * Estructura para          *
 * almacenar una lista      *
 * de camaras               *
 ****************************/
typedef struct camara_l{
    int numero;							  /*Numero que identifica la camara*/
    GLdouble left;                        /**/
    GLdouble right;                       /**/
    GLdouble bottom;                      /**/
    GLdouble top;                         /**/
    GLdouble near;                        /*Desde como de cerca vemos*/
	GLdouble far;                         /*Como de lejos llegamos a ver*/

	GLdouble ortho_x_min;				  /* Parametros usados*/
	GLdouble ortho_x_max;				  /* para el modo     */
	GLdouble ortho_y_min;				  /* de proyección    */
	GLdouble ortho_y_max;				  /* ortogonal        */

	point3 posicionC;
	struct matrix_l *matrix;              /* Lista de matrices de estado */
    struct camara_l *next;                /*Puntero a la siguiente camara */
    struct camara_l *previous;            /*Puntero a la camara anterior */
} camara_l;


/****************************
 * Estructura para          *
 * almacenar una lista      *
 * de matrices              *
 ****************************/
typedef struct matrix_l{
    GLdouble matrix[16];                /*Matriz de 4x4 que guarda la matriz activa*/
    struct matrix_l *next;              /*Lista de matrices del objeto*/
} matrix_l;

/****************************
 * Structure to store a     *
 * pile of 3D objects       *
 ****************************/
typedef struct object3d{
    GLint num_vertices;                 /* number of vertices in the object*/
    vertex *vertex_table;               /* table of vertices */
    GLint num_faces;                    /* number of faces in the object */
    face *face_table;                   /* table of faces */
    point3 min;                         /* coordinates' lower bounds */
    point3 max;                         /* coordinates' bigger bounds */
    matrix_l *matrix;                   /* Lista de matrices de estado */
    camara_l *camara;                   /* Lista de CAMARAS */
    struct object3d *next;              /* next element in the pile of objects */
}object3d;

#endif // DEFINITIONS_H
