#include "mbed.h"
#include "Accel/Accel.h" 
#include "Counter/Counter.cpp" 
#include <Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h>   
#include <PID/PID_v1.h>
#include "TCA9548/TCA9548.h"

//*********************** déclarations ***********************
    
      #define LED_PIN LED3 // the number of the LED pin 
      // CHANNEL VALUES
      #define CH0 0x01
      #define CH1 0x02
      #define CH2 0x04
      #define CH3 0x08
      #define CH4 0x10
      #define CH5 0x20 //carte PWM
      #define CH6 0x40 //accel_epaule
      #define CH7 0x80 //accel_coude + accel_poignet
#define I2C_SDA PB_7
#define I2C_SCL PB_6
    
      #define PIN_SENS_COUDE         0
      #define PIN_VITESSE_COUDE      1                       
      #define PIN_SENS_EPAULE        2
      #define PIN_VITESSE_EPAULE     3
      #define PIN_SENS_DOIGT1        6
      #define PIN_VITESSE_DOIGT1     7
      #define PIN_SENS_DOIGT2        8
      #define PIN_VITESSE_DOIGT2     9
      #define PIN_SENS_POIGNET       10
      #define PIN_VITESSE_POIGNET    11
      #define  pin_monte           PA_6        // pin monte
      #define  pin_descend         PA_7        // pin monte
      #define  pin_servo_pouce       12  //pin du servo du pouce
      #define   pin_ordre            5  //rouge
      #define   pin_status           7 //blanc 
      #define PIN_SENS_ROTATION_EPAULE        12
      #define PIN_VITESSE_ROTATION_EPAULE     13
      #define PIN_SENS_ROTATION_POIGNET       4
      #define PIN_VITESSE_ROTATION_POIGNET    5

 Counter counterA(PB_4);
 Counter counterB(PB_5);
 int nb_tours,counter,prev_counter; 

      I2C i2c(I2C_SDA , I2C_SCL); 
      Adafruit_PWMServoDriver carte_pwm  (&i2c,0x40) ;  //Carte d'extension 16 sorties pour le pilotage de servos en PWM  (adresse I2C par defaut 0x40)
       
      TCA9548 multiplexer_i2c(I2C_SDA, I2C_SCL );
      int calibation_epaule[3] =  {   1 , 1 , 1 };
      int calibation_coude[3] =  {   -1, -1, 1 };
      int calibation_poignet[3] = {  1, -1, 1};

      Accel accel_epaule( CH6,0x68,180,'X',calibation_epaule, 'XYZ' ); 
       
      Accel accel_coude( CH7,0x68,180,'X',calibation_coude,'XYZ');
      Accel accel_poignet( CH7,0x69,180,'X',calibation_poignet,'XZY');
      Accel accel_rot_poignet( CH7,0x69,180,'Z',calibation_poignet,'XZY');
      
      Serial pc(USBTX, USBRX);
      DigitalOut myled(LED_PIN);
   
      //PID aa
      //Define Variables we'll be connecting to
      double AngleCoude = 0, PrevAngleCoude = 0, PrevCmdeCoude = 0, CmdeCoude = 0, SetPointCoude = 0, AngleCoudeAbs =0;
      double AnglePoignet = 0, PrevAnglePoignet = 0,PrevCmdePoignet = 0, CmdePoignet = 0, SetPointPoignet = 0, AnglePoignetAbs =0;
      double AngleEpaule = 0, PrevAngleEpaule = 0,PrevCmdeEpaule = 0, CmdeEpaule = 0, SetPointEpaule = 0, AngleEpauleAbs =0;
      double AngleRotEpaule = 0, PrevCmdeRotEpaule = 0, CmdeRotEpaule = 0, SetPointRotEpaule = 0, AngleRotEpauleAbs =0;
      double AngleRotPoignet = 0, PrevAngleRotPoignet = 0,PrevCmdeRotPoignet = 0, CmdeRotPoignet = 0, SetPointRotPoignet = 0, AngleRotPoignetAbs =0;
      
      //PID PID_coude(&AngleCoude, &CmdeCoude, &SetPointCoude, 1, 0.1, 0.4, P_ON_E, DIRECT);
      PID PID_coude(&AngleCoude, &CmdeCoude, &SetPointCoude, 20, 0, 0, P_ON_E, DIRECT);
      PID PID_poignet(&AnglePoignet, &CmdePoignet, &SetPointPoignet, 30, 0, 0, P_ON_E, DIRECT);
      PID PID_epaule(&AngleEpaule, &CmdeEpaule, &SetPointEpaule, 100, 0, 0, P_ON_E, DIRECT);
      PID PID_RotEpaule(&AngleRotEpaule, &CmdeRotEpaule, &SetPointRotEpaule, 100, 0, 0, P_ON_E, DIRECT);
      PID PID_RotPoignet(&AngleRotPoignet, &CmdeRotPoignet, &SetPointRotPoignet, 100, 0, 0, P_ON_E, DIRECT);
      
      
      AnalogIn boutonMonte(pin_monte);
      AnalogIn boutonDescend(pin_descend);
       int val_bouton_monte =0, val_bouton_descend=0;

   //*********************** FIN déclarations ***********************

//*** ajuste le moteur en fonction de la commande
int ajuste_moteur(int pin_sens, int pin_vitesse , int cmde ){
 
     //pour eviter les petites oscillations, entre -100 et +100 le moteur est arreté

    if (cmde >=   -100 && cmde <=   100)
    {
        //pc.printf("stop -- ");
        carte_pwm.setPWM(pin_sens, 0,4096); //pour le sens on met 1
        carte_pwm.setPWM(pin_vitesse, 0,4096);
    }
    if (cmde >   100 )
    {
         //pc.printf("montee -- ");
         if ( cmde >   4096) { cmde =   4095; }
        carte_pwm.setPWM(pin_sens, 0,4096); //pour le sens on met 0
       carte_pwm.setPWM(pin_vitesse, 0,cmde);
    }
    if (cmde <  -100)
    {
          //pc.printf("descente -- ");
        if ( cmde <  -4096) { cmde =  -4095; }
        carte_pwm.setPWM(pin_sens, 4096,0); //pour le sens on met 1
        carte_pwm.setPWM(pin_vitesse, 0, -cmde);
    }
    
    return 1;
      }
 
//*** doigts
int ajuste_main(bool pOuvert){
    //Serial.print("ajuste_main" );  Serial.print("\t");
    if (pOuvert == true){
       
          ajuste_moteur(PIN_SENS_DOIGT1, PIN_VITESSE_DOIGT1, 4000  );
          ajuste_moteur(PIN_SENS_DOIGT2, PIN_VITESSE_DOIGT2, 4000 );

         

       
    }
    if (pOuvert == false){
       
          ajuste_moteur(PIN_SENS_DOIGT1, PIN_VITESSE_DOIGT1, -4000  );
          ajuste_moteur(PIN_SENS_DOIGT2, PIN_VITESSE_DOIGT2, -4000 );
          
 

    }
    return 1;
  }


  

void initial_setup(){
  // ================================================================
      // ===                      INITIAL SETUP                       ===
      // ================================================================ 
      pc.baud(9600); //serial begin 
      i2c.start();  

      //PWM begin
      //on selectionne le channel I2C
      multiplexer_i2c.select_I2C(CH5);
      carte_pwm.begin();
      carte_pwm.setPWMFreq(100); //permet au moteur de ne pas siffler


      accel_epaule.begin(); 
      pc.printf(accel_epaule.testConnection() ? "MPU6050 connection successful \n" : "MPU6050 connection failed \n");
     accel_epaule.setXAccelOffset(-1229);
     accel_epaule.setYAccelOffset(495);
     accel_epaule.setZAccelOffset(753);
      // AngleEpaule  = accel_epaule.lecture_angle();


      accel_coude.begin();
      pc.printf(accel_coude.testConnection() ? "MPU6050 connection successful \n" : "MPU6050 connection failed \n");
    accel_coude.setXAccelOffset((int16_t)-943);
     accel_coude.setYAccelOffset((int16_t)-707);
     accel_coude.setZAccelOffset((int16_t)1645);
     //AngleCoude   = accel_coude.lecture_angle() + AngleEpaule;

      accel_poignet.begin();
      pc.printf(accel_poignet.testConnection() ? "MPU6050 connection successful \n" : "MPU6050 connection failed \n");
       accel_poignet.setXAccelOffset((int16_t)1491);
     accel_poignet.setYAccelOffset((int16_t)-111);
     accel_poignet.setZAccelOffset((int16_t)507);
      // AnglePoignet = accel_poignet.lecture_angle(); 

      //	*******PID
      PID_coude.SetOutputLimits(-4095, 4095);
      PID_coude.SetMode(1);
      PID_poignet.SetOutputLimits(-4095, 4095);
      PID_poignet.SetMode(1);
      PID_epaule.SetOutputLimits(-4095, 4095);
      PID_epaule.SetMode(1); 
      PID_RotEpaule.SetOutputLimits(-4095, 4095);
      PID_RotEpaule.SetMode(1); 
      PID_RotPoignet.SetOutputLimits(-4095, 4095);
      PID_RotPoignet.SetMode(1); 


    nb_tours=0;
      // ================================================================
      // ===                   FIN INITIAL SETUP                       ===
      // ================================================================
 
}

int main() {

    initial_setup();
    pc.printf("AngleEpaule     AngleCoude     AnglePoignet  monte  descend   SetPointEpaule   CmdeEpaule     SetPointCoude   CmdeCoude       SetPointPoignet CmdePoignet\n" ); 
//*** Boucle principale
    while(1) {  
    

    //on lit les boutons, on ne peut pas avoir monté et descente en même temps
    val_bouton_monte = (int) boutonMonte.read_u16();
    val_bouton_descend =(int) boutonDescend.read_u16()  ; 
    if (val_bouton_monte > 40000){val_bouton_monte = 1;} else {val_bouton_monte=0;}//si il y a plus de 4V on met 1
    if (val_bouton_descend > 40000){val_bouton_descend = 1;val_bouton_monte=0;} else {val_bouton_descend=0;}//si il y a plus de 4V on met 1
    
       //on lit les accéléromètres 
        // pc.printf("AngleEpauleAbs   \n" ); 
        AngleEpauleAbs  =  accel_epaule.lecture_angle(); 
        AngleEpaule  =  AngleEpauleAbs; 

        // pc.printf("AngleCoudeAbs \n " );
        AngleCoudeAbs = accel_coude.lecture_angle(); 
        AngleCoude   =  180 + (AngleCoudeAbs - AngleEpauleAbs);
        if (AngleCoude>360){ AngleCoude = AngleCoude - 360;}

        // pc.printf("AnglePoignetAbs \n " );
        AnglePoignetAbs =   accel_poignet.lecture_angle();
        
        AnglePoignet =   180 + (AnglePoignetAbs -AngleCoudeAbs);
        if (AnglePoignet>360){ AnglePoignet = AnglePoignet - 360;}
        if (AnglePoignet<0){ AnglePoignet = AnglePoignet + 360;}

      AngleRotPoignetAbs  =  accel_rot_poignet.lecture_angle(); 
        AngleRotPoignet  =  AngleRotPoignetAbs; 

 //*************** Montée ***********************
    if (val_bouton_monte == 1) {
        counter = counterA.read();
        nb_tours = nb_tours + (counter - prev_counter);
        prev_counter = counter;
        

        // on définit l'objectif de l'épaule
        SetPointEpaule =  320;
        SetPointCoude =  110;
        SetPointPoignet =  210;
        //on calcul la commande//calcul le PID
        PID_epaule.Compute();
        PID_coude.Compute(); 
        PID_poignet.Compute();
        
        //pour le debug on affiche les données
        myled.write(true);
        
             //amortisseur au démarrage il ne doit pas y avoir d'accoups
        if (CmdeEpaule < PrevCmdeEpaule - 300 ){CmdeEpaule = PrevCmdeEpaule - 300; }; //commande epaule est negatif
        if (CmdeEpaule > PrevCmdeEpaule + 300 ){CmdeEpaule = PrevCmdeEpaule + 300; }; //commande epaule est positif
        if (CmdeCoude < PrevCmdeCoude - 100 ){CmdeCoude = PrevCmdeCoude - 100; }; //commande coude est negatif
        if (CmdeCoude > PrevCmdeCoude + 100 ){CmdeCoude = PrevCmdeCoude + 100; }; //commande coude est positif
        if (CmdePoignet < PrevCmdePoignet - 100 ){CmdePoignet = PrevCmdePoignet - 100; }; //commande  Poignet est negatif
        if (CmdePoignet > PrevCmdePoignet + 100 ){CmdePoignet = PrevCmdePoignet + 100; }; //commande  Poignet est positif

        
        
        //on selectionne le channel I2C et on pilote les moteurs
        multiplexer_i2c.select_I2C(CH5); 
        // ajuste_moteur(PIN_SENS_EPAULE,PIN_VITESSE_EPAULE, - CmdeEpaule   ) ; // on ajuste epaule
        // ajuste_moteur(PIN_SENS_COUDE,PIN_VITESSE_COUDE,  CmdeCoude    ) ;  // on ajuste le coude
        // ajuste_moteur(PIN_SENS_POIGNET,PIN_VITESSE_POIGNET, - CmdePoignet   ) ;  // on ajuste le poignet
        // ajuste_moteur(PIN_SENS_ROTATION_EPAULE,PIN_VITESSE_ROTATION_EPAULE, 1400   ) ;  // on ajuste la rotation de l'épaule
        ajuste_moteur(PIN_SENS_ROTATION_POIGNET,PIN_VITESSE_ROTATION_POIGNET, 500   ) ;  // on ajuste la rotation du poignet
        
    }
//*************** Descente ***********************
 if (val_bouton_descend == 1) {
        counter = counterA.read();
        nb_tours = nb_tours - (counter - prev_counter);
        prev_counter = counter;
        // on définit l'objectif de l'épaule
        SetPointEpaule =  40;
        SetPointCoude =  220;
        SetPointPoignet =  130;
        //on calcul la commande
        PID_epaule.Compute();
        PID_coude.Compute(); //calcul le PID
        PID_poignet.Compute();
        
        //pour le debug on affiche les données
        myled.write(true);
               //amortisseur au démarrage il ne doit pas y avoir d'accoups
        if (CmdeEpaule < PrevCmdeEpaule - 300 ){CmdeEpaule = PrevCmdeEpaule - 300; }; //commande epaule est negatif
        if (CmdeEpaule > PrevCmdeEpaule + 300 ){CmdeEpaule = PrevCmdeEpaule + 300; }; //commande epaule est positif
        if (CmdeCoude < PrevCmdeCoude - 100 ){CmdeCoude = PrevCmdeCoude - 100; }; //commande coude est negatif
        if (CmdeCoude > PrevCmdeCoude + 100 ){CmdeCoude = PrevCmdeCoude + 100; }; //commande coude est positif
        if (CmdePoignet < PrevCmdePoignet - 100 ){CmdePoignet = PrevCmdePoignet - 100; }; //commande  Poignet est negatif
        if (CmdePoignet > PrevCmdePoignet + 100 ){CmdePoignet = PrevCmdePoignet + 100; }; //commande  Poignet est positif

        //on selectionne le channel I2C et on pilote les moteurs
        multiplexer_i2c.select_I2C(CH5);
      //  ajuste_moteur(PIN_SENS_EPAULE,PIN_VITESSE_EPAULE, - CmdeEpaule    ) ; // on ajuste epaule
      //  ajuste_moteur(PIN_SENS_COUDE,PIN_VITESSE_COUDE,  CmdeCoude   ) ;  // on ajuste le coude
      //  ajuste_moteur(PIN_SENS_POIGNET,PIN_VITESSE_POIGNET, - CmdePoignet   ) ;  // on ajuste le poignet
      //  ajuste_moteur(PIN_SENS_ROTATION_EPAULE,PIN_VITESSE_ROTATION_EPAULE, -1400   ) ;  // on ajuste la rotation de l'épaule  
      ajuste_moteur(PIN_SENS_ROTATION_POIGNET,PIN_VITESSE_ROTATION_POIGNET, -500   ) ;  // on ajuste la rotation du poignet
    }
//*************** STOP ***********************
   //Quand on lache le bouton, on arrete tout
    if(val_bouton_descend==0 && val_bouton_monte ==0){
      
      myled.write(false);
      
      CmdeEpaule=0;
      CmdeCoude=0;
      CmdePoignet=0;
      //on selectionne le channel I2C et on pilote les moteurs
      multiplexer_i2c.select_I2C(CH5);
      ajuste_moteur(PIN_SENS_EPAULE,PIN_VITESSE_EPAULE, CmdeEpaule  ) ; 
      ajuste_moteur(PIN_SENS_COUDE,PIN_VITESSE_COUDE,   CmdeCoude   ) ;  // on ajuste le coude
      ajuste_moteur(PIN_SENS_POIGNET,PIN_VITESSE_POIGNET,  CmdePoignet   ) ;  // on ajuste le poignet
      ajuste_moteur(PIN_SENS_ROTATION_EPAULE,PIN_VITESSE_ROTATION_EPAULE, 0   ) ;  // on ajuste la rotation de l'épaule
      ajuste_moteur(PIN_SENS_ROTATION_POIGNET,PIN_VITESSE_ROTATION_POIGNET, 0   ) ;  // on ajuste la rotation du poignet
    }
    else
    {
     pc.printf("%i         ", counterA.read() );
     pc.printf("%i         ", nb_tours );
     pc.printf("%f      ", AngleRotPoignet );
     pc.printf("%f      ", AngleRotEpaule );
     pc.printf("%f      ", AngleEpaule );
     pc.printf("%f      ", AngleCoude );
     pc.printf("%f      ", AnglePoignet );
     pc.printf("%i      ", val_bouton_monte);
     pc.printf("%i      ", val_bouton_descend);
     pc.printf("%f      ", SetPointEpaule);
     pc.printf("%f      ", CmdeEpaule);
     pc.printf("%f      ", SetPointCoude);
     pc.printf("%f      ", CmdeCoude);
     pc.printf("%f      ", SetPointPoignet);
     pc.printf("%f      ", CmdePoignet); 
     pc.printf("\n" ); 
    //  pc.printf("Count so far: %d\n", counterA.read());
    //  pc.printf("Count so far: %d\n", counterB.read());

    }
    
    
    PrevCmdeEpaule = CmdeEpaule;
    PrevCmdeCoude = CmdeCoude;
    PrevCmdePoignet = CmdePoignet;
   
 
    }
 pc.printf("on sort du programme... \n");
}

