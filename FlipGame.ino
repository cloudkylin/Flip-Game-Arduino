/**
 * Created by Arduino IDE 1.8.1
 * Name: FlipGame
 * User: CloudKylin
 * Data: 2017-06-23
 * Base on MEGA 2560
**/
#include <LiquidCrystal.h>
#include <IRremote.h>
#define LCD_D7 2
#define LCD_D6 3
#define LCD_D5 4
#define LCD_D4 5
#define BUTTON 6
#define IR 8
#define LCD_E 11
#define LCD_RS 12

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
IRrecv irrecv(IR);
decode_results results;

int letime = 0;//控制游戏说明显示位置
int stepn = 0;//显示当前操作步数
int maps[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};//控制显示整个点阵
int now[2] = {0};//当前操作点所在位置
int success = 0;//判断游戏是否结束
int R[8] = {28, 47, 45, 22, 41, 24, 32, 34}; //行
int C[8] = {43, 49, 39, 51, 26, 37, 30, 23}; //列

void lcdprint(char lin1[], char lin2[]);//LCD输出显示内容
void start(int t);//打印游戏说明
int delaya();//按键去抖动
void IRread();//获取红外信号
void up();//控制上移
void down();//控制下移
void left();//控制左移
void right();//控制右移
void pass();//控制翻转
int change(int x, int y);//选中翻转
int ifsuccess();//判断游戏是否胜利
void asdisplay();//刷新点阵
void Clear();//去除点阵余晖

void setup() {
  lcd.begin(16, 2);//定义LCD1602显示屏
  pinMode(BUTTON, INPUT); //定义按键
  //定义红外
  Serial.begin(9600);
  irrecv.enableIRIn();
  //定义点阵
  for (int i = 0; i < 8; i++)
  {
    pinMode(R[i], OUTPUT);
    pinMode(C[i], OUTPUT);
    digitalWrite(C[i], HIGH);
    digitalWrite(R[i], LOW);
  }
  
  pinMode(13, OUTPUT);//调试灯
}

void loop() {
  /*显示说明*/
  if (letime < 44)
  {
    if (delaya())
    {
      //Serial.print(letime);
      if (letime % 2 == 1)
        digitalWrite(13, HIGH);
      else
        digitalWrite(13, LOW);
      start(letime);
      letime += 1;
    }
  }

  /*开始游戏*/
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("TIME: ");
    lcd.print(millis() / 1000);
    lcd.setCursor(0, 1);
    IRread();
  }
}


void lcdprint(char lin1[], char lin2[]) //LCD输出显示内容
{
  lcd.setCursor(0, 0);
  lcd.print(lin1);
  lcd.setCursor(0, 1);
  lcd.print(lin2);
}

void start(int t) //打印游戏说明
{
  char out[45][17] = {"***FLIP GAME*** ", "Flip game is pl-", "ayed on a recta-", "ngular 8x8 field", "with two-sided  ",
                      "pieces placed on", "each of its 64  ", "squares. One si-", "de of each piece", "is white and the",
                      "other one is bl-", "ack and each pi-", "ece is lying ei-", "ther it's black ", "or white side up",
                      ".Each round you ", "flip 3 to 5 pie-", "ces, thus chang-", "ing the color of", "their upper side",
                      "from black to w-", "hite and vice v-", "ersa. The pieces", "to be flipped a-", "re chosen every ",
                      "round according ", "to the following", "rules:          ", "  1.Choose any  ", "one of the 64 p-",
                      "ieces.          ", "  2.Flip the ch-", "osen piece and  ", "also all adjace-", "nt pieces to the",
                      "left, to the ri-", "ght, to the top,", "and to the bott-", "om of the chosen", "piece (if there ",
                      "are any).       ", "                ", "Now,let's begin!", "                ", "                "
                     };
  if (t < 44) lcdprint(out[t], out[t + 1]);
  else return;
}

int delaya()  //按键去抖动
{
  if (digitalRead(BUTTON) == HIGH)
  {
    delay(5);
    if (digitalRead(BUTTON) == HIGH)
    {
      while (digitalRead(BUTTON) == HIGH);
      return 1;
    }
  }
  return 0;
}

void IRread()
{
  asdisplay();
  if (irrecv.decode(&results))
  {
    int ans = 0;
    ans = results.value;
    Serial.println(ans);
    switch (ans)
    {
      case 0xFF18E7://上
        {
          asdisplay();
          up();
          break;
        }
      case 0xFF4AB5://下
        {
          asdisplay();
          down();
          break;
        }
      case 0xFF10EF://左
        {
          asdisplay();
          left();
          break;
        }
      case 0xFF5AA5://右
        {
          asdisplay();
          right();
          break;
        }
      case 0xFF38C7://确定
        {
          asdisplay();
          pass();
          asdisplay();
          break;
        }
      default:
        asdisplay();
        break;
    }
    irrecv.resume();
  }
}

void up()
{
  if (now[1] > 0)
    now[1]--;
  maps[now[0]][now[1]] = !maps[now[0]][now[1]];
  for (int i = 0; i < 50; i++)
    asdisplay();
  maps[now[0]][now[1]] = !maps[now[0]][now[1]];
  for (int i = 0; i < 50; i++)
    asdisplay();
}

void down()
{
  if (now[1] < 7)
    now[1]++;
  maps[now[0]][now[1]] = !maps[now[0]][now[1]];
  for (int i = 0; i < 50; i++)
    asdisplay();
  maps[now[0]][now[1]] = !maps[now[0]][now[1]];
  for (int i = 0; i < 50; i++)
    asdisplay();
}

void left()
{
  if (now[0] > 0)
    now[0]--;
  maps[now[0]][now[1]] = !maps[now[0]][now[1]];
  for (int i = 0; i < 50; i++)
    asdisplay();
  maps[now[0]][now[1]] = !maps[now[0]][now[1]];
  for (int i = 0; i < 50; i++)
    asdisplay();
}

void right()
{
  if (now[0] < 7)
    now[0]++;
  maps[now[0]][now[1]] = !maps[now[0]][now[1]];
  for (int i = 0; i < 50; i++)
    asdisplay();
  maps[now[0]][now[1]] = !maps[now[0]][now[1]];
  for (int i = 0; i < 50; i++)
    asdisplay();
}

void pass()
{
  success = change(now[0], now[1]);
  stepn++;
  lcd.print("STEP: ");
  lcd.print(stepn);
  ifsuccess();
}

int change(int x, int y)//选中翻转
{
  maps[x][y] = !maps[x][y];
  if (x - 1 >= 0)
    maps[x - 1][y] = !maps[x - 1][y];
  if (x + 1 <= 7)
    maps[x + 1][y] = !maps[x + 1][y];
  if (y + 1 <= 7)
    maps[x][y + 1] = !maps[x][y + 1];
  if (y - 1 >= 0)
    maps[x][y - 1] = !maps[x][y - 1];
  int pd = 0;
  for (int i = 0; i <= 7; i++)
    for (int j = 0; j <= 7; j++)
      if (maps[i][j] == 1)
        pd++;
  if (pd == 0)
    return 1;
  else return 0;
}

int ifsuccess()
{
  while (success == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("--YOU ARE WIN!--");
    lcd.setCursor(0, 1);
    lcd.print("STEP: ");
    lcd.print(stepn);
  }
}

void asdisplay()
{
  for (int c = 0; c < 8; c++)
  {
    digitalWrite(R[c], HIGH);
    for (int r = 0; r < 8; r++)
    {
      digitalWrite(C[r], !maps[r][c]);
    }
    delay(1);
    Clear();
  }
}

void Clear()
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(R[i], LOW);
    digitalWrite(C[i], HIGH);
  }
}