Robert Stefanyshin
ras867
11137532
CMPT434
A2

Q1:

To execute question 1 execute the following commands in the following order
on two different terminal windows, or on two separate machines.

1. receiver p_ack
   
   p_ack: An integer from 0 to 100. Indicates the probabily an ack
          will not be sent to the sender. Note - to run the program
          with all acks being sent, enter 0 for p_ack.


2. sender IP port wsize timeout

   IP: The IP address of the receiver's machine
   port: The port number the receiver is listening on
         **Use port 30000**

   wsize: Size of the sending window. Must be an integer
          greater than 0.

   timeout: The timeout value for the sending window. Must
            be an integer greater than 0.
 

Use ^C to exit both programs.

Q2:

To execute question 2 execute the following commands in the following order
on three different terminal windows, or on three separate machines.

1. receiver p_ack
   
   p_ack: An integer from 0 to 100. Indicates the probabily an ack
          will not be sent to the sender. Note - to run the program
          with all acks being sent, enter 0 for p_ack.

2. forwarder IP port wsize timeout

   IP: The IP address of the receiver's machine
   port: The port number the receiver is listening on
         **Use port 30000**

   wsize: Size of the sending window. Must be an integer
          greater than 0.

   timeout: The timeout value for the sending window. Must
            be an integer greater than 0.

3. sender IP port wsize timeout

   IP: The IP address of the forwarder's machine
   port: The port number the receiver is listening on
         **Use port 35000**

   wsize: Size of the sending window. Must be an integer
          greater than 0.

   timeout: The timeout value for the sending window. Must
            be an integer greater than 0.

Use ^C to exit each program.

NOTE: after starting the program, for both question 1 and question 2,
      the user will be prompted with clear instructions for how to
      use the simulation.
