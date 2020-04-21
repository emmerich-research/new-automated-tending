#include "device.hpp"

#include "gpio.hpp"

#ifdef MOCK_GPIO

// General
int gpioInitialise(void) {
  return 0;
}

void gpioTerminate(void) {}

// Digital
int gpioSetMode([[maybe_unused]] int gpio, [[maybe_unused]] int mode) {
  return 0;
}

int gpioRead([[maybe_unused]] int gpio) {
  return 0;
}

int gpioWrite([[maybe_unused]] int gpio, [[maybe_unused]] int level) {
  return 0;
}

// SPI
int i2cOpen([[maybe_unused]] unsigned int i2cBus,
            [[maybe_unused]] unsigned int i2cAddr,
            [[maybe_unused]] unsigned int i2cFlags) {
  return 0;
}

int i2cClose([[maybe_unused]] unsigned int handle) {
  return 0;
}

int i2cWriteDevice([[maybe_unused]] unsigned int handle,
                   [[maybe_unused]] char*        buf,
                   [[maybe_unused]] unsigned int count) {
  return 0;
}

int i2cReadDevice([[maybe_unused]] unsigned int handle,
                  [[maybe_unused]] char*        buf,
                  [[maybe_unused]] unsigned int count) {
  return 0;
}

int i2cWriteByte([[maybe_unused]] unsigned int handle,
                 [[maybe_unused]] unsigned int bVal) {
  return 0;
}

int i2cReadByte([[maybe_unused]] unsigned int handle) {
  return 0;
}

// PWM

int gpioPWM([[maybe_unused]] unsigned int user_gpio,
            [[maybe_unused]] unsigned int dutycycle) {
  return 0;
}

int gpioGetPWMdutycycle([[maybe_unused]] unsigned int user_gpio) {
  return 0;
}

int gpioSetPWMrange([[maybe_unused]] unsigned int user_gpio,
                    [[maybe_unused]] unsigned int range) {
  return 0;
}

int gpioGetPWMrange([[maybe_unused]] unsigned int user_gpio) {
  return 0;
}

int gpioGetPWMrealRange([[maybe_unused]] unsigned int user_gpio) {
  return 0;
}

int gpioSetPWMfrequency([[maybe_unused]] unsigned int user_gpio,
                        [[maybe_unused]] unsigned int frequency) {
  return 0;
}

int gpioGetPWMfrequency([[maybe_unused]] unsigned int user_gpio) {
  return 0;
}

int gpioHardwarePWM([[maybe_unused]] unsigned int gpio,
                    [[maybe_unused]] unsigned int PWMfreq,
                    [[maybe_unused]] unsigned int PWMduty) {
  return 0;
}

#endif  // MOCK_GPIO
