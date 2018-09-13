
//--------------------------------------------------------------
//       Autor del Programa: Konrad Peschka && Andreas Haller
//       Fecha: Septiembre 2017
//--------------------------------------------------------------

#include <Adafruit_NeoPixel.h>

#define PIN 10
#define NUMPIXELS 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#include <Servo.h>

//-------------------------------------------------------------------
//      Definicion Alias de cada servo y los Pines del Arduino
//-------------------------------------------------------------------

#define servo1_pin 8 // Pin Front Right Knee, Frente Derecha Cuerpo
#define servo2_pin 7 // Pin Back Right Knee, Atras Derecha Cuerpo
#define servo3_pin 3 // Pin Back Left Knee, Atras Izquierda Cuerpo
#define servo4_pin 4 // Pin Front Left Knee, Frente Izquierda Cuerpo

#define servo5_pin 9 // Pin Front Right Leg, Frente Derecha Pata
#define servo6_pin 6 // Pin Back Right Leg, Atras Derecha Pata
#define servo7_pin 2 // Pin Back Left Leg, Atras Izquierda Pata
#define servo8_pin 5 // Pin Front Left Leg, Frente Izquierda Pata

//-------------------------------------------------------------------
//  Definicion de las constantes posicion inicial de cada servo
//-------------------------------------------------------------------

//subir = giro antihor en cuerpo y patas

#define servo1_centro 1400 // Center Front Right Knee //Frente_derecha_cuerpo_centro
#define servo2_centro 1400 // Center Back Right Knee //Atras_derecha_cuerpo_centro
#define servo3_centro 1400 // Center Back Left Knee //Atras_Izquierda_cuerpo_centro
#define servo4_centro 1400 // Center Front Left Knee //Frente_Izquierda_cuerpo_centro

#define servo5_centro 2200 // Center Front Right Leg //Frente_derecha_pata_centro
#define servo6_centro 2200 // Center Back Right Leg //Atras_derecha_pata_centro
#define servo7_centro 2200 // Center Back Left Leg //Atras_Izquierda_pata_centro
#define servo8_centro 2200 // Center Front Left Leg //Frente_Izquierda_pata_centro

//-------------------------------------------------------------------

int raise_leg = 300;
int raise_leg2 = 800;

int leg_foreward = 800;

int left_movement;
int right_movement;

int aux = 0; //auxiliary to use for special functions //auxiliar que utilizo para las funciones especiales
int current_feeling = 1500;

int servo_wait_delay = 70;

int brightness = 50;

int auxiliar_saluda = 0;
int auxiliar_sienta = 0;

//-------------------------------------------------------------------
//       Defino como le va a llamar cada servo
//-------------------------------------------------------------------

Servo servo1; // Front Right Knee// Frente Derecha Cuerpo
Servo servo2; // Back Right Knee // Atras Derecha Cuerpo
Servo servo3; // Back Left Knee  // Atras Izquierda Cuerpo
Servo servo4; // Front Left Knee // Frente Izquierda Cuerpo

Servo servo5; // Front Right Leg // Frente Derecha Pata
Servo servo6; // Back Right Leg  // Atras Derecha Pata
Servo servo7; // Back Left Leg   // Atras Izquierda Pata
Servo servo8; // Front Left Leg  // Frente Izquierda Pata

//-------------------------------------------------------------------
//                 Seteo de Entradas, salidas y funciones
//                 Setting of Inputs, outputs and functions
//-------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);

  pixels.begin();

  //activa luces rojas
  for (int i = 0; i < NUMPIXELS; i++)
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(brightness, 0, 0)); // Moderately bright green color.
    pixels.show();                                       // This sends the updated pixel color to the hardware.
    delay(200);                                          // Delay for a period of time (in milliseconds).
  }

  //muestro blanco
  for (int i = 0; i < NUMPIXELS; i++)
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, brightness, brightness)); // Moderately bright green color.
    pixels.show();                                            // This sends the updated pixel color to the hardware.
    //delay(1); // Delay for a period of time (in milliseconds).
  }
}

//-------------------------------------------------------------------
//         Sector donde comienza la funcion Principal,
//        Donde se va a quedar el resto de la ejecucion
// Sector where the main function begins, where the rest of the execution will be
//-------------------------------------------------------------------

char auxColor;

void loop()
{
  char variable = 'X';
  if (Serial.available())
  {
    variable = Serial.read();
  }

  switch (variable)
  {
  case 'Y':
    deactivate_servos();
    delay(50);
    Serial.write(variable);
    break;
  case 'Z':
    activate_servos();
    delay(50);
    posicion_reposo();
    delay(50);
    Serial.write(variable);
    break;

  case 'F':
    walk_forward();
    Serial.write(variable);
    break;
  case 'B':
    walk_backward();
    Serial.write(variable);
    break;
  case 'L':
    turn_left();
    Serial.write(variable);
    break;
  case 'R':
    turn_right();
    Serial.write(variable);
    break;
  case 'S':
    auxiliar_saluda = 0;
    auxiliar_sienta = 0;
    posicion_reposo();
    Serial.write(variable);
    break;

  case 'I':
    if (auxiliar_sienta == 0)
    {
      sienta();
      auxiliar_sienta = 1;
    }
    Serial.write(variable);
    break;

  case 'O':
    if (auxiliar_saluda == 0)
    {
      sienta_para_saludar();
      auxiliar_saluda = 1;
    }
    move_leg_front();
    Serial.write(variable);
    break;

  case 'E':
    kick_left();
    Serial.write(variable);
    break;

  case 'A':
    kick_right();
    Serial.write(variable);
    break;
  case 'C':
    showColor(brightness, brightness, 0);
    delay(50);
    auxColor = Serial.read();
    if (auxColor == '0')
      showColor(0, 0, 0);
    else if (auxColor == '1')
      showColor(brightness, 0, 0);
    else if (auxColor == '2')
      showColor(brightness, 0, brightness);
    else if (auxColor == '3')
      showColor(0, 0, brightness);
    else if (auxColor == '4')
      showColor(0, brightness, brightness);
    else if (auxColor == '5')
      showColor(0, brightness, 0);
    else if (auxColor == '6')
      showColor(brightness, brightness, 0);
    else if (auxColor == '7')
      showColor(brightness, brightness, brightness);

    Serial.write(variable);
    //activate_servos();
    break;
  case 'V':
    servo_wait_delay = map(Serial.parseInt(), 0, 4, 100, 30);
    Serial.write(variable);
    break;
  case 'b':
    brightness = map(Serial.parseInt(), 0, 9, 0, 50);
    showColor(brightness, brightness, brightness);
    Serial.write(variable);
    break;
  }
}

/*  --------  Activate Servos ---------   */

void activate_servos(void)
{
  servo1.attach(servo1_pin);
  servo1.writeMicroseconds(servo1_centro);

  servo2.attach(servo2_pin);
  servo2.writeMicroseconds(servo2_centro);

  servo3.attach(servo3_pin);
  servo3.writeMicroseconds(servo3_centro);

  servo4.attach(servo4_pin);
  servo4.writeMicroseconds(servo4_centro);

  servo5.attach(servo5_pin);
  servo5.writeMicroseconds(servo5_centro);

  servo6.attach(servo6_pin);
  servo6.writeMicroseconds(servo6_centro);

  servo7.attach(servo7_pin);
  servo7.writeMicroseconds(servo7_centro);

  servo8.attach(servo8_pin);
  servo8.writeMicroseconds(servo8_centro);

  delay(servo_wait_delay);
}

/*  --------  Deactivate Servos ---------   */

void deactivate_servos(void)
{
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();
  servo5.detach();
  servo6.detach();
  servo7.detach();
  servo8.detach();
  delay(servo_wait_delay);
}
//------------------------Funcion donde todos los servos van a reposo---------------
// Function where all the servos go to rest
void posicion_reposo(void)
{
  servo1.writeMicroseconds(servo1_centro);
  delay(servo_wait_delay);
  servo2.writeMicroseconds(servo2_centro);
  delay(servo_wait_delay);
  servo3.writeMicroseconds(servo3_centro);
  delay(servo_wait_delay);
  servo4.writeMicroseconds(servo4_centro);
  delay(servo_wait_delay);
  servo5.writeMicroseconds(servo5_centro);
  delay(servo_wait_delay);
  servo6.writeMicroseconds(servo6_centro);
  delay(servo_wait_delay);
  servo7.writeMicroseconds(servo7_centro);
  delay(servo_wait_delay);
  servo8.writeMicroseconds(servo8_centro);
  delay(servo_wait_delay);
}

//--------------------Movimiento Base--------------------
// Base Movement
void base_movement(void)
{
  // ----------------sube las patas 8 y 6 ------------------
  servo6.writeMicroseconds(servo6_centro - raise_leg);
  delay(servo_wait_delay);
  servo8.writeMicroseconds(servo8_centro - raise_leg);
  delay(servo_wait_delay);

  // -------------avanza todas las patas-------------------
  servo4.writeMicroseconds(servo4_centro + left_movement);
  servo2.writeMicroseconds(servo2_centro - right_movement);
  servo1.writeMicroseconds(servo1_centro + right_movement);
  servo3.writeMicroseconds(servo3_centro - left_movement);
  delay(servo_wait_delay);

  // ----------------baja las patas 8 y 6 ------------------
  servo6.writeMicroseconds(servo6_centro);
  delay(servo_wait_delay);
  servo8.writeMicroseconds(servo8_centro);
  delay(servo_wait_delay);

  //----------levanta las patas 5 y 7-----------------
  servo7.writeMicroseconds(servo7_centro - raise_leg);
  delay(servo_wait_delay);
  servo5.writeMicroseconds(servo5_centro - raise_leg);
  delay(servo_wait_delay);

  // -------------avanza todas las patas-------------------
  servo1.writeMicroseconds(servo1_centro - right_movement);
  servo3.writeMicroseconds(servo3_centro + left_movement);
  servo4.writeMicroseconds(servo4_centro - left_movement);
  servo2.writeMicroseconds(servo2_centro + right_movement);
  delay(servo_wait_delay);

  // ----------------baja las patas 5 y 7 ------------------
  servo7.writeMicroseconds(servo7_centro);
  delay(servo_wait_delay);
  servo5.writeMicroseconds(servo5_centro);
  delay(servo_wait_delay);
}

// -----------------------------------Funcion Avanzar hacia el frente-----------------------
// Function Advance towards the front
void walk_forward()
{
  left_movement = 300;
  right_movement = 300;
  base_movement();
}

// -----------------------------------Funcion Gira Derecha------------------------------------
// Right Turn Function
void turn_right()
{
  left_movement = -300;
  right_movement = 300;
  base_movement();
}

// -----------------------------------Funcion Gira Izquierda----------------------------------
// Left Turn Function
void turn_left()
{
  left_movement = 300;
  right_movement = -300;
  base_movement();
}

// -----------------------------------Funcion Camina hacia Atras-------------------------------
// Function Walk Backwards
void walk_backward()
{
  left_movement = -300;
  right_movement = -300;
  base_movement();
}

//--------------------funcion con al cual se sienta--------------------
//function with which you feel
void sienta(void)
{
  servo1.writeMicroseconds(servo1_centro);
  delay(servo_wait_delay);
  servo4.writeMicroseconds(servo4_centro);
  delay(servo_wait_delay);

  servo2.writeMicroseconds(servo2_centro - current_feeling / 3); //se tira de a poco
  delay(servo_wait_delay);
  servo3.writeMicroseconds(servo3_centro + current_feeling / 3);
  delay(servo_wait_delay);
  servo6.writeMicroseconds(servo6_centro - current_feeling / 3);
  delay(servo_wait_delay);
  servo7.writeMicroseconds(servo7_centro - current_feeling / 3);
  delay(servo_wait_delay);

  servo2.writeMicroseconds(servo2_centro - current_feeling / 2); //se tira de a poco
  delay(servo_wait_delay);
  servo3.writeMicroseconds(servo3_centro + current_feeling / 2);
  delay(servo_wait_delay);
  servo6.writeMicroseconds(servo6_centro - current_feeling / 2);
  delay(servo_wait_delay);
  servo7.writeMicroseconds(servo7_centro - current_feeling / 2);
  delay(servo_wait_delay);

  servo2.writeMicroseconds(servo2_centro - current_feeling / 2); //se termina de tirar
  delay(servo_wait_delay);
  servo3.writeMicroseconds(servo3_centro + current_feeling / 2);
  delay(servo_wait_delay);
}

//-------------------se tira para mover las patas y saludar---------------------
//he pulls to move his legs and say hello

void sienta_para_saludar(void)
{
  servo1.writeMicroseconds(servo1_centro);
  delay(servo_wait_delay / 2);
  servo4.writeMicroseconds(servo4_centro);
  delay(servo_wait_delay / 2);

  servo2.writeMicroseconds(servo2_centro - current_feeling / 2); //se tira al piso de costado
  delay(servo_wait_delay / 2);
  // servo3.writeMicroseconds(servo3_centro - current_feeling / 2);
  // delay(servo_wait_delay / 2);

  servo5.writeMicroseconds(servo5_centro - current_feeling);
  delay(servo_wait_delay / 2);
  servo7.writeMicroseconds(servo7_centro - current_feeling / 3);
  delay(servo_wait_delay / 2);
  servo7.writeMicroseconds(servo7_centro - current_feeling / 2);
  delay(servo_wait_delay / 2);
  servo7.writeMicroseconds(servo7_centro - current_feeling);
  delay(servo_wait_delay / 2);
  servo7.writeMicroseconds(servo6_centro - current_feeling);
  delay(servo_wait_delay / 2);
}

//--------------Funcion Mueve Pata en Frente cuando saluda---------------
// Function Moves Paw in Front when it greets

void move_leg_front(void)
{
  int maximo = 500;
  for (int j = 0; j < maximo; j = j + 100)
  {
    servo1.writeMicroseconds(servo1_centro + j); //se tira al piso de costado
    delay(servo_wait_delay);
  }

  for (int j = maximo; j > 0; j = j - 100)
  {
    servo1.writeMicroseconds(servo1_centro + j); //se tira al piso de costado
    delay(servo_wait_delay);
  }

  for (int j = 0; j < maximo; j = j + 100)
  {
    servo1.writeMicroseconds(servo1_centro - j); //se tira al piso de costado
    delay(servo_wait_delay);
  }

  for (int j = maximo; j > 0; j = j - 100)
  {
    servo1.writeMicroseconds(servo1_centro - j); //se tira al piso de costado
    delay(servo_wait_delay);
  }
}

//------------------Funcion donde patea del lazo izquierdo-----------------
// Function where you kick from the right loop
void kick_right(void)
{
  servo1.writeMicroseconds(servo1_centro - leg_foreward);
  delay(servo_wait_delay * 4);
  servo7.writeMicroseconds(servo7_centro - raise_leg);
  delay(servo_wait_delay * 6);
  servo5.writeMicroseconds(servo5_centro - raise_leg2);
  delay(servo_wait_delay * 6);
  servo5.writeMicroseconds(servo5_centro);
  delay(servo_wait_delay);
  servo1.writeMicroseconds(servo1_centro);
  delay(servo_wait_delay);
}
//------------------Funcion donde patea del lazo izquierdo-----------------
// Function where you kick from the left loop
void kick_left(void)
{
  servo4.writeMicroseconds(servo4_centro + leg_foreward);
  delay(servo_wait_delay * 4);
  servo6.writeMicroseconds(servo6_centro - raise_leg);
  delay(servo_wait_delay * 6);
  servo8.writeMicroseconds(servo8_centro - raise_leg2);
  delay(servo_wait_delay * 6);
  servo8.writeMicroseconds(servo8_centro);
  delay(servo_wait_delay);
  servo4.writeMicroseconds(servo4_centro);
  delay(servo_wait_delay);
}

void showColor(int r, int g, int b)
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(r, g, b)); // Moderately bright green color.
    pixels.show();                                  // This sends the updated pixel color to the hardware.
    //delay(200); // Delay for a period of time (in milliseconds).
  }
}

//
//-------------------------------------------------------------------
//               FIN PROGRAMA
//-------------------------------------------------------------------
