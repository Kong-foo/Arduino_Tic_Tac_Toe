
#include <time.h>
#define FRONTBACK 8
#define BASE A0
#define UPDOWN A2
#define CLAW 9

#define JOYSTICK 7

#define X 1
#define O 2
#define PLAYER1 -1 //cpu
#define PLAYER2 1 //user

int corners[4] = {0, 2, 6, 8};
int moves[9];
int movecount;
int g;

int getplayermove();
int rowmoves[8][3] = {{0,4,8}, {2, 4, 6}, {0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}};

int positions[9][3] = {
{170,178,30},
{145,178,30},
{130,178,30},
{170,150,7},
{142,150,7},
{125,150,7},
{170,120,-10},
{145,120,-10},
{117,120,-10}
};

int twoinrow() {

int *array1;
int count = 0;
int preset = -3;
int pos = 0;
int inc=0,i = 0;
g=0;


while (g < 7) {
array1 = rowmoves[g];
count = 0;
//printf("%d %d %d\t", array[0], array[1], array[2]);
//printf("%d %d %d\n", moves[array[0]], moves[array[1]], moves[array[2]]);
for (int inc = 0; inc<3; inc++) {
i = array1[inc];
pos += moves[i]*(inc+1);
count += moves[i];
if (inc == 2) {
//return untaken spot if 2 in row
if (count == -2) for (int p = 0; p <=2; p++) if (moves[array1[p]] == 0) return array1[p];

if (count == 2) for (int p = 0; p <=2; p++) if (moves[array1[p]] == 0) preset = array1[p];
}

}
g++;
}
return preset;
}


int won() {
g = 0;
int *array;
while (g < 7) {
array = rowmoves[g];
//printf("%d %d %d ", array[0], array[1], array[2]);
//printf("\t%d %d %d\n", moves[array[0]], moves[array[1]], moves[array[2]]);

if (moves[array[0]] != 0 && moves[array[0]] == moves[array[1]] && moves[array[1]] == moves[array[2]])
return moves[array[0]];
g++;
}
return 0;
}


int decideonmove() {
int tmp;
if (movecount == 0) {
return corners[rand() % 4];
}

if (movecount == 1) {
if (moves[0] == PLAYER1) return 8;
if (moves[2] == PLAYER1) return 6;
if (moves[6] == PLAYER1) return 2;
if (moves[8] == PLAYER1) return 0;
return tmp;
}

if (movecount >= 2) {
tmp = won();
if (tmp != 0) return tmp;
tmp = twoinrow();
if (tmp >= 0) return tmp;

}
//just fill an empty
for (int i = 0; i < 9; i++) if (moves[i] == 0) return i;

return -3;
}


int getplayermove() {
  int currentX, currentY, nextX, nextY, moveX, moveY;
int realpos = 4;
while (digitalRead(6) != 1) {
  moveX = 0;
  moveY = 0;
currentX = analogRead(A4);
  if (currentX > 725) moveX = -2;
  else if (currentX < 10) moveX = 2;
  currentY = analogRead(A3);
  if (currentY > 725) moveY = -2;
  else if (currentY < 10) moveY = 2;
  delay(40);
 nextX = analogRead(A4);
  if (nextX > 725 && moveX == -2) moveX = -1;
  else if (nextX < 10 && moveX == 2) moveX = 1;
  
     nextY = analogRead(A3);
  if (nextY > 725 && moveY == -2) moveY = -1;
  else if (nextY < 10 && moveY == 2) moveY = 1;

if (realpos != 2 && realpos != 5 && realpos != 8 && moveX == 1) realpos ++;
else if (realpos != 0 && realpos != 3 && realpos != 6 && moveX == -1) realpos --;
else if (realpos < 6 && moveY == 1) realpos += 3;
else if (realpos > 2 &&  moveY == -1) realpos -= 3;
    moveangle(BASE, positions[realpos][0]);
    delay(200);
      moveangle(FRONTBACK, positions[realpos][1]-15);
      delay(100);
        moveangle(UPDOWN, positions[realpos][2]+15);
delay(200);
}
Serial.print("hi");
  return realpos;
}


int n;


void moveangle(int pin, int angle) {
int pulsewidth = (angle * 11) + 500;
for (int i = 0; i < 5; i++) {
digitalWrite(pin, HIGH);
delayMicroseconds(pulsewidth);
digitalWrite(pin, LOW);
delay(20 - pulsewidth/1000);
}


}

void movetobase() {
  moveangle(UPDOWN, 30);
  delay(300);
  moveangle(BASE, 70);
  moveangle(FRONTBACK, 90);
  delay(500);
  moveangle(FRONTBACK, 140);
  moveangle(CLAW, 120);
}


void liftfree(int XorO) {
  if (XorO == O) {
  moveangle(CLAW, 123);
  delay(500);
  moveangle(BASE, 120);
  }
  else if (XorO == X) {
  moveangle(CLAW, 123);
  delay(500);
  moveangle(BASE, 100);
  
  }
  delay(500);
  moveangle(FRONTBACK, 185);
  delay(500);
    moveangle(UPDOWN, 20);
  delay(500);
  moveangle(CLAW, 96);
  delay(500);
  moveangle(FRONTBACK, 130);
  delay(500);
}


// Positions[number][2] increase from 0 to 6 from 150 to 180
//positions[number][1] decreases from 0-6



//degree = arccos( sqrt( (d^2) / 2 ) - bCONST )

void liftup(int number) {
    moveangle(BASE, positions[number][0]);
//    d = sqrt(2*pow(bCONST+cos(90), 2));
  delay(500);
  moveangle(CLAW, 130);
  delay(400);
    moveangle(UPDOWN, positions[number][2]);
  delay(400);
    moveangle(FRONTBACK, positions[number][1]);
  delay(400);
    moveangle(CLAW, 96);
  delay(400);
  moveangle(FRONTBACK, 130);
delay(200);
}

void dropon(int number) {
  
moveangle(BASE, positions[number][0]);
  delay(500);
      moveangle(UPDOWN, positions[number][2]);
delay(500);
  moveangle(FRONTBACK, positions[number][1]);
  delay(500);
  moveangle(CLAW, 130);
delay(500);
moveangle(FRONTBACK, 130);
delay(500);
}


void setup() {
  
srand(time(NULL));

for (int i = 0; i <= 8; i++) moves[i] = 0;
movecount = 0;


pinMode(BASE, OUTPUT);
pinMode(FRONTBACK, OUTPUT);
pinMode(UPDOWN, OUTPUT);
pinMode(CLAW, OUTPUT);
pinMode(7, INPUT);
Serial.begin(9600);





//movetobase();
delay(500);
n = 6;
}

//FRONTBACK goes from 70-160
//UPDOWN goes from 150-185
//BASE FROM 0-180
void loop() {

int cpumove;
int playermove = 0;

cpumove = decideonmove();
if (cpumove < 0) {
if (cpumove == PLAYER1) {
Serial.print("PLAYER1 (cpu) won.\n"); //player 1 won
return;
}
else if (cpumove == PLAYER2) { Serial.print("PLAYER2 (player) won.\n"); return;} //player 2 won
}
else if (cpumove >= 0) {
moves[cpumove] = PLAYER1;
delay(500);
Serial.println("LIFTING FOR CPU");
liftfree(X);
delay(500);
dropon(cpumove);
}

Serial.print(cpumove);

playermove = getplayermove();
liftfree(O);
dropon(playermove);
Serial.print(", ");
Serial.print(playermove);
Serial.println("");

moves[playermove] = PLAYER2;
movecount++;
for (int i = 0; i < 9; i+=3) {
  Serial.print(moves[i]);
  Serial.print("|");
  Serial.print(moves[i+1]);
    Serial.print("|");
  Serial.print(moves[i+2]);
  Serial.println("");
}
delay(500);
Serial.println("\n");


  /*
  static int prevX = 0, prevY=0;
  int currentX, currentY;
  static int fbdeg = 70;
  static int uddeg = 70;
  delay(100);
  currentX = analogRead(A4);
  currentY = analogRead(A3);

if (currentX > 374 || currentX < 354) {
  if (currentX > prevX) {delay(10); if (analogRead(A4) > prevX) fbdeg +=10;}
  else if (currentX < prevX) {delay(10); if (analogRead(A4) < prevX) fbdeg -=10;}
  moveangle(FRONTBACK, fbdeg); delay(500);

}
if (currentY > 379 || currentY < 359) {
  if (currentY > prevY) {delay(10); if (analogRead(A3) > prevY) uddeg +=10;} 
  else if (currentY < prevY) {delay(10); if (analogRead(A3) < prevY) uddeg -=10;} 
  moveangle(UPDOWN, uddeg); delay(500);

}


Serial.print(fbdeg);
Serial.print(", ");
Serial.println(uddeg);
prevX = currentX;
prevY = currentY;
delay(100);
*/
/*
delay(500);

delay(1500);
dropon(n);
delay(500);

n++;
if (n == 9) n = 0;
*/

}
