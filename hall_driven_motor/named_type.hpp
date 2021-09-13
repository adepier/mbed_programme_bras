#include "../NamedType/include/NamedType/named_type.hpp"
#include "PinNames.h"  

using FirstName = fluent::NamedType<std::string, struct FirstNameTag>;
using LastName = fluent::NamedType<std::string, struct LastNameTag>;

using Count_pin = fluent::NamedType< PinName, struct Count_pinTag>;
// PinName count_pin,             //pin compteur de tour
//     PinName stop_pin,          //pin de fin de course
//     int forward_or_dir_pin,    //pin de commande de la direction du moteur
//     int backward_or_speed_pin, //pin de commande de la vitesse du moteur
//     char motor_name,           //nom du moteur
//     int motor_shield_type,     //motor_shield_type:1=type dir/speed -- 2=type Forward/backward
//     int32_t flag_start,        //numéro du flag start
//     int32_t flag_stop,         //numéro du flag stop
//     int init_speed,            //vitesse pour l'initialisation
//     int min_speed,             //vitesse mini
//     int max_speed,             //vitesse maxi
//     double coef_Kp,            //PID coef proportionnel
//     double coef_Ki,            //PID coef integral
//     double coef_Kd,            //PID coef dérivée
//     double nb_tic_per_deg      // nombre de tic par tour pour calculer l'angle

    static const FirstName::argument firstName;
static const LastName::argument lastName;


static const Count_pin::argument count_pin;