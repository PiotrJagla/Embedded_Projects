.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.def    temp = r16  

rjmp    Init        

Init:    
   ser    temp         
   out    DDRB,temp   
                       
   ldi temp,0b11111110 
                       
   out    DDRD,temp    

   clr    temp         
   out    PortB,temp   
   ldi temp,0b00000001 
   out    PortD,temp   

Main:
   in    temp,PinD     
                       
   out    PortB,temp   
                       
   rjmp    Main        
