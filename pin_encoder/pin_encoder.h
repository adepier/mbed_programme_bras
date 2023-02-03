#include "mbed.h" 
struct  encoder_values 
{  
 int  tic_forward =0 ;
 int  tic_backward=0 ;   
 int count = 0;
};

class pin_encoder {
public:
    pin_encoder(PinName pin , int num_compteur, encoder_values &encoder)  ;
  void  fall_1();
  void  fall_2();
  void  rise_2();
  void  increment(); 




private:
    InterruptIn _interrupt; 
    encoder_values * _encoder ;  
};
