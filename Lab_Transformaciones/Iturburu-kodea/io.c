#include "definitions.h"
#include "load_obj.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

extern object3d * _first_object;
extern object3d * _selected_object;
extern camara_l *camaraG;
extern camara_l *camaraG2;
extern camara_l *camaraG3;
extern camara_l *camaraG4;

//Direccion del sol
extern float sun_position[];
//Posicion y angulo del foco del objeto
extern float angulofoco;
extern float pbombilla[];

//Posicion, angulo y direccion del foco externo
extern float angulofoco2;
extern float pfoco2[];
extern float dfoco[];

extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

camara_l *camara_auxiliar;
matrix_l *matrizaux2;
matrix_l *matrizaux1;
int luces[4]={0,0,0,0};

int modo=2; //Variable usada para elegir entre rotacion(2), traslacion(1) o escalado(3)
int modo2=0; //Cambios respecto al mundo modo2=1 o respecto al objeto modo2=0
int modog=0; //Realizar los cambios en la camara (modog=1), o en el objeto (modog=0) o la iluminacion (modog=2)
int modop=0; //Modo de perspectica ortogonal (modop=0) o perspectiva(modop=1)
int modoV=0; //Modo de vuelo o analisis. Modo de vuelo (modoV=0), modo de analisis (modoV=1)
int modoprueba=0; //Modo camara objeto. Activado (modoprueba=1), Desactivado (modoprueba=0)
int modoAnalisis=0; //Modo analisis. Activado (modoAnalisis=1), Desactivado (modoAnalisis=0)
int iluminacion=0; //iluminacion=0 (desactivada),  iluminacion=1 (FLAT),iluminacion=2 (SMOOTH)
int *luzseleccionada=&luces[0];//Puntero a la luz seleccionada

int moverluces=0;//0 no se mueven luces, 1 se mueven las luces
/**
 * @brief This function just prints information about the use
 * of the keys
 */
void print_help(){
    printf("Practica de Transformaciones. Este programa visualiza\n");
    printf("y modifica objetos en 3D.  \n\n");
    printf("\n\n");
    printf("FUNCIONES PRINCIPALES: \n");
    printf("<?>\t\t Visualizar ayuda \n");
    printf("<ESC>\t\t Salir del programa \n");
    printf("<F>\t\t Cargar un objeto\n");
    printf("<TAB>\t\t Cambiar la seleccion de un objeto cargado\n");
    printf("<DEL>\t\t Borrar el objeto seleccionado\n");
    printf("<CTRL + ->\t Aumentar el zoom / aumentar el volumen de vision / aumentar el angulo de apertura de un foco\n");
    printf("<CTRL + +>\t Reducir el zoom / reducir el volumen de vision / reducir el angulo de apertura de un foco\n");
    printf("<o / O>\t\t Activar transformaciones sobre objetos\n");
    printf("<l / L>\t\t Activar transformaciones respecto al objeto\n");
    printf("<g / G>\t\t Activar transformaciones respecto al mundo\n");
    printf("------------------------------------------------------------------------\n");
    printf("<m / M>\t\t Activar la traslacion\n");
    printf("<b / B>\t\t Activar la rotacion\n");
    printf("<t / T>\t\t Activar el escalado\n");
    printf("------------------------------------------------------------------------\n");
    printf("<k / K>\t\t Activar transformaciones sobre camaras\n");
    printf("<c>\t\t Cambiar de camara\n");
    printf("<C>\t\t Cambiar a la camara del objeto\n");
    printf("<g / C>\t\t Modo analisis\n");
    printf("<l / L>\t\t Modo vuelo\n");
    printf("<p / P>\t\t Cambio de tipo de perspectiva\n");
    printf("------------------------------------------------------------------------\n");
    printf("<f9>\t\t Activar/Desactivar el modo iluminacion\n");
    printf("<f1-f4>\t\t Activar/desactivar fuentes de luz\n");
    printf("<1-4>\t\t Seleccionar fuente de luz\n");
    printf("\n\n");
}

//Guardamos el estado de la matriz actual (ya esta cargada) en el objeto o en la camara
void guardar_estado()
{
    if(modog==0) {//Guardamos los cambios realizados sobre el objeto
        //Creamos una matriz auxiliar que apunte a la matrix de estado actual
        matrix_l *matrizaux;
        matrizaux = (matrix_l *) malloc(sizeof (matrix_l));
        matrizaux->next=_selected_object->matrix;
        //Guardamos la matriz cargada dentro de la auxiliar
        glGetDoublev(GL_MODELVIEW_MATRIX,matrizaux->matrix);
        //Hacemos que el objeto apunte a la matriz auxiliar
        _selected_object->matrix=matrizaux;   
    }
    if(modog==1) {//Guardamos los cambios realizados sobre la camara
        matrix_l *matrizaux;
        matrizaux = (matrix_l *) malloc(sizeof (matrix_l));
        matrizaux->next=camaraG->matrix;
        glGetDoublev(GL_MODELVIEW_MATRIX,matrizaux->matrix);
        camaraG->matrix=matrizaux; 
    }
}

//Representa las matrices Mcsr y la del objeto
void representar_matriz() {
    printf("Matriz Cambio Sistema de Referencia:\n");
    printf("-----------------------------------------------------------------\n");
    printf("%f %f %f %f\n",camaraG->matrix->matrix[0],camaraG->matrix->matrix[4],camaraG->matrix->matrix[8],camaraG->matrix->matrix[12]);
    printf("%f %f %f %f\n",camaraG->matrix->matrix[1],camaraG->matrix->matrix[5],camaraG->matrix->matrix[9],camaraG->matrix->matrix[13]);
    printf("%f %f %f %f\n",camaraG->matrix->matrix[2],camaraG->matrix->matrix[6],camaraG->matrix->matrix[10],camaraG->matrix->matrix[14]);
    printf("%f %f %f %f\n",camaraG->matrix->matrix[3],camaraG->matrix->matrix[7],camaraG->matrix->matrix[11],camaraG->matrix->matrix[15]);
    printf("-----------------------------------------------------------------\n");

    printf("Matriz Del Objeto:\n");
    printf("-----------------------------------------------------------------\n");
    printf("%f %f %f %f\n",_selected_object->matrix->matrix[0],_selected_object->matrix->matrix[4],_selected_object->matrix->matrix[8],_selected_object->matrix->matrix[12]);
    printf("%f %f %f %f\n",_selected_object->matrix->matrix[1],_selected_object->matrix->matrix[5],_selected_object->matrix->matrix[9],_selected_object->matrix->matrix[13]);
    printf("%f %f %f %f\n",_selected_object->matrix->matrix[2],_selected_object->matrix->matrix[6],_selected_object->matrix->matrix[10],_selected_object->matrix->matrix[14]);
    printf("%f %f %f %f\n",_selected_object->matrix->matrix[3],_selected_object->matrix->matrix[7],_selected_object->matrix->matrix[11],_selected_object->matrix->matrix[15]);
    printf("-----------------------------------------------------------------\n");
}

//Modifica la camara del objeto cuando se mueve el objeto
void camera_update() {
	if(modoprueba==1) {
		glLoadIdentity();
		camaraG->posicionC.x = _selected_object->matrix->matrix[12];
		camaraG->posicionC.y = _selected_object->matrix->matrix[13];
		camaraG->posicionC.z = _selected_object->matrix->matrix[14];
		gluLookAt(_selected_object->matrix->matrix[12],_selected_object->matrix->matrix[13],_selected_object->matrix->matrix[14],
              _selected_object->matrix->matrix[12]-_selected_object->matrix->matrix[8],_selected_object->matrix->matrix[13]-_selected_object->matrix->matrix[9],_selected_object->matrix->matrix[14]-_selected_object->matrix->matrix[10],
                _selected_object->matrix->matrix[4],_selected_object->matrix->matrix[5],_selected_object->matrix->matrix[6]);
		glGetDoublev(GL_MODELVIEW_MATRIX,camaraG->matrix->matrix);

		printf("Matriz Cambio Sistema de Referencia tras camara del objeto:\n");
		printf("%f %f %f %f\n",camaraG->matrix->matrix[0],camaraG->matrix->matrix[4],camaraG->matrix->matrix[8],camaraG->matrix->matrix[12]);
    	printf("%f %f %f %f\n",camaraG->matrix->matrix[1],camaraG->matrix->matrix[5],camaraG->matrix->matrix[9],camaraG->matrix->matrix[13]);
    	printf("%f %f %f %f\n",camaraG->matrix->matrix[2],camaraG->matrix->matrix[6],camaraG->matrix->matrix[10],camaraG->matrix->matrix[14]);
    	printf("%f %f %f %f\n",camaraG->matrix->matrix[3],camaraG->matrix->matrix[7],camaraG->matrix->matrix[11],camaraG->matrix->matrix[15]);
	}
}

double absoluto(double a) {
	if(a>0) return a;
	else	return -a;
}



void traslate(float a, float b, float c)
{
    if(_selected_object!=NULL) {
        if(modog==0) {//Hacemos el cambio respecto al objeto
            if(modo2==0){
                printf("Traslacion respecto al objeto\n");
                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixd(_selected_object->matrix->matrix);
                glTranslatef(a, b, c);
                guardar_estado();
                representar_matriz();
            }
            else {//Hacemos el cambio respecto al mundo
                printf("Traslacion respecto al mundo\n");
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glTranslatef(a, b, c);
                glMatrixMode(GL_MODELVIEW);
                glMultMatrixd(_selected_object->matrix->matrix);
                guardar_estado();
                representar_matriz();
                }
        }
        //Cambio respecto a la camara
        else {
        	if (modoV==0)//Modo vuelo
            {
	            glMatrixMode(GL_MODELVIEW);
	            glLoadIdentity();
	            glTranslatef(a,b,c);
	            printf("Posicion de la camara X %f Y %f Z %f\n",camaraG->posicionC.x,camaraG->posicionC.y,camaraG->posicionC.z);
	            camaraG->posicionC.x += a;
	            camaraG->posicionC.y += b;
	            camaraG->posicionC.z += c;
	            printf("Posicion NUEVA de la camara X %f Y %f Z %f\n",camaraG->posicionC.x,camaraG->posicionC.y,camaraG->posicionC.z);
	            glMatrixMode(GL_MODELVIEW);
	            glMultMatrixd(camaraG->matrix->matrix);
	            guardar_estado();
	            representar_matriz();
	        }
	        else {//Modo analisis, movernos hacia adelante o hacia atras
	        	printf("posicionC.z:%f Z objeto:%f\n",camaraG->posicionC.z,_selected_object->matrix->matrix[14]);
	        	double distanciaZ=absoluto(-camaraG->posicionC.z - _selected_object->matrix->matrix[14]);
	        	printf("DistanciaZ:%f\n",distanciaZ);
	        	if(distanciaZ>c) {
	        		glLoadIdentity();
		            glTranslatef(a,b,c);
		            camaraG->posicionC.z += c;
		            glMatrixMode(GL_MODELVIEW);
		            glMultMatrixd(camaraG->matrix->matrix);
		            guardar_estado();
		            printf("CamaraG->posicionC.z:%f\n",camaraG->posicionC.z);
		            representar_matriz();
	        	}
	        	else{
	        		printf("No nos podemos mover mas adelante\n");
	        		printf("DistanciaZ:%f <= Movimiento:%f\n",distanciaZ,c);
	        	}
	        }
        }
        if (moverluces==1)
        {
        	if(luzseleccionada==&luces[1]){
	        	printf("Trasladamos la bombilla\n");
	        	pbombilla[0]+=a;
	        	pbombilla[1]+=b;
	        	pbombilla[2]+=c;
	        	glLightfv(GL_LIGHT1, GL_POSITION, pbombilla);
	        	printf("Posicion bombilla: X:%f Y:%f Z:%f\n",pbombilla[0],pbombilla[1],pbombilla[2]);
        	}
        	else if(luzseleccionada==&luces[3]){
	        	printf("Trasladamos el foco externo\n");
	        	pfoco2[0]+=a;
	        	pfoco2[1]+=b;
	        	pfoco2[2]+=c;
	        	glLightfv(GL_LIGHT3, GL_POSITION, pfoco2);
	        	printf("Posicion del foco externo: X:%f Y:%f Z:%f\n",pfoco2[0],pfoco2[1],pfoco2[2]);
        	}
        	else {
        		printf("La fuente de luz seleccionada no puede ser traladada.\n");
        	}

        }
    }
    else {
        printf("ERROR: No hay ningun objeto cargado!\n");
    }      
}




if(){
}

else{}


void scale(float a, float b, float c)
{
    if(_selected_object!=NULL) {
        if(modog==0) {
            //Hacemos el cambio respecto al objeto
            if(modo2==0){
                printf("Escalado respecto al objeto\n");
                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixd(_selected_object->matrix->matrix);
                glScalef(a, b, c);
                guardar_estado();
                representar_matriz();
            }
            //Hacemos el cambio respecto al mundo
            else {
                printf("Escalado respecto al mundo\n");
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glScalef(a, b, c);
                glMatrixMode(GL_MODELVIEW);
                glMultMatrixd(_selected_object->matrix->matrix);
                guardar_estado();
                representar_matriz();
            }
        }
        else {//Cambio respecto a la camara
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glScalef(a, b, c);
            glMatrixMode(GL_MODELVIEW);
            glMultMatrixd(camaraG->matrix->matrix);
            guardar_estado();
            representar_matriz();
        }
    }
    else {
        printf("ERROR: No hay ningun objeto cargado!\n");
    }
}


void rotate(float angle,float a, float b, float c)
{
    //Hacemos el cambio respecto al objeto
    if(_selected_object!=NULL) {
        if(modog==0) {//Sobre el objeto
            if(modo2==0){
                printf("Rotacion respecto al objeto\n");
                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixd(_selected_object->matrix->matrix);
                glRotatef(angle,a, b, c);
                guardar_estado();
                representar_matriz();
            }
        //Hacemos el cambio respecto al mundo
            else {
                printf("Rotacion respecto al mundo\n");
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glRotatef(angle,a, b, c);
                glMatrixMode(GL_MODELVIEW);
                glMultMatrixd(_selected_object->matrix->matrix);
                guardar_estado();
                representar_matriz();
            }   
        }
        else {//Cambio respecto a la camara
            if (modoV==0)//Modo vuelo
            {
            	glMatrixMode(GL_MODELVIEW);
	            glLoadIdentity();
	            glRotatef(angle,a, b, c);
	            glMatrixMode(GL_MODELVIEW);
	            glMultMatrixd(camaraG->matrix->matrix);
	            guardar_estado();
	            representar_matriz();
            }
            else{//modo analisis
            	printf("ROTACION RESPECTO AL OBJETO???\n");
            	matrix_l *matrizaux;
        		matrizaux = (matrix_l *) malloc(sizeof (matrix_l));
        		
        		printf("Posicion de la camara\n");
        		printf("Posicion de la camara X %f Y %f Z %f\n",camaraG->posicionC.x,camaraG->posicionC.y,camaraG->posicionC.z);
        		//Matriz inversa de la MCSR
        		matrizaux->matrix[0] = camaraG->matrix->matrix[0];
				matrizaux->matrix[1] = camaraG->matrix->matrix[4];
				matrizaux->matrix[2] = camaraG->matrix->matrix[8];	
				matrizaux->matrix[3] = 0.0;

	
				matrizaux->matrix[4] = camaraG->matrix->matrix[1];
				matrizaux->matrix[5] = camaraG->matrix->matrix[5];	
				matrizaux->matrix[6] = camaraG->matrix->matrix[9];	
				matrizaux->matrix[7] = 0.0;
	
	
				matrizaux->matrix[8] = camaraG->matrix->matrix[2];	
				matrizaux->matrix[9] = camaraG->matrix->matrix[6];
				matrizaux->matrix[10] = camaraG->matrix->matrix[10];
				matrizaux->matrix[11] = 0.0;
				matrizaux->matrix[12] = camaraG->posicionC.x;
        		matrizaux->matrix[13] = camaraG->posicionC.y;
        		matrizaux->matrix[14] = camaraG->posicionC.z;
				matrizaux->matrix[15] = 1.0;
				
				
        		glLoadIdentity();
        		glMatrixMode(GL_MODELVIEW);
        		glTranslatef(_selected_object->matrix->matrix[12],
		    		_selected_object->matrix->matrix[13],
		    		_selected_object->matrix->matrix[14]);
		     	glRotatef(angle,a,b,c);
        		glTranslatef(-_selected_object->matrix->matrix[12],
		    		-_selected_object->matrix->matrix[13],
		    		-_selected_object->matrix->matrix[14]);
        		glMultMatrixd(matrizaux->matrix);
		    
        		
        		printf("Posicion de matrizaux\n");
        		printf("Posicion de la camara X %f Y %f Z %f\n",matrizaux->matrix[12],camaraG->posicionC.y,camaraG->posicionC.z);
        		printf("Posicion de la camara\n");
        		printf("Posicion de la camara X %f Y %f Z %f\n",camaraG->posicionC.x,camaraG->posicionC.y,camaraG->posicionC.z);
        		
       			printf("Matrizaux = Inversa de la Mcsr:\n");
       		 	printf("%f %f %f %f\n",matrizaux->matrix[0],matrizaux->matrix[4],matrizaux->matrix[8],matrizaux->matrix[12]);
    			printf("%f %f %f %f\n",matrizaux->matrix[1],matrizaux->matrix[5],matrizaux->matrix[9],matrizaux->matrix[13]);
    			printf("%f %f %f %f\n",matrizaux->matrix[2],matrizaux->matrix[6],matrizaux->matrix[10],matrizaux->matrix[14]);
    			printf("%f %f %f %f\n",matrizaux->matrix[3],matrizaux->matrix[7],matrizaux->matrix[11],matrizaux->matrix[15]);	
        			
        		
        		glMatrixMode(GL_MODELVIEW);
        		glGetDoublev(GL_MODELVIEW_MATRIX,matrizaux->matrix);
        		camaraG->posicionC.x = matrizaux->matrix[12];
        		camaraG->posicionC.y = matrizaux->matrix[13];
        		camaraG->posicionC.z = matrizaux->matrix[14];

				//Generamos la Mcsr a partir de la inversa
				camaraG->matrix->matrix[0] = matrizaux->matrix[0];
				camaraG->matrix->matrix[1] = matrizaux->matrix[4];
				camaraG->matrix->matrix[2] = matrizaux->matrix[8];	
				camaraG->matrix->matrix[3] = 0.0;

	
				camaraG->matrix->matrix[4] = matrizaux->matrix[1];
				camaraG->matrix->matrix[5] = matrizaux->matrix[5];	
				camaraG->matrix->matrix[6] = matrizaux->matrix[9];	
				camaraG->matrix->matrix[7] = 0.0;
	
	
				camaraG->matrix->matrix[8] = matrizaux->matrix[2];	
				camaraG->matrix->matrix[9] = matrizaux->matrix[6];
				camaraG->matrix->matrix[10] = matrizaux->matrix[10];
				camaraG->matrix->matrix[11] = 0.0;
	
	
				camaraG->matrix->matrix[12] = -((matrizaux->matrix[0]*matrizaux->matrix[12])+(matrizaux->matrix[1]*matrizaux->matrix[13]) + (matrizaux->matrix[2]*matrizaux->matrix[14]));
				camaraG->matrix->matrix[13] = -((matrizaux->matrix[4]*matrizaux->matrix[12])+(matrizaux->matrix[5]*matrizaux->matrix[13]) + (matrizaux->matrix[6]*matrizaux->matrix[14]));				
				camaraG->matrix->matrix[14] = -((matrizaux->matrix[8]*matrizaux->matrix[12])+(matrizaux->matrix[9]*matrizaux->matrix[13]) + (matrizaux->matrix[10]*matrizaux->matrix[14]));
				camaraG->matrix->matrix[15] = 1.0;

				glLoadMatrixd(camaraG->matrix->matrix);	
        		guardar_estado();
        		representar_matriz();
            }  
        }
    }
    else {
        printf("ERROR: No hay ningun objeto cargado!\n");
    }
}

void deshacer() {
    if(_selected_object!=NULL) {
    	if(modog==0) {
        	if(_selected_object->matrix->next!=0) {
            	glMatrixMode(GL_MODELVIEW);
            	matrix_l *borrar = _selected_object->matrix;                //Creamos una matriz que apunte al estado actual de la pila
            	_selected_object->matrix=_selected_object->matrix->next;    //Hacemos que el objeto apunte al siguiente
            	free(borrar);                                               //Eliminamos el antiguo estado de la pila
            	glLoadMatrixd(_selected_object->matrix->matrix);            //Cargamos la matriz de nuevo
        	}
        	else {
            	printf("ERROR: El objeto no tiene mas estados anteriores\n");
        	}
    	}
    	if(modog==1) {
    		if(camaraG->matrix->next!=0) {
            	glMatrixMode(GL_MODELVIEW);
            	matrix_l *borrar = camaraG->matrix;                			//Creamos una matriz que apunte al estado actual de la pila
            	camaraG->matrix=camaraG->matrix->next;    					//Hacemos que el objeto apunte al siguiente
            	free(borrar);                                               //Eliminamos el antiguo estado de la pila
            	glLoadMatrixd(camaraG->matrix->matrix);            			//Cargamos la matriz de nuevo
        	}
        	else {
            	printf("ERROR: La camara no tiene mas estados anteriores\n");
        	}
    	}
    }
    else {
        printf("ERROR: No hay ningun objeto cargado!\n");
    }

}


void special_keyboard(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_F9:
            if(iluminacion==0) {
                printf("Usted acaba de activar el modo de iluminacion.\n");
                iluminacion=1;

                luces[0]=1; luces[1]=1; luces[2]=1; luces[3]=1;
                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);
                glEnable(GL_LIGHT1);
                glEnable(GL_LIGHT2);
                glEnable(GL_LIGHT3);
                glEnable(GL_DEPTH_TEST);
            }
            else{
                iluminacion=0;
                glDisable(GL_LIGHTING);
                luces[0]=0; luces[1]=0; luces[2]=0; luces[3]=0;
            }
        break;

        case GLUT_KEY_F12:
            if(iluminacion==1) {
                iluminacion=2;
                printf("Usted acaba de activar el modo SMOOTH.\n");
                glShadeModel(GL_SMOOTH);
            }
            else{
                iluminacion=1;
                printf("Usted acaba de activar el modo FLAT.\n");
                glShadeModel(GL_FLAT);
            }
        break;

        case GLUT_KEY_F1:
            if(iluminacion!=0) {
                if(luces[0]==1) {//Luz 0 activada, la desactivamos
                    luces[0]=0;
                    printf("Se ha DESACTIVADO la fuente de luz 0, el sol.\n");
                    glDisable(GL_LIGHT0);
                }
                else{//Luz 0 desactivada, la activamos
                    luces[0]=1;
                    printf("Se ha ACTIVADO la fuente de luz 0, el sol.\n");
                    glEnable(GL_LIGHT0);
                }
            }
            else {
                printf("Activa la iluminacion para encender/apagar fuentes de luz.\n");
            }
        break;

        case GLUT_KEY_F2:
            if(iluminacion!=0) {
                if(luces[1]==1) {//Luz 1 activada, la desactivamos
                    luces[1]=0;
                    printf("Se ha DESACTIVADO la fuente de luz 1, la bombilla.\n");
                    glDisable(GL_LIGHT1);
                }
                else{//Luz 1 desactivada, la activamos
                    luces[1]=1;
                    printf("Se ha ACTIVADO la fuente de luz 1, la bombilla.\n");
                    glEnable(GL_LIGHT1);
                }
            }
            else {
                printf("Activa la iluminacion para encender/apagar fuentes de luz.\n");
            }
        break;

        case GLUT_KEY_F3:
            if(iluminacion!=0) {
                if(luces[2]==1) {//Luz 2 activada, la desactivamos
                    luces[2]=0;
                    printf("Se ha DESACTIVADO la fuente de luz 2, el foco relacionado al objeto.\n");
                    glDisable(GL_LIGHT2);
                }
                else{//Luz 2 desactivada, la activamos
                    luces[2]=1;
                    printf("Se ha ACTIVADO la fuente de luz 2, el foco relacionado al objeto.\n");
                    glEnable(GL_LIGHT2);
                }
            }
            else {
                printf("Activa la iluminacion para encender/apagar fuentes de luz.\n");
            }
        break;

        case GLUT_KEY_F4:
            if(iluminacion!=0) {
                if(luces[3]==1) {//Luz 3 activada, la desactivamos
                    luces[3]=0;
                    printf("Se ha DESACTIVADO la fuente de luz 3, el foco externo.\n");
                    glDisable(GL_LIGHT3);
                }
                else{//Luz 3 desactivada, la activamos
                    luces[3]=1;
                    printf("Se ha ACTIVADO la fuente de luz 3, el foco externo.\n");
                    glEnable(GL_LIGHT3);
                }
            }
            else {
                printf("Activa la iluminacion para encender/apagar fuentes de luz.\n");
            }
        break;

        case GLUT_KEY_UP:
    		if(modoV==0) {//Modo Vuelo
	            if(modo==1) {
	                printf("Usted acaba de pulsar Flecha Arriba con el modo de Traslacion activado\n");
	                traslate(0,KG_STEP_MOVE,0);
	            }
	            if(modo==2) {
	                printf("Usted acaba de pulsar Flecha Arriba con el modo de Rotacion activado\n");
	                rotate(KG_STEP_CAMERA_ANGLE,KG_STEP_MOVE, 0, 0);
	            }
	            if(modo==3) {
	            	if(modog==0) {
	                	printf("Usted acaba de pulsar Flecha Arriba con el modo de Escalado activado\n");
	                	scale(1, KG_STEP_SCALE, 1);
	            	}
	            	if(modog==1) {
	            		printf("Usted acaba de pulsar Flecha Arriba, ha aumentado el volumen de visión en Y\n");
						camaraG->top=camaraG->top*1.1;
						camaraG->bottom=camaraG->bottom*1.1;
	            	}
	            }
        	}
        	else {//Rotacion especial modo analisis
        		printf("Modo Analisis activado, solo se permite rotar con Flecha Arriba\n");
        		rotate(KG_STEP_CAMERA_ANGLE,camaraG->matrix->matrix[0],camaraG->matrix->matrix[4],camaraG->matrix->matrix[8]);
        	}
        	printf("modog:%d\n",modog);
        	if (moverluces==1){
        		traslate(0,KG_STEP_MOVE,0);
        	}
            camera_update();
        break;

        case GLUT_KEY_DOWN:
        	if(modoV==0) {//Modo Vuelo
	            if(modo==1) {
	                printf("Usted acaba de pulsar Flecha Abajo con el modo de Traslacion activado\n");
	                traslate(0,-KG_STEP_MOVE,0);
	            }
	            if(modo==2) {
	                printf("Usted acaba de pulsar Flecha Abajo con el modo de Rotacion activado\n");
	                rotate(KG_STEP_CAMERA_ANGLE,-KG_STEP_MOVE, 0, 0);
	            }
	            if(modo==3) {
	            	if (modog==0)
	            	{
	            		printf("Usted acaba de pulsar Flecha Abajo con el modo de Escalado activado\n");
	                	scale(1, 1/KG_STEP_SCALE, 1);
	            	}
	            	if (modog==1) {
	            		printf("Usted acaba de pulsar Flecha Abajo, ha reducido el volumen de visión en Y\n");
                		camaraG->bottom=camaraG->bottom*0.9;
                		camaraG->top=camaraG->top*0.9;
	            	}
	           	}
	           }
           else {//Rotacion especial modo analisis
    			printf("Modo Analisis activado, solo se permite rotar con Flecha Abajo\n");
    			rotate(KG_STEP_CAMERA_ANGLE,-camaraG->matrix->matrix[0],-camaraG->matrix->matrix[4],-camaraG->matrix->matrix[8]);
    		}
    		printf("modog:%d\n",modog);
    		if (moverluces==1){
        		traslate(0,-KG_STEP_MOVE,0);
        	}
	           	camera_update();
        break;

        case GLUT_KEY_LEFT:
        	if(modoV==0) {//Modo Vuelo
	            if(modo==1) {
	                printf("Usted acaba de pulsar Flecha Izquierda con el modo de Traslacion activado\n");
	                traslate(-KG_STEP_MOVE,0,0);
	            }
	            if(modo==2) {
	                printf("Usted acaba de pulsar Flecha Izquierda con el modo de Rotacion activado\n");
	                rotate(KG_STEP_CAMERA_ANGLE,0,-KG_STEP_MOVE, 0);
	            }

	            if(modo==3) {
	            	if(modog==0) {
	                	printf("Usted acaba de pulsar Flecha Izquierda con el modo de Escalado activado\n");
	                	scale(1/KG_STEP_SCALE,1, 1);
	                }
	                if(modog==1) {
	                	printf("Usted acaba de pulsar Flecha Izquierda, ha reducido el volumen de visión en X\n");
                		camaraG->left=camaraG->left*0.9;
                		camaraG->right=camaraG->right*0.9;
	                }
	            }
	        }
	        else {//Rotacion especial modo analisis
        		printf("SIN IMPLEMENTAR\n");
        		rotate(KG_STEP_CAMERA_ANGLE,-camaraG->matrix->matrix[1],-camaraG->matrix->matrix[5],-camaraG->matrix->matrix[9]);
        	}
        	printf("modog:%d\n",modog);
        	if (moverluces==1){
        		traslate(-KG_STEP_MOVE,0,0);
        	}
	            camera_update();
        break;

        case GLUT_KEY_RIGHT:
        	if(modoV == 0){
                if(modo==1) {
                    printf("Usted acaba de pulsar Flecha Derecha con el modo de Traslacion activado\n");
                    traslate(KG_STEP_MOVE,0,0);
                }
                if(modo==2) {
                    printf("Usted acaba de pulsar Flecha Derecha con el modo de Rotacion activado\n");
                    rotate(KG_STEP_CAMERA_ANGLE, 0, KG_STEP_MOVE, 0);
                }
                if(modo==3) {
                	if(modog==0) {
                    	printf("Usted acaba de pulsar Flecha Derecha con el modo de Escalado activado\n");
                    	scale(KG_STEP_SCALE,1, 1);
                	}
                	if(modog==1) {
                		printf("Usted acaba de pulsar Felcha Derecha, ha aumentado el volumen de visión en X\n");
                		camaraG->right=camaraG->right*1.1;
                		camaraG->left=camaraG->left*1.1;
                	}
                }
              }
            else{
              	rotate(KG_STEP_CAMERA_ANGLE,camaraG->matrix->matrix[1],camaraG->matrix->matrix[5],camaraG->matrix->matrix[9]);
            }
            printf("modog:%d\n",modog);  
            if (moverluces==1){
        		traslate(KG_STEP_MOVE,0,0);
        	} 
            camera_update();
        break;

        case GLUT_KEY_PAGE_UP:
        	if(modoV == 0){//Modo vuelo
                if(modo==1) {
                    printf("Usted acaba de pulsar PagUp con el modo de Traslacion activado\n");
                    traslate(0, 0, KG_STEP_MOVE);
                }
                if(modo==2) {
                    printf("Usted acaba de pulsar PagUp con el modo de Rotacion activado\n");
                    rotate(KG_STEP_CAMERA_ANGLE, 0, 0, KG_STEP_MOVE);
                }
               	if(modo==3) {
               		if(modog==0){
                    	printf("Usted acaba de pulsar PagUp con el modo de Escalado activado\n");
                    	scale(1, 1, KG_STEP_SCALE);
                    }
                    if (modog==1){
                     	printf("Usted acaba de pulsar PagUp, ha aumentado la distancia de visión\n");
                		camaraG->near=camaraG->near*1.1;
                		camaraG->far=camaraG->far*1.1;
                     } 
                }
             }
            else {//Modo analisis
             	if(modo==2){
             		rotate(KG_STEP_CAMERA_ANGLE,camaraG->matrix->matrix[2],camaraG->matrix->matrix[6],camaraG->matrix->matrix[10]);
             	}
             	if(modo==1){
             		traslate(0,0,KG_STEP_MOVE);
             	}
            }
            printf("modog:%d\n",modog);
            if (moverluces==1){
        		traslate(0, 0, KG_STEP_MOVE);
        	}

                camera_update();
        break;

        case GLUT_KEY_PAGE_DOWN:
        	if(modoV == 0){
        		if(modo==1) {
                    printf("Usted acaba de pulsar PagDown con el modo de Traslacion activado\n");
                    traslate(0,0,-KG_STEP_MOVE);
                }
               	if(modo==2) {
                    printf("Usted acaba de pulsar PagDown con el modo de Rotacion activado\n");
                    rotate(KG_STEP_CAMERA_ANGLE, 0, 0, -KG_STEP_MOVE);
                }
                if(modo==3) {
                	if(modog==0) {
                    	printf("Usted acaba de pulsar PagDown con el modo de Escalado activado\n");
                    	scale(1, 1, 1/KG_STEP_SCALE);
                	}
                	if(modog==1) {
                		printf("Usted acaba de pulsar PagUp, ha reducido la distancia de visión\n");
                		camaraG->near=camaraG->near*0.9;
                		camaraG->far=camaraG->far*0.9;
                	}
                }
            }
            else {
            	if(modo==2){
             		rotate(KG_STEP_CAMERA_ANGLE,-camaraG->matrix->matrix[2],-camaraG->matrix->matrix[6],-camaraG->matrix->matrix[10]);
             	}
             	if(modo==1){
             		traslate(0,0,-KG_STEP_MOVE);
             	}
            }
            printf("modog:%d\n",modog);
            if (moverluces==1){
        		traslate(0,0,-KG_STEP_MOVE);
        	}
            camera_update();
        break;
    }  
    glutPostRedisplay();
}


/**
 * @brief Callback function to control the basic keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */
void keyboard(unsigned char key, int x, int y) {

    char* fname = malloc(sizeof (char)*128); /* Note that scanf adds a null character at the end of the vector*/
    int read = 0;
    object3d *auxiliar_object = 0;
    GLdouble wd,he,midx,midy;
    

    switch (key) {
    case 'f':
    case 'F':
        /*Ask for file*/
        printf("%s", KG_MSSG_SELECT_FILE);
        scanf("%s", fname);
        /*Allocate memory for the structure and read the file*/
        auxiliar_object = (object3d *) malloc(sizeof (object3d));
        read = read_wavefront(fname, auxiliar_object);
        switch (read) {
        /*Errors in the reading*/
        case 1:
            printf("%s: %s\n", fname, KG_MSSG_FILENOTFOUND);
            break;
        case 2:
            printf("%s: %s\n", fname, KG_MSSG_INVALIDFILE);
            break;
        case 3:
            printf("%s: %s\n", fname, KG_MSSG_EMPTYFILE);
            break;
        /*Read OK*/
        case 0:
            /*Insert the new object in the list*/
            auxiliar_object->next = _first_object;
            _first_object = auxiliar_object;
            _selected_object = _first_object;
            printf("%s\n",KG_MSSG_FILEREAD);
            break;
        }
        break;

    //-------------------------------------------------------------------------------------
    case 'o':
    case 'O':
        printf("Usted acaba de activar las transformaciones sobre los objetos\n");
        modog=0;
        moverluces=0;
        break;

    case 'g':
    case 'G':
    	if(modog==0) {
        	printf("Usted acaba de activar el sistema de referencia al del mundo\n");
        	modo2=1;
    	}
    	if (modog==1)
    		{
    			if (modoV==0)
    			{
    				printf("Usted acaba de ACTIVAR el modo de analisis, estamos mirando al objeto.\n");
	    			matrix_l *matrizaux;
	        		matrizaux = (matrix_l *) malloc(sizeof (matrix_l));
	    			glLoadIdentity();
	    			gluLookAt(camaraG->matrix->matrix[12],camaraG->matrix->matrix[13],camaraG->matrix->matrix[14],
	              	camaraG->matrix->matrix[12]-camaraG->matrix->matrix[8],camaraG->matrix->matrix[13]-camaraG->matrix->matrix[9],camaraG->matrix->matrix[14]-camaraG->matrix->matrix[10],
	                camaraG->matrix->matrix[4],camaraG->matrix->matrix[5],camaraG->matrix->matrix[6]);
					glGetDoublev(GL_MODELVIEW_MATRIX,matrizaux->matrix);//Matriz inversa de la camara
					glLoadIdentity();

					gluLookAt(matrizaux->matrix[12],matrizaux->matrix[13],matrizaux->matrix[14],
	              			_selected_object->matrix->matrix[12],_selected_object->matrix->matrix[13],_selected_object->matrix->matrix[14],
	                		_selected_object->matrix->matrix[4],_selected_object->matrix->matrix[5],_selected_object->matrix->matrix[6]);
					guardar_estado();
					representar_matriz();
	    			modoV=1;
    			}
    			else {
    				printf("Usted acaba de DESACTIVAR el modo de analisis, estamos mirando al objeto.\n");
    				modoV=0;
    			}

    		}	
        break;

    case '1':
    	printf("Usted ha seleccionado la luz 0, el sol.\n");
    	printf("Ahora solo podrá mover la luz seleccionada.\n");
    	luzseleccionada=&luces[0];
    	moverluces=1;
    	modog=2;
    break;
    case '2':
    	printf("Usted ha seleccionado la luz 1, la bombilla.\n");
    	printf("Ahora solo podrá mover la luz seleccionada.\n");
    	luzseleccionada=&luces[1];
    	moverluces=1;
    	modog=2;
    break;
    case '3':
    	printf("Usted ha seleccionado la luz 2, el foco del objeto.\n");
    	printf("Ahora solo podrá mover la luz seleccionada.\n");
    	luzseleccionada=&luces[2];
    	moverluces=1;
    	modog=2;
    break;
    case '4':
    	printf("Usted ha seleccionado la luz 3, el foco externo.\n");
    	printf("Ahora solo podrá mover la luz seleccionada.\n");
    	luzseleccionada=&luces[3];
    	moverluces=1;
    	modog=2;
    break;

    case 'l':
    case 'L':
    	if(modog==0) {
        	printf("Usted acaba de activar el sistema de referencia a el del objeto\n");
        	modo2=0;
    	}
    	if (modog==1)
    		{
    			printf("Usted acaba de activar el modo de vuelo.\n");
    			modoV=0;
    		}
        break;

    case 'b':
    case 'B':
        printf("Usted acaba de activar el modo de rotacion\n");
        modo=2;
        break;

    case 't':
    case 'T':
    	if(modog==0) {
        	printf("Usted acaba de activar el modo de escalado\n");
        	modo=3;
    	}
    	if (modog==1)
    		{
    			modo=3;
    			printf("Usted acaba de activar el cambio de volumen de visión.\n");	
    		}
        
        break;

    case 'm':
    case 'M':
        printf("Usted acaba de activar el modo de traslacion\n");
        modo=1;

        break;

    case 26: //Control z Tenemos que volver al estado anterior
        printf("Deshacer cambios\n");
        deshacer();
    break;

    case 'c'://Cambiamos de camara
    	if(camaraG==_selected_object->camara){
    		camaraG=camara_auxiliar;
    	}     
    	else{
    		camara_auxiliar=camaraG;
    		camaraG=camaraG->next;
    	}
        printf("Usted acaba de cambiar a la camara: %i\n",camaraG->numero);
    break;

    case 'C':
    	if(modog==1) {
    		if(modoprueba==0){
	    		modoprueba=1;
	        	printf("Usted acaba de cambiar a la camara del objeto\n");
        	}
        	else {
        		modoprueba=0;
        		printf("Usted acaba de desactivar la camara del objeto\n");
        	}
    	}
    	else {
    		//modoprueba=0;
    		printf("No esta activado el modo de camara\n");
    	}
    break;

    case 'k':
    case 'K':
        printf("Usted acaba de activar los cambios sobre la camara\n");
        modog=1;
        moverluces=0;
        break;

    case 'p':
    case 'P':
        if(modop==0){
            modop=1;
            printf("Usted acaba de cambiar a modo de Perspectiva.\n");
        }
        else {
            modop=0;
            printf("Usted acaba de cambiar a modo de Perspectiva Ortogonal.\n");
        }
        break;

    //-------------------------------------------------------------------------------------

    case 9: /* <TAB> */
        if(_selected_object!=NULL){
            _selected_object = _selected_object->next;
            /*The selection is circular, thus if we move out of the list we go back to the first element*/
            if (_selected_object == 0) _selected_object = _first_object;
        }
        else {
            printf("ERROR: No hay ningun objeto cargado!\n");
        }
        break;

    case 127: /* <SUPR> */
        if(_selected_object!=NULL){
            /*Erasing an object depends on whether it is the first one or not*/
            if (_selected_object == _first_object)
            {
                /*To remove the first object we just set the first as the current's next*/
                _first_object = _first_object->next;
                /*Once updated the pointer to the first object it is save to free the memory*/
                free(_selected_object);
                /*Finally, set the selected to the new first one*/
                _selected_object = _first_object;
            } else {
                /*In this case we need to get the previous element to the one we want to erase*/
                auxiliar_object = _first_object;
                while (auxiliar_object->next != _selected_object)
                    auxiliar_object = auxiliar_object->next;
                /*Now we bypass the element to erase*/
                auxiliar_object->next = _selected_object->next;
                /*free the memory*/
                free(_selected_object);
                /*and update the selection*/
                _selected_object = auxiliar_object;
            }
        }
        else {
            printf("ERROR: No hay ningun objeto cargado!\n");
        }
        break;

    case '-':
    	printf("Acaba de pulsar '-'\n");
    	if(iluminacion=!0 && luces[2]==1 && luzseleccionada==&luces[2]){
    		printf("Hemos reducido el cono del foco del objeto:%f\n",angulofoco*0.9);
    		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, angulofoco*0.9);
    		angulofoco=angulofoco*0.9;
    	}
    	else if(iluminacion=!0 && luces[3]==1 && luzseleccionada==&luces[3]){
    		printf("Hemos reducido el cono del foco exterior:%f\n",angulofoco2*0.9);
    		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, angulofoco2*0.9);
    		angulofoco2=angulofoco2*0.9;
    	}
    	else {
    		printf("No pasa nada\n");
    	}

        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
            /*Increase the projection plane; compute the new dimensions*/
            wd=(_ortho_x_max-_ortho_x_min)/KG_STEP_ZOOM;
            he=(_ortho_y_max-_ortho_y_min)/KG_STEP_ZOOM;
            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_ortho_x_max+_ortho_x_min)/2;
            midy = (_ortho_y_max+_ortho_y_min)/2;
            /*The the new limits are set, keeping the center of the plane*/
            _ortho_x_max = midx + wd/2;
            _ortho_x_min = midx - wd/2;
            _ortho_y_max = midy + he/2;
            _ortho_y_min = midy - he/2;
        }
        break;

    case '+':
    	printf("Acaba de pulsar '+'\n");
    	if(iluminacion=!0 && luces[2]==1 && luzseleccionada==&luces[2]){
    		printf("Hemos aumentado el cono del foco del objeto:%f\n",angulofoco*1.1);
    		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, angulofoco*1.1);
    		angulofoco=angulofoco*1.1;
    	}
    	else if(iluminacion=!0 && luces[3]==1 && luzseleccionada==&luces[3]){
    		printf("Hemos aumentado el cono del foco exterior:%f\n",angulofoco2*1.1);
    		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, angulofoco2*1.1);
    		angulofoco2=angulofoco2*1.1;
    	}
    	else {
    		printf("No pasa nada\n");
    	}

      	if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
	        /*Decrease the projection plane; compute the new dimensions*/
	        wd=(_ortho_x_max-_ortho_x_min)*KG_STEP_ZOOM;//Multiplicamos por el factor del zoom (0.75) para reducir asi los valores de x e y en un cuarto
	        he=(_ortho_y_max-_ortho_y_min)*KG_STEP_ZOOM;
	        /*In order to avoid moving the center of the plane, we get its coordinates*/
	        midx = (_ortho_x_max+_ortho_x_min)/2;
	        midy = (_ortho_y_max+_ortho_y_min)/2;
	        /*The the new limits are set, keeping the center of the plane*/
	        _ortho_x_max = midx + wd/2;
	        _ortho_x_min = midx - wd/2;
	        _ortho_y_max = midy + he/2;
	        _ortho_y_min = midy - he/2;
    	}
        break;

    case '?':
        print_help();
        break;

    case 27: /* <ESC> */
        exit(0);
        break;

    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
    }
    camera_update();
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();

}
