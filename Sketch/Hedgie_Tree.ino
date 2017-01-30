// ===== INCLUDES ===== //
//Neopixel library
#include <Adafruit_NeoPixel.h>
// ===== END INCLUDES ===== //

// ===== VARIABLES AND CONSTANTS ===== //
#define ROWS 1
#define COLS 150
#define DATA_PIN 12

//Number of pixels
int nPixels = ROWS*COLS;
float center[2] = {
	static_cast<float>(COLS - 1)/2.0,
	static_cast<float>(ROWS - 1)/2.0
};

//Dimming constant
const double DIM = 5;

//Time variables
float t = 0;
float dt = 0.01;
float delayTime = 4;

// Sweeping Location
float spotLight = 0;
float spotRadius = 5;

//Color type
class Color{
	public:
		//Constructor
		Color(int R = 0, int G = 0, int B = 0) : R(R), G(G), B(B){}

		//Color values
		short int R, G, B;
};

//Color operators
Color operator+(const Color a, const Color b){
	return Color(a.R + b.R, a.G + b.G, a.B + b.B);
}
Color operator-(const Color a, const Color b){
	return Color(a.R - b.R, a.G - b.G, a.B - b.B);
}

Color operator*(const double t, const Color a){
	return Color(t*a.R, t*a.G, t*a.B);
}

Color operator*(const Color a, const Color b){
	return Color(a.R*b.R, a.G*b.G, a.B*b.B);
}

Color operator/(const Color a, const double t){
	return Color(a.R/t, a.G/t, a.B/t);
}


//Constant colors
const Color RED = Color(255, 0, 0);
const Color ORANGE = Color(220, 65, 0);
const Color YELLOW = Color(255, 255, 0);
const Color GREEN = Color(0, 255, 0);
const Color DARKGREEN = Color(0, 15, 4);
const Color BLUE = Color(0, 0, 255);
const Color CYAN = Color(0, 255, 255)/3;
const Color PURPLE = Color(180, 0, 255);
const Color BLACK = Color(0, 0, 0);
const Color WHITE = Color(255, 255, 255);

//Neopixel driver
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
	nPixels,
	DATA_PIN,
	NEO_GRB + NEO_KHZ800
);
// ===== END VARIABLES AND CONSTANTS ===== //

// ===== SETUP FUNCTION ===== //
void setup(){
	//Begin the NeoPixel driver
	pixels.begin();
}
// ===== END SETUP FUNCTION ===== //

// ===== LOOP FUNCTION ===== //
void loop(){
	//Make sure t stays between 0 and 1
	t += dt;

	// Location of spot = nPixels*sin(t)/2
	// Determine Spotlight Location
	spotLight = static_cast<float>(nPixels)*(sin(t) + 1)/2;

	//Cycle through all pixels
	for (int x = 0; x < COLS; x++){
		// Find Location Relative to Spot Light
		float dx = spotLight - x;
		float local = abs(dx);

		// Find Intensity
		float level = exp(-(pow((x - spotLight)/spotRadius, 2)));

		// Write Value
		setColor(x, 0, lerp(DARKGREEN, CYAN, level));
		//setColor(x, 0, lerp(BLACK, colorSin(level), level));
	}

	//Show pixels
	pixels.show();

	//Wait a short while
	delay(delayTime);
}
// ===== END LOOP FUNCTION ===== //


// ===== FUNCTION DEFINITION ===== //
//Linear interpolation
Color lerp(Color a, Color b, double t){
	double s = 1-t;
	return s*a + t*b;
}

//Distance to center
double rDist(float x, float y){
	//Find components
	float rX = center[0] - x;
	float rY = center[1] - y;

	//Calculate distance
	return sqrt(rX*rX + rY*rY);
}

//Periodic color generator
Color colorSin(double t){
	//Make sure t doesn't get to big
	t *= (t > 100) ? 0 : 2*PI;

	//Determine RGB values along a circular walk through RGB space
	double rVal = sin(t) + 1;
	double gVal = sin(t + 2.0*PI/3.0) + 1;
	double bVal = sin(t + 4.0*PI/3.0) + 1;

	//Return the color with amplitude multipliers
	return
		Color(round(127*rVal), round(127*gVal), round(127*bVal))
		- Color(1, 1, 1);
}

//Set pixel (x, y) to a color 
void setColor(int x, int y, Color c){
	//Convert from 2D to 1D
	int n = y*COLS + x;

	//Set the pixel color
	pixels.setPixelColor(n, pixels.Color(c.R, c.G, c.B));
}

//Clear all pixels by setting them to black
void clearPixels(){
	for (int i = 0; i < COLS; i++){
		for (int j = 0; j < ROWS; j++){
			setColor(i, j, BLACK);
		}
	}
}
// ===== END FUNCTION DEFINITION ===== //
