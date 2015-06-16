function test_models

A = [0  1
     0  -0.271e-3];
B = [0
      1];
C = [1 0];
D = 0;
mymodel = ss(A,B,C,D);

step(mymodel, 4000)

end