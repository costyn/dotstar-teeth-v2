// Adafruit_DotStarMatrix example for single DotStar LED matrix.
// Scrolls 'Howdy' across the matrix.

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define DATAPIN  13
#define CLOCKPIN 14

#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

// MATRIX DECLARATION:
// Parameter 1 = width of DotStar matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   DS_MATRIX_TOP, DS_MATRIX_BOTTOM, DS_MATRIX_LEFT, DS_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     DS_MATRIX_TOP + DS_MATRIX_LEFT for the top-left corner.
//   DS_MATRIX_ROWS, DS_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   DS_MATRIX_PROGRESSIVE, DS_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type:
//   DOTSTAR_BRG  Pixels are wired for BRG bitstream (most DotStar items)
//   DOTSTAR_GBR  Pixels are wired for GBR bitstream (some older DotStars)

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
  32, 8, DATAPIN, CLOCKPIN,
  DS_MATRIX_TOP     + DS_MATRIX_LEFT +
  DS_MATRIX_COLUMNS + DS_MATRIX_ZIGZAG,
  DOTSTAR_BRG);
//Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
//  8, 8,
//  DS_MATRIX_TOP     + DS_MATRIX_RIGHT +
//  DS_MATRIX_COLUMNS + DS_MATRIX_PROGRESSIVE,
//  DOTSTAR_BRG);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

const int tWidth = 3 ;
const int tHeight = 2 ;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(64);
  teethNormal() ;
  randomSeed(analogRead(0));
}

void teethNormal() {
  matrix.fillScreen(BLACK);

  for( int y = 0 ; y < 3 ; y++ ) {
    for( int x = 0 ; x < 8 ; x++ ) {
      matrix.fillRect(x*(tWidth+1), y*(tHeight+1), tWidth, tHeight, YELLOW);
    }
  }
  matrix.show();  
}

void mTeeth2( int cX ) {
  if( cX < 0 or cX > 7 ) { return ; }
  matrix.fillRect(cX*(tWidth+1), 0, tWidth, 2, YELLOW);
  matrix.fillRect(cX*(tWidth+1), 2, tWidth, 1, BLACK);
  matrix.fillRect(cX*(tWidth+1), 3, tWidth, 1, YELLOW);
  matrix.fillRect(cX*(tWidth+1), 4, tWidth, 1, BLACK);
  matrix.fillRect(cX*(tWidth+1), 5, tWidth, 3, YELLOW);
}

void mTeeth3( int cX ) {
  if( cX < 0 or cX > 7 ) { return ; }
  matrix.fillRect(cX*(tWidth+1), 0, tWidth, 2, YELLOW);
  matrix.fillRect(cX*(tWidth+1), 2, tWidth, 1, BLACK);
  matrix.fillRect(cX*(tWidth+1), 3, tWidth, 3, YELLOW);
  matrix.fillRect(cX*(tWidth+1), 5, tWidth, 1, BLACK);
  matrix.fillRect(cX*(tWidth+1), 6, tWidth, 2, YELLOW);
}

void mTeeth4( int cX ) {
  if( cX < 0 or cX > 7 ) { return ; }
  matrix.fillRect(cX*(tWidth+1), 0, tWidth, 1, YELLOW);
  matrix.fillRect(cX*(tWidth+1), 1, tWidth, 1, BLACK);
  matrix.fillRect(cX*(tWidth+1), 2, tWidth, 4, YELLOW);
  matrix.fillRect(cX*(tWidth+1), 6, tWidth, 1, BLACK);
  matrix.fillRect(cX*(tWidth+1), 7, tWidth, 1, YELLOW);
}

const int tSpeed = 80 ;

long counter = 0 ;
int mod = 8 ;

int teethSequence[50] ;

void fillSequence( int seqLength ) {
  for( int x = 0 ; x < seqLength ; x++ ) {
     teethSequence[x] = random(3) ;
  }
  /* Pad the rest of the array with nonsense */
  for( int x = seqLength ; x < 50 ; x++ ) {
     teethSequence[x] = 5 ;
  }
}

int cIndex = 0 ;

void displaySequence() {
  for( int i=0 ; i < 50 ; i++ ) {
    for( int c=0; c < 8 and c <= i ; c++ ) {
      cIndex = i - c ;

      if( teethSequence[cIndex]==5 and c==7 ) {
        teethNormal() ;
        return ;
      }

      Serial.println(teethSequence[cIndex]) ;
      
      if( teethSequence[cIndex] == 0 ) {
        mTeeth2(c) ;
      } else if( teethSequence[cIndex] == 1 or teethSequence[cIndex] == 5 ) {
        mTeeth3(c) ;
      } else if( teethSequence[cIndex] == 2 ) {
        mTeeth4(c) ;
      }
    }
      matrix.show();  
      delay(tSpeed);
  }
  
}
  
/*
  Serial.print( "c: " ) ;
  Serial.print(counter) ;
  Serial.print( "; mod1: ");
  Serial.print(counter%7);
  Serial.print(" mod2: ");
  Serial.println( counter%7 + 1 ) ;
*/
/*  for( int x = 0 ; x < 8 ; x++ ) {
    mTeeth4( x ) ;
    delay(tSpeed);
    mTeeth4( x+1 ) ;
    delay(tSpeed);
    mTeeth4( x+2 ) ;
    mTeeth3( x ) ;
    delay(tSpeed);
  } 
  delay(tSpeed); 
  */

void loop() {  
/*  mTeeth3( counter%mod - 4 ) ;
  mTeeth2( counter%mod - 3 ) ;
  mTeeth3( counter%mod - 2 ) ;
  mTeeth4( counter%mod - 1 ) ;
  mTeeth4( counter%mod ) ;
  mTeeth4( counter%mod + 1 ) ;
  matrix.show();  
  delay(tSpeed);
  counter++ ;
  */
  fillSequence( 20 ) ;
  displaySequence() ;
  delay(2000) ;
  Serial.println( "Staring again..." ) ;
}

