#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

#define screen_width 128
#define screen_hight 64

const int buttonpin[]={13,12,14,27};
int output[4][4];
int l=127;
int w=63;
int g=14;
int x=l-4*g-3;
int y=w-4*g-3;
int c=1;
int score=0;

bool isfull=0;
bool game_over=0;
bool win_game=0;

String words1="Match Over";
String words2="Won The Game";

Adafruit_SSD1306 display(screen_width,screen_hight, &Wire,-1);
void print();
void addRandomTile();
void moveDown();
void moveUp();
void moveRight();
void moveLeft();
void Display();
void check_over();

//bitmaps for all tiles

const uint8_t num2[] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00, 0x07, 0x00,
 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t num4[] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x07, 0x00,
 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t num8[] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x05, 0x00, 0x07, 0x00,
 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t num16[] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xC0, 0x19, 0x00, 0x09, 0xC0,
 0x09, 0x40, 0x1D, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t num32[] = {
 0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x1D, 0xC0,0x04, 0x40,0x1D, 0xC0,
 0x05, 0x00,0x1D, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t num64[] = {
 0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x1D, 0x40,0x11, 0x40,0x1D, 0xC0,
 0x14, 0x40,0x1C, 0x40,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,
};
const uint8_t num128[] = {
 0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x13, 0xB8,0x30, 0xA8,0x13, 0xB8,
 0x12, 0x28,0x3B, 0xB8,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,
};
const uint8_t num256[] = {
 0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x3B, 0xB8,0x0A, 0x20,0x3B, 0xB8,
 0x20, 0xA8,0x3B, 0xB8,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,
};
const uint8_t num512[] = {
 0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x39, 0x38,0x23, 0x08,0x39, 0x38,
 0x09, 0x20,0x3B, 0xB8,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,
};
const uint8_t num1024[] = {
 0x00, 0x00,0x08, 0xE0,0x18, 0xA0,0x08, 0xA0,0x08, 0xA0,0x1C, 0xE0,0x00, 0x00,
 0x00, 0x00,0x1C, 0xA0,0x04, 0xA0,0x1C, 0xE0,0x10, 0x20,0x1C, 0x20,0x00, 0x00,
};
const uint8_t num2048[] = {
 0x00, 0x00,0x1C, 0xE0,0x04, 0xA0,0x1C, 0xA0,0x10, 0xA0,0x1C, 0xE0,0x00, 0x00,
 0x00, 0x00,0x14, 0xE0,0x14, 0xA0,0x1C, 0xE0,0x04, 0xA0,0x04, 0xE0,0x00, 0x00,
};
const uint8_t num4096[] = {
 0x00, 0x00,0x14, 0xE0,0x14, 0xA0,0x1C, 0xA0,0x04, 0xA0,0x04, 0xE0,0x00, 0x00,
 0x00, 0x00,0x1C, 0xE0,0x14, 0x80,0x1C, 0xE0,0x04, 0xA0,0x1C, 0xE0,0x00, 0x00,
};
const uint8_t num8192[] = {
 0x00, 0x00 ,0x1C, 0x40 ,0x14, 0xC0,0x1C, 0x40,0x14, 0x40,0x1C, 0xE0,0x00, 0x00,
 0x00, 0x00,0x1C, 0xE0,0x14, 0x20,0x1C, 0xE0,0x04, 0x80,0x1C, 0xE0,0x00, 0x00,
};
const uint8_t* num[]={num2,num4,num8,num16,num32,num64,num128,num256,num512,num1024,num2048,num4096,num8192};

void setup(){
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.print(F("failed"));
    for (;;);
  }
  for (int i=0;i<4;i++){
    pinMode(buttonpin[i],INPUT_PULLUP);
  }
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      output[i][j]=0;
    }
  }
  display.clearDisplay();         //draw 4*4 grid
  for(int i=0;i<5;i++){
    display.drawLine(l-i*g-i,4,l-i*g-i,w,WHITE);
    display.drawLine(l-4*g-4,w-g*i-i,l,w-i*g-i,WHITE);
  }
  
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(5,10);
  display.print("Score");
  display.setCursor(5,20);
  display.print(score);
  display.display();
}
void loop(){
  //Move Down
  if (digitalRead(buttonpin[3]) == LOW) {
    moveDown();
    addRandomTile();
    check_over();
    Display();
    delay(200);
  }
  //Move Up
  if (digitalRead(buttonpin[2]) == LOW) {
    moveUp();
    addRandomTile();
    check_over();
    Display();
    delay(200);
  }
  //Move Right
  if (digitalRead(buttonpin[1]) == LOW) {
    moveRight();
    addRandomTile();
    check_over();
    Display();
    delay(200);
  }
  //Move Left
  if (digitalRead(buttonpin[0]) == LOW) {
    moveLeft();
    addRandomTile();    
    check_over();
    Display();
    delay(200);
  }
}
void addRandomTile() {
  isfull=1;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      if(output[i][j]==0){
        isfull=0;
      }
      if(output[i][j]==13)win_game=1;
    }
  }
  if(isfull==0){
    while (true) {
      int a = esp_random() % 4;
      int b = esp_random() % 4;
      if (output[a][b] == 0) {
        int r= esp_random()%101;
        if(r>90) output[a][b]=2;
        else output[a][b]=1;
        break;
      }
    }
  }
}
void moveDown() {
  // Step 1: Compress all non-zero tiles downwards
  for (int col = 0; col < 4; col++) {
    for (int row = 2; row >=0; row--) {
      if (output[row][col] != 0) {
        int next = row;
        while (next +1 < 4 && output[next + 1][col] == 0) {
          output[next + 1][col] = output[next][col];
          output[next][col] = 0;
          next++;
        }
      }
    }
  }
  // Step 2: Merge adjacent tiles with same value
  for (int col = 0; col < 4; col++) {
    for (int row = 2; row >= 0; row--) {
      if (output[row][col] != 0 && output[row][col] == output[row + 1][col]) {
        output[row + 1][col] += 1;
        score=score+pow(2,output[row + 1][col]);
        output[row][col] = 0;
      }
    }
  }
  // Step 3: Compress again after merging
  for (int col = 0; col < 4; col++) {
    for (int row = 2; row >=0; row--) {
      if (output[row][col] != 0) {
        int next = row;
        while (next +1 < 4 && output[next + 1][col] == 0) {
          output[next + 1][col] = output[next][col];
          output[next][col] = 0;
          next++;
        }
      }
    }
  }

}
void moveUp() {
  // Step 1: Compress all non-zero tiles upwards
  for (int col = 0; col < 4; col++) {
    for (int row = 1; row <=3; row++) {
      if (output[row][col] != 0) {
        int next = row;
        while (next - 1 >=0 && output[next - 1][col] == 0) {
          output[next - 1][col] = output[next][col];
          output[next][col] = 0;
          next--;
        }
      }
    }
  }
  // Step 2: Merge adjacent tiles with same value
  for (int col = 0; col < 4; col++) {
    for (int row = 1; row <=3; row++) {
      if (output[row][col] != 0 && output[row][col] == output[row - 1][col]) {
        output[row - 1][col] += 1;
        score=score+pow(2,output[row - 1][col]);
        output[row][col] = 0;
      }
    }
  }
  // Step 3: Compress again after merging
  for (int col = 0; col < 4; col++) {
    for (int row = 1; row <=3; row++) {
      if (output[row][col] != 0) {
        int next = row;
        while (next - 1 >=0 && output[next - 1][col] == 0) {
          output[next - 1][col] = output[next][col];
          output[next][col] = 0;
          next--;
        }
      }
    }
  }
}
void moveRight() {
  // Step 1: Compress all non-zero tiles right
  for (int col = 2; col>= 0; col--) {
    for (int row = 0; row <4; row++) {
      if (output[row][col] != 0) {
        int next = col;
        while (next + 1 < 4 && output[row][next+1] == 0) {
          output[row][next + 1] = output[row][next];
          output[row][next] = 0;
          next++;
        }
      }
    }
  }
  // Step 2: Merge adjacent tiles with same value
  for (int col = 2; col >= 0; col--) {
    for (int row = 0; row < 4; row++) {
      if (output[row][col] != 0 && output[row][col] == output[row][col+1]) {
        output[row][col+1] += 1;
        score=score+pow(2,output[row][col+1]);
        output[row][col] = 0;
      }
    }
  }
  /// Step 3: Compress again after merging
  for (int col = 2; col>= 0; col--) {
    for (int row = 0; row <4; row++) {
      if (output[row][col] != 0) {
        int next = col;
        while (next + 1 < 4 && output[row][next+1] == 0) {
          output[row][next + 1] = output[row][next];
          output[row][next] = 0;
          next++;
        }
      }
    }
  }
}
void moveLeft() {
  // Step 1: Compress all non-zero tiles left
  for (int col = 1; col <= 3;col++) {
    for (int row = 0; row < 4; row++) {
      if (output[row][col] != 0) {
        int next = col;
        while (next - 1 >= 0 && output[row][next-1] == 0) {
          output[row][next-1] = output[row][next];
          output[row][next] = 0;
          next--;
        }
      }
    }
  }
  // Step 2: Merge adjacent tiles with same value
  for (int col = 1; col <= 3; col++) {
    for (int row = 0; row <4; row++) {
      if (output[row][col] != 0 && output[row][col] == output[row][col-1]) {
        output[row][col-1] += 1;
        score=score+pow(2,output[row][col-1]);
        output[row][col] = 0;
      }
    }
  }
  // Step 3: Compress again after merging
  for (int col = 1; col <= 3;col++) {
    for (int row = 0; row < 4; row++) {
      if (output[row][col] != 0) {
        int next = col;
        while (next - 1 >= 0 && output[row][next-1] == 0) {
          output[row][next-1] = output[row][next];
          output[row][next] = 0;
          next--;
        }
      }
    }
  }
}
void check_over(){
  if(isfull==1){
    game_over=1;
    for (int p=0;p<4;p++){
      for(int q=0;q<4;q++){
        int current=output[p][q];
        if((q<3) && (output[p][q+1]==current)){
          game_over=0;
        }
        if((p<3) && (output[p+1][q]==current)){
          game_over=0;
        }
      }
    }
  }
  if(game_over==1 || win_game==1){
    Display();
    display.display();
    display.clearDisplay();
    int16_t x1,y1;
    uint16_t width,hight;
    if(win_game==1){
      display.getTextBounds(words2,0,0,&x1,&y1,&width,&hight);
      int x2 =(screen_width - width)/2;
      int y2 =(screen_hight - hight)/2;
      display.setCursor(x2,y2);
      display.setTextSize(1);
      display.print(words2);
      display.display();
    }
    else{
      display.getTextBounds(words1,0,0,&x1,&y1,&width,&hight);
      int x2 =(screen_width - width)/2;
      int y2 =(screen_hight-hight)/2;
      display.setCursor(x2,y2);
      display.setTextSize(1);
      display.print(words1);
    }
    
    display.display();
    delay(1000);
    display.clearDisplay();
    Display();
    display.display();
    delay(3000);
    display.clearDisplay();
    score=0;
    game_over=0;
    for(int i=0;i<4;i++){
      for(int j=0;j<4;j++){
        output[i][j]=0;
      }
    }
    Display();
    display.display();
  }

}
void Display(){
  display.clearDisplay();
 for(int i=0;i<5;i++){
    display.drawLine(l-i*g-i,4,l-i*g-i,w,WHITE);
    display.drawLine(l-4*g-4,w-g*i-i,l,w-i*g-i,WHITE);
  }
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(5,10);
  display.print("Score");
  display.setCursor(5,20);
  display.print(score);
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      int power=output[i][j];
      if(output[i][j]!=0){
        display.drawBitmap(x+j*15,y+i*15,num[power-1],14,14,WHITE);
      }
    }
  }
  display.display();
}