#ifndef IO_H
#define IO_H

void guardar_estado();
void traslate(float a, float b, float c);
void rotate(float angle,float a, float b, float c);
void scale(float a, float b, float c);
void special_keyboard(unsigned char key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void print_help();
void representar_matriz();

#endif // IO_H
