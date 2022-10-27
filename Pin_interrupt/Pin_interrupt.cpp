#include "Pin_interrupt.h" 

Pin_interrupt::Pin_interrupt(PinName pin , int num_compteur, Encoder &encoder) : _interrupt(pin,PullDown)          // create the InterruptIn on the pin specified to MyInterrupt
    {   
         if (num_compteur==1)
        {
           // attach increment function of this counter instance
            _interrupt.rise(callback(this, &Pin_interrupt::increment)); 
            _interrupt.fall(callback(this, &Pin_interrupt::fall_1)); 
             }
        if (num_compteur==2)
        {
           // attach increment function of this counter instance
            _interrupt.rise(callback(this, &Pin_interrupt::rise_2)); 
            _interrupt.fall(callback(this, &Pin_interrupt::fall_2)); 
             }
        
        _encoder   = &encoder; 
    }
/*principe:
il y a 2 compteurs 1 et 2
on dit compteur 1 rise = 0 / fall = 10
on dit compteur 2 rise = 21 / fall = 20

seul le compteur 1 rise augmente le compteur
dans le sens aller, on doit avoir la trame :  0 -> 21 -> 10 -> 20 =>  51
dans le sens retour, on doit avoir la trame : 0 -> 20 -> 10 -> 21 =>  51
*/

   void Pin_interrupt::fall_1()
{
      if (_encoder->tic_forward==21) {_encoder->tic_forward=31; };
      if (_encoder->tic_backward==20) {_encoder->tic_backward=30; };
}
void Pin_interrupt::fall_2()
{
   if (_encoder->tic_forward==31) {_encoder->tic_forward=51; };
   if (_encoder->tic_backward==0) {_encoder->tic_backward=20; };
     
}

void Pin_interrupt::rise_2()
{
   if (_encoder->tic_forward==0) {_encoder->tic_forward=21 ;};
   if (_encoder->tic_backward==30) {_encoder->tic_backward=51; };
     
}

void Pin_interrupt::increment()
{
      //on est arrivé a bout de la trame, on incremente le compteur
      if (_encoder->tic_forward==51){_encoder->count=_encoder->count+1;};
      if (_encoder->tic_backward==51){_encoder->count=_encoder->count-1;};
      
      //si le compteur a été incrémenté, on remet tout a zéro
      if (_encoder->tic_forward==51 || _encoder->tic_backward==51)
      {
            // met à jour l'angle du moteur
          //_angle = count / _nbtic_per_deg;
          //RAZ des tic
          _encoder->tic_forward = 0;
          _encoder->tic_backward = 0;  
      }
}
   
 
