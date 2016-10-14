/*
 MIT License
Copyright (c) [2016] [Radoslav Mandev with the help of Orlin Dimitrov]
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial SerialPortions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// This is a program to control stepper motor with H-bridge driver and play the Super Mario theme song

//http://wiki.mikrotik.com/wiki/Super_Mario_Theme
//delay(1) == delayMicroseconds(1000);
//f=1/T
//T=1/f
//T=1/660 
//~0,002s

// The coil pins.
#define COIL_1A 5
#define COIL_1B 4
#define COIL_2A 3
#define COIL_2B 2

// Total notes count.
#define NOTES_COUNT 156
// The duration(delay) and frequency numbers in the array.
#define NOTE_SIZE 2

// The steps per revolution of the motor.
#define STEPS_PER_REV 30

#include <Stepper.h>

// Define the structure
typedef struct sNote 
{
  int frequency;
  int duration;
};

// Array of the frequencies and times.
sNote Melody[] = {
{ 660, 150 }, { 660, 300 }, { 660, 300 }, { 510, 100 }, { 660, 300 },
{ 150, 660 }, { 300, 660 }, { 300, 510 }, { 100, 660 }, { 300, 770 },
{ 660, 300 }, { 660, 300 }, { 510, 100 }, { 660, 300 }, { 770, 550 },
{ 300, 660 }, { 300, 510 }, { 100, 660 }, { 300, 770 }, { 550, 380 },
{ 660, 300 }, { 510, 100 }, { 660, 300 }, { 770, 550 }, { 380, 575 },
{ 300, 510 }, { 100, 660 }, { 300, 770 }, { 550, 380 }, { 575, 510 },
{ 510, 100 }, { 660, 300 }, { 770, 550 }, { 380, 575 }, { 510, 450 },
{ 100, 660 }, { 300, 770 }, { 550, 380 }, { 575, 510 }, { 450, 380 },
{ 660, 300 }, { 770, 550 }, { 380, 575 }, { 510, 450 }, { 380, 400 },
{ 300, 770 }, { 550, 380 }, { 575, 510 }, { 450, 380 }, { 400, 320 },
{ 770, 550 }, { 380, 575 }, { 510, 450 }, { 380, 400 }, { 320, 500 },
{ 550, 380 }, { 575, 510 }, { 450, 380 }, { 400, 320 }, { 500, 440 },
{ 380, 575 }, { 510, 450 }, { 380, 400 }, { 320, 500 }, { 440, 300 },
{ 575, 510 }, { 450, 380 }, { 400, 320 }, { 500, 440 }, { 300, 480 },
{ 510, 450 }, { 380, 400 }, { 320, 500 }, { 440, 300 }, { 480, 330 },
{ 450, 380 }, { 400, 320 }, { 500, 440 }, { 300, 480 }, { 330, 450 },
{ 380, 400 }, { 320, 500 }, { 440, 300 }, { 480, 330 }, { 450, 150 },
{ 400, 320 }, { 500, 440 }, { 300, 480 }, { 330, 450 }, { 150, 430 },
{ 320, 500 }, { 440, 300 }, { 480, 330 }, { 450, 150 }, { 430, 300 },
{ 500, 440 }, { 300, 480 }, { 330, 450 }, { 150, 430 }, { 300, 380 },
{ 440, 300 }, { 480, 330 }, { 450, 150 }, { 430, 300 }, { 380, 200 },
{ 300, 480 }, { 330, 450 }, { 150, 430 }, { 300, 380 }, { 200, 660 },
{ 480, 330 }, { 450, 150 }, { 430, 300 }, { 380, 200 }, { 660, 200 },
{ 330, 450 }, { 150, 430 }, { 300, 380 }, { 200, 660 }, { 200, 760 },
{ 450, 150 }, { 430, 300 }, { 380, 200 }, { 660, 200 }, { 760, 150 },
{ 150, 430 }, { 300, 380 }, { 200, 660 }, { 200, 760 }, { 150, 860 },
{ 430, 300 }, { 380, 200 }, { 660, 200 }, { 760, 150 }, { 860, 300 },
{ 300, 380 }, { 200, 660 }, { 200, 760 }, { 150, 860 }, { 300, 700 },
{ 380, 200 }, { 660, 200 }, { 760, 150 }, { 860, 300 }, { 700, 150 },
{ 200, 660 }, { 200, 760 }, { 150, 860 }, { 300, 700 }, { 150, 760 },
{ 660, 200 }, { 760, 150 }, { 860, 300 }, { 700, 150 }, { 760, 350 },
{ 200, 760 }, { 150, 860 }, { 300, 700 }, { 150, 760 }, { 350, 660 },
{ 760, 150 }, { 860, 300 }, { 700, 150 }, { 760, 350 }, { 660, 300 },
{ 150, 860 }, { 300, 700 }, { 150, 760 }, { 350, 660 }, { 300, 520 },
{ 860, 300 }, { 700, 150 }, { 760, 350 }, { 660, 300 }, { 520, 150 },
{ 300, 700 }, { 150, 760 }, { 350, 660 }, { 300, 520 }, { 150, 580 },
{ 700, 150 }, { 760, 350 }, { 660, 300 }, { 520, 150 }, { 580, 150 },
{ 150, 760 }, { 350, 660 }, { 300, 520 }, { 150, 580 }, { 150, 480 },
{ 760, 350 }, { 660, 300 }, { 520, 150 }, { 580, 150 }, { 480, 500 },
{ 350, 660 }, { 300, 520 }, { 150, 580 }, { 150, 480 }, { 500, 510 },
{ 660, 300 }, { 520, 150 }, { 580, 150 }, { 480, 500 }, { 510, 450 },
{ 300, 520 }, { 150, 580 }, { 150, 480 }, { 500, 510 }, { 450, 380 },
{ 520, 150 }, { 580, 150 }, { 480, 500 }, { 510, 450 }, { 380, 400 },
{ 150, 580 }, { 150, 480 }, { 500, 510 }, { 450, 380 }, { 400, 320 },
{ 580, 150 }, { 480, 500 }, { 510, 450 }, { 380, 400 }, { 320, 500 },
{ 150, 480 }, { 500, 510 }, { 450, 380 }, { 400, 320 }, { 500, 440 },
{ 480, 500 }, { 510, 450 }, { 380, 400 }, { 320, 500 }, { 440, 300 },
{ 500, 510 }, { 450, 380 }, { 400, 320 }, { 500, 440 }, { 300, 480 },
{ 510, 450 }, { 380, 400 }, { 320, 500 }, { 440, 300 }, { 480, 330 },
{ 450, 380 }, { 400, 320 }, { 500, 440 }, { 300, 480 }, { 330, 450 },
{ 380, 400 }, { 320, 500 }, { 440, 300 }, { 480, 330 }, { 450, 150 },
{ 400, 320 }, { 500, 440 }, { 300, 480 }, { 330, 450 }, { 150, 430 },
{ 320, 500 }, { 440, 300 }, { 480, 330 }, { 450, 150 }, { 430, 300 },
{ 500, 440 }, { 300, 480 }, { 330, 450 }, { 150, 430 }, { 300, 380 },
{ 440, 300 }, { 480, 330 }, { 450, 150 }, { 430, 300 }, { 380, 200 },
{ 300, 480 }, { 330, 450 }, { 150, 430 }, { 300, 380 }, { 200, 660 },
{ 480, 330 }, { 450, 150 }, { 430, 300 }, { 380, 200 }, { 660, 200 },
{ 330, 450 }, { 150, 430 }, { 300, 380 }, { 200, 660 }, { 200, 760 },
{ 450, 150 }, { 430, 300 }, { 380, 200 }, { 660, 200 }, { 760, 150 },
{ 150, 430 }, { 300, 380 }, { 200, 660 }, { 200, 760 }, { 150, 860 },
{ 430, 300 }, { 380, 200 }, { 660, 200 }, { 760, 150 }, { 860, 300 },
{ 300, 380 }, { 200, 660 }, { 200, 760 }, { 150, 860 }, { 300, 700 }
};

// The instance of the motor driver.
Stepper MelodyMotor(STEPS_PER_REV, COIL_1A, COIL_1B, COIL_2A, COIL_2B);


// Setup the pheriferals.
void setup()
{
}

// Main loop of the program.
void loop()
{
  PlayMelodie();
}

// Plays the melody in "Melody" array.
void PlayMelodie()
{
  for (int noteIndex = 0; noteIndex < NOTES_COUNT; noteIndex += NOTE_SIZE)
  {
    MotorTone(Melody[noteIndex].frequency);
    delay(Melody[noteIndex].duration);
  }
} 
// Generate tone via motor.
void MotorTone(double speedTone)
{
  // Transpose scale value.
  speedTone *= 1.5;
  MelodyMotor.setSpeed(speedTone);
  MelodyMotor.step(STEPS_PER_REV);
}

