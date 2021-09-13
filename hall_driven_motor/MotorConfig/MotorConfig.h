
#include "mbed.h"
/*!
 *  @brief classe pour définir les paramètre d'un moteur
 */
class MotorConfig
{
public:
    MotorConfig(PinName count_pin,     //pin compteur de tour
                PinName stop_pin,      //pin de fin de course
                int forward_or_dir_pin,       //pin de commande de la direction du moteur
                int backward_or_speed_pin,      //pin de commande de la vitesse du moteur
                char motor_name,       //nom du moteur
                int motor_shield_type, //motor_shield_type:1=type dir/speed -- 2=type Forward/backward
                int32_t flag_start,    //numéro du flag start
                int32_t flag_stop,     //numéro du flag stop
                int init_speed,        //vitesse pour l'initialisation
                int min_speed,         //vitesse mini
                int max_speed,         //vitesse maxi 
                double coef_Kp,        //PID coef proportionnel
                double coef_Ki,        //PID coef integral
                double coef_Kd,        //PID coef dérivée
                double nb_tic_per_deg  // nombre de tic par tour pour calculer l'angle
    );

private:
    PinName _count_pin;
    PinName _stop_pin;
    int _forward_or_dir_pin;
    int _backward_or_speed_pin;
    char _motor_name;
    int _motor_shield_type;
    int _flag_start;
    int _flag_stop;
    int _init_speed;
    int _min_speed;
    int _max_speed;
    double _coef_Kp;
    double _coef_Ki;
    double _coef_Kd;
    double _nb_tic_per_deg;
};
