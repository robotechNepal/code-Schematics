#include <Arduino.h>

#define SAMPLES 128             // Number of samples. Should be a power of 2
#define SAMPLING_FREQUENCY 500  // Hz, must be > 2 * max frequency of the signal
#define PI 3.1415926535897932384626433832795028841971

unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];

// Function prototypes
void FFT(double* real, double* imag, int n);
void swap(double* a, double* b);
double cosine(double x);
double sine(double x);
double sqrt(double x);

void setup() {
  // Initialize the serial communication:
  Serial.begin(115200);
  pinMode(5, INPUT); // Setup for leads off detection LO +
  pinMode(6, INPUT); // Setup for leads off detection LO -

  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

void loop() {
  if ((digitalRead(10) == 1) || (digitalRead(11) == 1)) {
    Serial.println('!');
  } else {
    // Collect SAMPLES number of samples
    for (int i = 0; i < SAMPLES; i++) {
      microseconds = micros();
      vReal[i] = analogRead(A0);
      Serial.println(analogRead(A0));
      vImag[i] = 0;
      while (micros() < (microseconds + sampling_period_us)) {
        // wait
      }
    }

    // Compute FFT
    FFT(vReal, vImag, SAMPLES);

    // Find the peak frequency
    double peak = 0;
    int peakIndex = 0;
    for (int i = 0; i < (SAMPLES / 2); i++) {
      double magnitude = sqrt(vReal[i] * vReal[i] + vImag[i] * vImag[i]);
      if (magnitude > peak) {
        peak = magnitude;
        peakIndex = i;
      }
    }

    double peakFrequency = (peakIndex * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
    Serial.println(peakFrequency);

    // Optionally, you can send all FFT values
    for (int i = 0; i < (SAMPLES / 2); i++) {
      double magnitude = sqrt(vReal[i] * vReal[i] + vImag[i] * vImag[i]);
      Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
      Serial.print(" ");
      Serial.println(magnitude, 1);
    }
  }

  delay(50); // Repeat the process every second
}

void FFT(double* real, double* imag, int n) {
  // Bit-reverse
  int j = 0;
  for (int i = 0; i < n; i++) {
    if (i < j) {
      swap(&real[i], &real[j]);
      swap(&imag[i], &imag[j]);
    }
    int m = n / 2;
    while (j >= m && m > 0) {
      j -= m;
      m /= 2;
    }
    j += m;
  }

  // FFT
  int mmax = 1;
  while (n > mmax) {
    int step = mmax * 2;
    double theta = -2 * PI / step;
    double wtemp, wr, wi;
    for (int m = 0; m < mmax; m++) {
      wr = cosine(m * theta);
      wi = sine(m * theta);
      for (int i = m; i < n; i += step) {
        j = i + mmax;
        wtemp = wr * real[j] - wi * imag[j];
        imag[j] = wr * imag[j] + wi * real[j];
        real[j] = real[i] - wtemp;
        imag[j] = imag[i] - imag[j];
        real[i] += wtemp;
        imag[i] += imag[j];
      }
    }
    mmax = step;
  }
}

void swap(double* a, double* b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}

double cosine(double x) {
  // Use a simple approximation or precomputed values
  // Here we use a simple Taylor series expansion
  const double c1 = 1;
  const double c2 = -0.5;
  const double c3 = 1.0 / 24;
  const double c4 = -1.0 / 720;
  double x2 = x * x;
  return c1 + x2 * (c2 + x2 * (c3 + x2 * c4));
}

double sine(double x) {
  // Use a simple approximation or precomputed values
  // Here we use a simple Taylor series expansion
  const double s1 = x;
  const double s2 = -x * x * x / 6;
  const double s3 = x * x * x * x * x / 120;
  const double s4 = -x * x * x * x * x * x * x / 5040;
  return s1 + s2 + s3 + s4;
}

double sqrt(double x) {
  // Simple approximation of square root using Newton's method
  double guess = x / 2.0;
  for (int i = 0; i < 10; i++) {
    guess = (guess + x / guess) / 2.0;
  }
  return guess;
}
