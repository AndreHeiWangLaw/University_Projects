K = 5.0; 
B = 0.002;
J = 0.0043;

s = tf('s');
dcm_s = K/(J*s+B);
dcm_p = dcm_s/s;


ltiview('step', dcm_s, 0:0.1:10);


ltiview('step',dcm_p, 0:0.1:10);