#include <Adafruit_Protomatter.h> // For RGB matrix

// Matrix dimensions and configuration
#define HEIGHT  64
#define WIDTH   64
#define MAX_FPS 45
uint8_t rgbPins[]  = {7, 8, 9, 10, 11, 12};
uint8_t addrPins[] = {17, 18, 19, 20, 21};
uint8_t clockPin   = 14;
uint8_t latchPin   = 15;
uint8_t oePin      = 16;
#define NUM_ADDR_PINS 5

// Colors used in the automata
uint16_t TRUE_COLOR;
uint16_t FALSE_COLOR;

// Bitmap and state for cellular automata
uint16_t bitmap[HEIGHT][WIDTH];
uint16_t automata_state[HEIGHT][WIDTH];

// Function pointer for rule functions
typedef uint16_t (*RuleFunction)(uint16_t, uint16_t, uint16_t);

// Matrix initialization
Adafruit_Protomatter matrix(
  WIDTH, 4, 1, rgbPins, NUM_ADDR_PINS, addrPins,
  clockPin, latchPin, oePin, true);

uint16_t colors[8];

// Function to initialize the colors array with rainbow colors
void init_colors() {
    colors[0] = matrix.color565(64, 64, 64);   // Dark Gray
    colors[1] = matrix.color565(120, 79, 23);  // Brown
    colors[2] = matrix.color565(228, 3, 3);    // Red
    colors[3] = matrix.color565(255, 140, 0);  // Orange
    colors[4] = matrix.color565(255, 237, 0);  // Yellow
    colors[5] = matrix.color565(0, 128, 38);   // Green
    colors[6] = matrix.color565(0, 77, 255);   // Blue
    colors[7] = matrix.color565(117, 7, 135);  // Purple
}

// Function to get a random color from the colors array
uint16_t get_random_color() {
    int randomIndex = rand() % 8; // Random index between 0 and 7
    return colors[randomIndex];
}

// Initialize all elements of bitmap to black
void init_bitmap() {
    TRUE_COLOR = matrix.color565(0, 128, 38);
    FALSE_COLOR = matrix.color565(0, 0, 0);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            bitmap[y][x] = FALSE_COLOR;
        }
    }
}

// Apply bitmap to matrix
void apply_bitmap() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            matrix.drawPixel(x, y, bitmap[y][x]);
        }
    }
}

// Rule 22
uint16_t rule_22(uint16_t p, uint16_t q, uint16_t r) {
    return (p + q + r + p * q * r) % 2;
}

// Rule 30
uint16_t rule_30(uint16_t p, uint16_t q, uint16_t r) {
    return (p + q + r + q * r) % 2;
}

// Rule 110
uint16_t rule_110(uint16_t p, uint16_t q, uint16_t r) {
    return (q + r + q * r + p * q * r) % 2;
}

// Rule 45
uint16_t rule_45(uint16_t p, uint16_t q, uint16_t r) {
    return (1 + p + r + q * r) % 2;
}

// Rule 60
uint16_t rule_60(uint16_t p, uint16_t q, uint16_t r) {
    return (p + q) % 2;
}

// Rule 105
uint16_t rule_105(uint16_t p, uint16_t q, uint16_t r) {
    return (1 + p + q + r) % 2;
}

// Rule 225
uint16_t rule_225(uint16_t p, uint16_t q, uint16_t r) {
    return (1 + p + q + r + q * r) % 2;
}

// Rule 73
uint16_t rule_73(uint16_t p, uint16_t q, uint16_t r) {
    return (1 + p + q + r + p * r + p * q * r) % 2;
}

// Array of rule functions
uint16_t (*rules[])(uint16_t, uint16_t, uint16_t) = {rule_22, rule_30, rule_110, rule_45, rule_60, rule_105, rule_225, rule_73};

// Apply a rule to the last row of automata_state, and update bitmap
void applyRuleToRowPeriodicBC(RuleFunction rule) {
    uint16_t y = HEIGHT - 1; 
    for (uint16_t x = 0; x < WIDTH; x++) {
        uint16_t left = (x + WIDTH - 1) % WIDTH;
        uint16_t right = (x + 1) % WIDTH;
        automata_state[y][x] = rule(automata_state[y - 1][left], automata_state[y - 1][x], automata_state[y - 1][right]);
        bitmap[y][x] = automata_state[y][x] ? get_random_color() : FALSE_COLOR;
    }
}

// Scroll up the automata state and bitmap by one row
void scroll_up() {
    for (int y = 1; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            automata_state[y - 1][x] = automata_state[y][x];
            bitmap[y - 1][x] = bitmap[y][x];
        }
    }
    for (int x = 0; x < WIDTH; x++) {
        automata_state[HEIGHT - 1][x] = 0;
        bitmap[HEIGHT - 1][x] = FALSE_COLOR;
    }
}

// Initialize the automata with a single active cell in the center
void init_automata_pixel() {
    automata_state[HEIGHT - 1][WIDTH / 2] = 1;
    bitmap[HEIGHT - 1][WIDTH / 2] = TRUE_COLOR;
}

void setup(void) {
  Serial.begin(115200);
  ProtomatterStatus status = matrix.begin();
  init_colors();
}

void loop() {
    for (int r = 0; r < 3; r++) {
      init_bitmap();
      init_automata_pixel();
      apply_bitmap();
      matrix.show();
      for (int x = 0; x < 64; x++) {
        scroll_up();
        applyRuleToRowPeriodicBC(rules[r]);
        apply_bitmap();
        matrix.show();
        delay(10);
      }
      delay(2000);
    }
}
