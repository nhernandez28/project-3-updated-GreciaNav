/** \file lcddraw.c
 *  \brief Adapted from RobG's EduKit
 */
#include "lcdutils.h"
#include "lcddraw.h"


/** Draw single pixel at x,row 
 *
 *  \param col Column to draw to
 *  \param row Row to draw to
 *  \param colorBGR Color of pixel in BGR
 */
void drawPixel(u_char col, u_char row, u_int colorBGR) 
{
  lcd_setArea(col, row, col, row);
  lcd_writeColor(colorBGR);
}

void drawDiamond(char center, u_int shapeColor)
{
  for(u_char r = 0; r < 11; r++) {
    for(u_char c = 0; c <= r; c++) {
      drawPixel(center+c, r, shapeColor);
      drawPixel(center-c, r, shapeColor);
    }
  }

  for(u_char c = 0; c < 11; c++) {
    for(u_char r = 10; r <= 20-c; r++) {
      drawPixel(center+c, r, shapeColor);
      drawPixel(center-c, r, shapeColor);
    }
  }
}  
void drawLink(u_char offc, u_char offr, u_int bgColorBGR)
{
  u_char col = 0;
  u_char row = 0;
  while (row < 16) {
    col = 0;
    while (col < 16) {
      if (link[row][col] == 0)
	drawPixel(col + offc, row+offr, bgColorBGR);
      else if (link[row][col] == 1)
	drawPixel(col + offc, row+offr, COLOR_GREEN);
      else if (link[row][col] == 2)
	drawPixel(col + offc, row+offr, COLOR_BROWN);
      else if (link[row][col] == 3)
	drawPixel(col + offc, row+offr, COLOR_TAN);
      col++;
    }
    row++;
  }   
}

void drawZeldaScene()
{
  static char colorState = 0;
  u_int textColor; 
  switch (colorState) {
  case 0:
    textColor = COLOR_WHITE;
    colorState++;
    break;
  case 1:
    textColor = COLOR_RED;
    colorState = 0;
    break;
  }   
  
  drawString11x16(0,5,"It's 2020.", textColor, COLOR_BLACK);
  drawString11x16(0,22,"You've met", textColor, COLOR_BLACK);
  drawString11x16(0,39,"a terrible", textColor, COLOR_BLACK);
  drawString11x16(0,56,"fate...", textColor, COLOR_BLACK);
  drawString11x16(0,73,"Haven't", textColor, COLOR_BLACK);
  drawString11x16(0,90,"you?", textColor, COLOR_BLACK);
  drawLink(50,107, COLOR_BLACK);
}

void drawSonic(u_char offc, u_char offr, u_int bgColorBGR, u_int sonicColor) {
  u_char col = 0;
  u_char row = 0;
  while (row < 32) {
    col = 0;
    while (col < 32) {
      if (sonic[row][col] == 0)
	drawPixel(col + offc, row+offr, bgColorBGR);
      else if (sonic[row][col] == 1)
	drawPixel(col + offc, row+offr, COLOR_BLACK);
      else if (sonic[row][col] == 2)
	drawPixel(col + offc, row+offr, sonicColor);
      else if (sonic[row][col] == 3)
	drawPixel(col + offc, row+offr, COLOR_TAN);
      else if (sonic[row][col] == 4)
	drawPixel(col + offc, row+offr, COLOR_WHITE);
      else if (sonic[row][col] == 5)
	drawPixel(col + offc, row+offr, COLOR_RED);
      col++;
    }
    row++;
  }   
}

void drawSonic2(u_char offc, u_char offr, u_int bgColorBGR, u_int sonicColor) {
  u_char col = 0;
  u_char row = 0;
  while (row < 32) {
    col = 0;
    while (col < 32) {
      if (sonic2[row][col] == 0)
	drawPixel(col + offc, row+offr, bgColorBGR);
      else if (sonic2[row][col] == 1)
	drawPixel(col + offc, row+offr, COLOR_BLACK);
      else if (sonic2[row][col] == 2)
	drawPixel(col + offc, row+offr, sonicColor);
      else if (sonic2[row][col] == 3)
	drawPixel(col + offc, row+offr, COLOR_TAN);
      else if (sonic2[row][col] == 4)
	drawPixel(col + offc, row+offr, COLOR_WHITE);
      else if (sonic2[row][col] == 5)
	drawPixel(col + offc, row+offr, COLOR_RED);
      col++;
    }
    row++;
  }   
}
/** Fill rectangle
 *
 *  \param colMin Column start
 *  \param rowMin Row start
 *  \param width Width of rectangle
 *  \param height height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void fillRectangle(u_char colMin, u_char rowMin, u_char width, u_char height, 
		   u_int colorBGR)
{
  u_char colLimit = colMin + width, rowLimit = rowMin + height;
  lcd_setArea(colMin, rowMin, colLimit - 1, rowLimit - 1);
  u_int total = width * height;
  u_int c = 0;
  while ((c++) < total) {
    lcd_writeColor(colorBGR);
  }
}

/** Clear screen (fill with color)
 *  
 *  \param colorBGR The color to fill screen
 */
void clearScreen(u_int colorBGR) 
{
  u_char w = screenWidth;
  u_char h = screenHeight;
  fillRectangle(0, 0, screenWidth, screenHeight, colorBGR);
}

/** 5x7 font - this function draws background pixels
 *  Adapted from RobG's EduKit
 */
void drawChar5x7(u_char rcol, u_char rrow, char c, 
     u_int fgColorBGR, u_int bgColorBGR) 
{
  u_char col = 0;
  u_char row = 0;
  u_char bit = 0x01;
  u_char oc = c - 0x20;

  lcd_setArea(rcol, rrow, rcol + 4, rrow + 7); /* relative to requested col/row */
  while (row < 8) {
    while (col < 5) {
      u_int colorBGR = (font_5x7[oc][col] & bit) ? fgColorBGR : bgColorBGR;
      lcd_writeColor(colorBGR);
      col++;
    }
    col = 0;
    bit <<= 1;
    row++;
  }
}

/**11x16 font - this function draws background pixels
 *  Adapted from RobG's EduKit
 */
void drawChar11x16(u_char rcol, u_char rrow, char c, 
     u_int fgColorBGR, u_int bgColorBGR) 
{
  u_char col = 0;
  u_char row = 0;
  u_int bit = 0x0001;
  u_char oc = c - 0x20;

  lcd_setArea(rcol, rrow, rcol + 10, rrow + 16); /* relative to requested col/row */
  while (row < 17) {
    while (col < 11) {
      u_int colorBGR = (font_11x16[oc][col] & bit) ? fgColorBGR : bgColorBGR;
      lcd_writeColor(colorBGR);
      col++;
    }
    col = 0;
    bit <<= 1;
    row++;
  }
}

/** Draw string at col,row
 *  Type:
 *  FONT_SM - small (5x8,) FONT_MD - medium (8x12,) FONT_LG - large (11x16)
 *  FONT_SM_BKG, FONT_MD_BKG, FONT_LG_BKG - as above, but with background color
 *  Adapted from RobG's EduKit
 *
 *  \param col Column to start drawing string
 *  \param row Row to start drawing string
 *  \param string The string
 *  \param fgColorBGR Foreground color in BGR
 *  \param bgColorBGR Background color in BGR
 */
void drawString5x7(u_char col, u_char row, char *string,
		u_int fgColorBGR, u_int bgColorBGR)
{
  u_char cols = col;
  while (*string) {
    drawChar5x7(cols, row, *string++, fgColorBGR, bgColorBGR);
    cols += 6;
  }
}


/** Draw string at col,row
 *  Type:
 *  FONT_SM - small (5x8,) FONT_MD - medium (8x12,) FONT_LG - large (11x16)
 *  FONT_SM_BKG, FONT_MD_BKG, FONT_LG_BKG - as above, but with background color
 *  Adapted from RobG's EduKit
 *
 *  \param col Column to start drawing string
 *  \param row Row to start drawing string
 *  \param string The string
 *  \param fgColorBGR Foreground color in BGR
 *  \param bgColorBGR Background color in BGR
 */
void drawString11x16(u_char col, u_char row, char *string,
		u_int fgColorBGR, u_int bgColorBGR)
{
  u_char cols = col;
  while (*string) {
    drawChar11x16(cols, row, *string++, fgColorBGR, bgColorBGR);
    cols += 12;
  }
}


/** Draw rectangle outline
 *  
 *  \param colMin Column start
 *  \param rowMin Row start 
 *  \param width Width of rectangle
 *  \param height Height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void drawRectOutline(u_char colMin, u_char rowMin, u_char width, u_char height,
		     u_int colorBGR)
{
  /**< top & bot */
  fillRectangle(colMin, rowMin, width, 1, colorBGR);
  fillRectangle(colMin, rowMin + height, width, 1, colorBGR);

  /**< left & right */
  fillRectangle(colMin, rowMin, 1, height, colorBGR);
  fillRectangle(colMin + width, rowMin, 1, height, colorBGR);
}

