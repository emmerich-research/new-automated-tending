#include "device.hpp"

#include "gpio.hpp"

#ifdef MOCK_GPIO

// General
int gpioInitialise(void) {
  return PI_OK;
}

void gpioTerminate(void) {}

// Digital
int gpioSetMode([[maybe_unused]] int gpio, [[maybe_unused]] int mode) {
  return PI_OK;
}

int gpioRead([[maybe_unused]] int gpio) {
  return PI_OK;
}

int gpioWrite([[maybe_unused]] int gpio, [[maybe_unused]] int level) {
  return PI_OK;
}

// SPI
int i2cOpen([[maybe_unused]] unsigned int i2cBus,
            [[maybe_unused]] unsigned int i2cAddr,
            [[maybe_unused]] unsigned int i2cFlags) {
  return PI_OK;
}

int i2cClose([[maybe_unused]] unsigned int handle) {
  return PI_OK;
}

int i2cWriteDevice([[maybe_unused]] unsigned int handle,
                   [[maybe_unused]] char*        buf,
                   [[maybe_unused]] unsigned int count) {
  return PI_OK;
}

int i2cReadDevice([[maybe_unused]] unsigned int handle,
                  [[maybe_unused]] char*        buf,
                  [[maybe_unused]] unsigned int count) {
  return PI_OK;
}

int i2cWriteByte([[maybe_unused]] unsigned int handle,
                 [[maybe_unused]] unsigned int bVal) {
  return PI_OK;
}

int i2cReadByte([[maybe_unused]] unsigned int handle) {
  return PI_OK;
}

// PWM

int gpioPWM([[maybe_unused]] unsigned int user_gpio,
            [[maybe_unused]] unsigned int dutycycle) {
  return PI_OK;
}

int gpioGetPWMdutycycle([[maybe_unused]] unsigned int user_gpio) {
  return PI_OK;
}

int gpioSetPWMrange([[maybe_unused]] unsigned int user_gpio,
                    [[maybe_unused]] unsigned int range) {
  return PI_OK;
}

int gpioGetPWMrange([[maybe_unused]] unsigned int user_gpio) {
  return PI_OK;
}

int gpioGetPWMrealRange([[maybe_unused]] unsigned int user_gpio) {
  return PI_OK;
}

int gpioSetPWMfrequency([[maybe_unused]] unsigned int user_gpio,
                        [[maybe_unused]] unsigned int frequency) {
  return PI_OK;
}

int gpioGetPWMfrequency([[maybe_unused]] unsigned int user_gpio) {
  return PI_OK;
}

int gpioHardwarePWM([[maybe_unused]] unsigned int gpio,
                    [[maybe_unused]] unsigned int PWMfreq,
                    [[maybe_unused]] unsigned int PWMduty) {
  return PI_OK;
}

int gpioSetPullUpDown([[maybe_unused]] unsigned gpio,
                      [[maybe_unused]] unsigned pud) {
  return PI_OK;
}

#endif  // MOCK_GPIO
