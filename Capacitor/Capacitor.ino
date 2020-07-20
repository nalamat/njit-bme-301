#define SAMPLING_PERIOD 10               // Time between consecutive analog readings in milliseconds
#define TOTAL_SAMPLES   500              // Total number of analog readings, 0: unlimited, 500: one window
#define INITIAL_STATE   2                // Initial state of the capacitor, 0: none, 1: charged, 2: discharged
#define PULSE_PERIOD    1000             // Period of the pulse waveform in milliseconds, 0: no pulse

#define BAUD_RATE       2000000          // Serial transfer rate in bits/s

unsigned long startTime   = 0;
unsigned long currentTime = 0;
unsigned long counter     = 0;
unsigned int  pulse       = 0;

// Custom function for reading the analog voltage on A0  and sending out via serial port to PC
void measure() {
    int val = analogRead(A0) / 1023. * 5000;                 // Read analog value and convert it to units of mV
    
    Serial.print(val);                                       // Send out the read value of A0 over serial port to PC
    
    if (PULSE_PERIOD) {                                      // Send out the value of the pulse on A1 to PC
        Serial.print(" ");
        Serial.print(pulse * 5000);
    }

    Serial.println();
    counter += 1;                                            // Count total number of measurements
}

// Arduino setup function, called once when Arduino turns on
void setup() {
    if (PULSE_PERIOD) {                                      // Prepare for outputting pulses on A1 if set
        pinMode(A1, OUTPUT);
        digitalWrite(A1, LOW);
    }
    
    if (INITIAL_STATE) {                                     // Set initial state for the capacitor (charged or discharged)
        pinMode(A0, OUTPUT);
        digitalWrite(A0, HIGH ? INITIAL_STATE==1 : LOW);
        delay(500);
    }
    
    Serial.begin(BAUD_RATE);                                 // Initialize serial communication
    
    pinMode(A0, INPUT);                                      // From now on A0 is only used for reading voltages
    digitalWrite(A0, LOW);
    
    if (PULSE_PERIOD)                                        // Send out name of the channels for labeling in Serial Plotter
        Serial.println("A0 A1");
    
    startTime = micros();                                    // Read the start time and use it as reference
    
    measure();                                               // Make the first measurement before going to loop
}

// Arduino loop function, called repeatedly until Arduino turns off
void loop() {
  if (TOTAL_SAMPLES && counter == TOTAL_SAMPLES)             // Stop measuring if a limit is set for total number of samples
    return;

  currentTime = micros() - startTime;                        // Get the current system time in microseconds

  if (PULSE_PERIOD) {                                        // Adjust pulse output on A1
    pulse = (currentTime / (PULSE_PERIOD*1000L/2) +
      (INITIAL_STATE+1) % 2) % 2;
    digitalWrite(A1, pulse);
  }

  if (currentTime / (SAMPLING_PERIOD*1000L) != counter - 1)  // Measure analog input on A0
    measure();
}
