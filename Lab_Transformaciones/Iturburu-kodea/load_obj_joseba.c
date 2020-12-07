#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "definitions.h"
#include <math.h>


#define MAXLINE 200

/*
 * Auxiliar function to process each line of the file
 */
static int sreadint(char * lerroa, int * zenbakiak) {
    char *s = lerroa;
    int i, zbk, kont = 0;

    while (sscanf(s, " %d%n", &zbk, &i) > 0) {
        s += i;
        zenbakiak[kont++] = zbk;
    }
    return (kont);
}

static int sreadint2(char * lerroa, int * zenbakiak) {
    char *s = lerroa;
    int i, zbk, kont = 0;

    while (sscanf(s, " %d%n", &zbk, &i) > 0) {
        s += i;
	while ((*s != ' ')&&(*s !='\0')) s++;  // jump vector normal information
        zenbakiak[kont++] = zbk;
    }
printf("%d numbers in the line\n",kont);
    return (kont);
}
/**
 * @brief Function to read wavefront files (*.obj)
 * @param file_name Path of the file to be read
 * @param object_ptr Pointer of the object3d type structure where the data will be stored
 * @return Result of the reading: 0=Read ok, 1=File not found, 2=Invalid file, 3=Empty file
 */
int read_wavefront(char * file_name, object3d * object_ptr) {
    vertex *vertex_table;
    face *face_table;
    //-------------------------------------------------------------------------------------
    matrix_l *matrix;
    //-------------------------------------------------------------------------------------
    int num_vertices = -1, num_faces = -1, count_vertices = 0, count_faces = 0;
    FILE *obj_file;
    char line[MAXLINE], line_1[MAXLINE], aux[45];
    int k;
    int i, j;
    int values[MAXLINE];


    /*
     * The function reads twice the file. In the first read the number of
     * vertices and faces is obtained. Then, memory is allocated for each
     * of them and in the second read the actual information is read and
     * loaded. Finally, the object structure is created
     */
    if ((obj_file = fopen(file_name, "r")) == NULL) return (1);
    while (fscanf(obj_file, "\n%[^\n]", line) > 0) {
        i = 0;
        while (line[i] == ' ') i++;
        if ((line[0] == '#') && ((int) strlen(line) > 5)) {
            i += 2;
            j = 0;
            while (line[i] != ' ') line_1[j++] = line[i++];
            i++;
            line_1[j] = '\0';
            j = 0;
            while ((line[i] != ' ') && (line[i] != '\0'))
                aux[j++] = line[i++];
            aux[j] = 0;
            if (strcmp(aux, "vertices") == 0) {
                num_vertices = atoi(line_1);
            }
            if (strncmp(aux, "elements", 7) == 0) {
                num_faces = atoi(line_1);
            }
        } else {
            if (strlen(line) > 6) {
                if (line[i] == 'f' && line[i + 1] == ' ')
                    count_faces++;
                else if (line[i] == 'v' && line[i + 1] == ' ')
                    count_vertices++;
            }
        }
    }
    fclose(obj_file);
printf("1 pasada: num vert = %d (%d), num faces = %d(%d) \n",num_vertices,count_vertices,num_faces,count_faces);
    if ((num_vertices != -1 && num_vertices != count_vertices) || (num_faces != -1 && num_faces != count_faces)) {
        printf("WARNING: full file format: (%s)\n", file_name);
        //return (2);
    }
    if (num_vertices == 0 || count_vertices == 0) {
        printf("No vertex found: (%s)\n", file_name);
        return (3);
    }
    if (num_faces == 0 || count_faces == 0) {
        printf("No faces found: (%s)\n", file_name);
        return (3);
    }

    num_vertices = count_vertices;
    num_faces = count_faces;

    vertex_table = (vertex *) malloc(num_vertices * sizeof (vertex));
    face_table = (face *) malloc(num_faces * sizeof (face));
    //-------------------------------------------------------------------------------------
    matrix = (matrix_l *) malloc(sizeof (matrix_l)); //Asignamos un espacio de memoria para la lista de MATRICES
    //-------------------------------------------------------------------------------------

    obj_file = fopen(file_name, "r");
    k = 0;
    j = 0;

    for (i = 0; i < num_vertices; i++)
        vertex_table[i].num_faces = 0;

    while (fscanf(obj_file, "\n%[^\n]", line) > 0) {
        switch (line[0]) {
            case 'v':
            if (line[1] == ' ')  // vn not interested
		{
                sscanf(line + 2, "%lf%lf%lf", &(vertex_table[k].coord.x),
                        &(vertex_table[k].coord.y), &(vertex_table[k].coord.z));
                k++;
		}
               break;

            case 'f':
	    if (line[1] == ' ') // fn not interested
                {
                for (i = 2; i <= (int) strlen(line); i++)
                    line_1[i - 2] = line[i];
		line_1[i-2] = '\0';
                face_table[j].num_vertices = sreadint2(line_1, values);
                face_table[j].vertex_table = (int *) malloc(face_table[j].num_vertices * sizeof (int));
                for (i = 0; i < face_table[j].num_vertices; i++) {
                    face_table[j].vertex_table[i] = values[i] - 1;
                    vertex_table[face_table[j].vertex_table[i]].num_faces++;
                    }
                j++;
                }
              break;
        }
    }
    fclose(obj_file);

//-----------------------------------------------------------------------
    //Bucle que recorre todas las caras, para calcular los vectores normales
    for (int i = 0; i < num_faces; ++i)
    {
    	//Tres primeros puntos de la cara
    	int a[]={vertex_table[face_table[i].vertex_table[0]].coord.x, vertex_table[face_table[i].vertex_table[0]].coord.y, vertex_table[face_table[i].vertex_table[0]].coord.z};
    	int b[]={vertex_table[face_table[i].vertex_table[1]].coord.x, vertex_table[face_table[i].vertex_table[1]].coord.y, vertex_table[face_table[i].vertex_table[1]].coord.z};
    	int c[]={vertex_table[face_table[i].vertex_table[2]].coord.x, vertex_table[face_table[i].vertex_table[2]].coord.y, vertex_table[face_table[i].vertex_table[2]].coord.z};

    	//Vectores generados a partir de los 3 puntos obtenidos
    	double vector1[]={b[0]-a[0], b[1]-a[1], b[2]-a[2]};
    	double vector2[]={c[0]-a[0], c[1]-a[1], c[2]-a[2]};

    	//Vector normal a traves del producto vectorial de los dos vectores anteriores
    	double vnormal[]={(vector1[1]*vector2[2]) - (vector1[2]*vector2[1]), 
    			(vector1[2]*vector2[0]) - (vector1[0]*vector2[2]), 
    			(vector1[0]*vector2[1]) - (vector1[1]*vector2[0])};
    	//Calculo de la distancia del vector normal para poder normalizarlo
    	double distancia = sqrt(pow(vnormal[0],2)+pow(vnormal[1],2)+pow(vnormal[2],2));
    	vnormal[0]=vnormal[0]/distancia;
    	vnormal[1]=vnormal[1]/distancia;
    	vnormal[2]=vnormal[2]/distancia;
    	//Vector normalizado
		face_table[i].normalF.x=vnormal[0];
		face_table[i].normalF.y=vnormal[1];
		face_table[i].normalF.z=vnormal[2];

		//Sumamos el vector normal a los 3 puntos
		vertex_table[face_table[i].vertex_table[0]].normalV.x += face_table[i].normalF.x;
        vertex_table[face_table[i].vertex_table[0]].normalV.y += face_table[i].normalF.y;
        vertex_table[face_table[i].vertex_table[0]].normalV.z += face_table[i].normalF.z;

		vertex_table[face_table[i].vertex_table[1]].normalV.x += face_table[i].normalF.x;
        vertex_table[face_table[i].vertex_table[1]].normalV.y += face_table[i].normalF.y;
        vertex_table[face_table[i].vertex_table[1]].normalV.z += face_table[i].normalF.z;

        vertex_table[face_table[i].vertex_table[2]].normalV.x += face_table[i].normalF.x;
        vertex_table[face_table[i].vertex_table[2]].normalV.y += face_table[i].normalF.y;
        vertex_table[face_table[i].vertex_table[2]].normalV.z += face_table[i].normalF.z;

        //Recorremos todos los vertices de la cara y vamos sumando los vectores normales de la cara
    	for (int j = 3; j < face_table[i].num_vertices; ++j)
    	{
    		vertex_table[face_table[i].vertex_table[j]].normalV.x += face_table[i].normalF.x;
            vertex_table[face_table[i].vertex_table[j]].normalV.y += face_table[i].normalF.y;
            vertex_table[face_table[i].vertex_table[j]].normalV.z += face_table[i].normalF.z;
    	}
    }
    //Bucle para recorrer los vertices, vamos a normalizarlos
    for (int i = 0; i < num_faces; ++i)
    {
    	for (int j = 0; j < face_table[i].num_vertices; ++j)
    	{
    		//Normalizamos los vectores de cada vertice
    		//Distancia del vector normal para poder normalizarlo
    		double distancia = sqrt(pow(vertex_table[face_table[i].vertex_table[j]].normalV.x,2)+ 
					    			pow(vertex_table[face_table[i].vertex_table[j]].normalV.y,2)+ 
					    			pow(vertex_table[face_table[i].vertex_table[j]].normalV.z,2));

    		//Vector normal del vertice normalizado
	    	vertex_table[face_table[i].vertex_table[j]].normalV.x = vertex_table[face_table[i].vertex_table[j]].normalV.x/distancia;
	    	vertex_table[face_table[i].vertex_table[j]].normalV.y = vertex_table[face_table[i].vertex_table[j]].normalV.y/distancia;
	    	vertex_table[face_table[i].vertex_table[j]].normalV.z = vertex_table[face_table[i].vertex_table[j]].normalV.z/distancia;
    	}
    }

printf("2 pasada\n");

    /*
     * Information read is introduced in the structure */
    object_ptr->vertex_table = vertex_table;
    object_ptr->face_table = face_table;
    object_ptr->num_vertices = num_vertices;
    object_ptr->num_faces = num_faces;
    //-------------------------------------------------------------------------------------
    object_ptr->matrix = matrix;
    //-------------------------------------------------------------------------------------


    /*
     * The maximum and minimum coordinates are obtained **/
    object_ptr->max.x = object_ptr->vertex_table[0].coord.x;
    object_ptr->max.y = object_ptr->vertex_table[0].coord.y;
    object_ptr->max.z = object_ptr->vertex_table[0].coord.z;
    object_ptr->min.x = object_ptr->vertex_table[0].coord.x;
    object_ptr->min.y = object_ptr->vertex_table[0].coord.y;
    object_ptr->min.z = object_ptr->vertex_table[0].coord.z;

    for (i = 1; i < object_ptr->num_vertices; i++)
    {
        if (object_ptr->vertex_table[i].coord.x < object_ptr->min.x)
            object_ptr->min.x = object_ptr->vertex_table[i].coord.x;

        if (object_ptr->vertex_table[i].coord.y < object_ptr->min.y)
            object_ptr->min.y = object_ptr->vertex_table[i].coord.y;

        if (object_ptr->vertex_table[i].coord.z < object_ptr->min.z)
            object_ptr->min.z = object_ptr->vertex_table[i].coord.z;

        if (object_ptr->vertex_table[i].coord.x > object_ptr->max.x)
            object_ptr->max.x = object_ptr->vertex_table[i].coord.x;

        if (object_ptr->vertex_table[i].coord.y > object_ptr->max.y)
            object_ptr->max.y = object_ptr->vertex_table[i].coord.y;

        if (object_ptr->vertex_table[i].coord.z > object_ptr->max.z)
            object_ptr->max.z = object_ptr->vertex_table[i].coord.z;

    }
    //---------------------------------------------------------------------------------
    //Estamos cargando la pila de MATRICES inicial al cargar un objeto
    glMatrixMode(GL_MODELVIEW); //Para avisar de que vamos a usar la matriz modelview
    glLoadIdentity();
    glGetDoublev(GL_MODELVIEW_MATRIX,matrix->matrix);
    matrix->next=0;
    //---------------------------------------------------------------------------------    
    return (0);
}
