#include "adafruit_pwmservodriver/Adafruit_PWMServoDriver.h"
#include "hall_driven_motor/hall_driven_motor.h"
#include "mbed.h"

// Nucleo L432KC
#define I2C_SDA PA_10
#define I2C_SCL PA_9
#define FLAG_START_1 1
#define FLAG_STOP_1 2
#define FLAG_START_2 3
#define FLAG_STOP_2 4
#define MOTOR_SHIELD_TYPE                                                      \
  1 // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward

int val1 = 0;
int val2 = 0;
I2C i2c(I2C_SDA, I2C_SCL);

Adafruit_PWMServoDriver
    pwm(0x40, i2c); // Carte d'extension 16 sorties pour le pilotage de servos
                    // en PWM  (adresse I2C par defaut 0x40)

hall_driven_motor motor1(PB_0, PA_12, pwm, 12, 13, val1, 'A', FLAG_START_1,
                         FLAG_STOP_1, MOTOR_SHIELD_TYPE);
// hall_driven_motor motor2(PB_1,PA_8, pwm,7,
// 6,val2,'B',FLAG_START_2,FLAG_STOP_2,MOTOR_SHIELD_TYPE);
Thread thread_motor1;
// Thread thread_motor2;
EventFlags event_flags;

void init() {
  printf("init\n");

  pwm.begin();

    pwm.setPWMFreq(100); // This is the maximum PWM frequency
                       // RAZ pwm
  for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum++) {
    pwm.setPin(pwmnum, 0, true); 
  }
   

  motor1.set_min_motor_speed(200);
  motor1.set_max_motor_speed(1000); 
  motor1.set_coef_accel_motor(1.1);
  motor1.set_coef_decel_motor(5);
  motor1.init();

  // motor2.set_min_motor_speed(1000);
  // motor2.set_coef_accel_motor(1.1);
  // motor2.set_coef_decel_motor(5);
  // motor2.init();
}
// main() runs in its own thread in the OS
// TODO utiliser les threads pour faire tourner les moteurs en //

void run_motor_in_thread(hall_driven_motor *motor) {

  while (true) {
    event_flags.wait_any(motor->get_cmde_flag_start()); // attend que le moteur
    motor->run();
    event_flags.set(motor->get_cmde_flag_stop()); // attend que le moteur
  }
}

int main() {
  init();

  int flag1 = 1;
  int flag2 = 2;
  uint32_t flags_read = 0;

  thread_motor1.start(callback(run_motor_in_thread, &motor1));
  // thread_motor2.start(callback(run_motor_in_thread, &motor2 ));

  while (true) {
    // motor1.run(500);
    // motor2.run(500);
    // event_flags.wait_all(FLAG_STOP_1|FLAG_STOP_2 ); // attend que les moteurs
    // event_flags.wait_all(FLAG_STOP_1 ); // attend que les moteurs
    val1 = 1242*2;
    event_flags.set(FLAG_START_1);
    //  val2 = 1800;
    //  event_flags.set(FLAG_START_2);
    //  event_flags.wait_all(FLAG_STOP_1|FLAG_STOP_2 ); // attend que les
    //  moteurs
    event_flags.wait_all(FLAG_STOP_1); // attend que les moteurs
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // motor1.run(0);
    // motor2.run(0);
    val1 = 0;
    //    val2 = 0;
    event_flags.set(FLAG_START_1);
    // event_flags.set(FLAG_START_2);
    //    event_flags.wait_all(FLAG_STOP_1|FLAG_STOP_2 ); // attend que les
    //    moteurs
    event_flags.wait_all(FLAG_STOP_1); // attend que les moteurs
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
