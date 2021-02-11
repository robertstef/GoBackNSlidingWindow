**<ins>Go Back N Sliding Window</ins>**

This program is an implementation of a go-back-n sliding window protocol. The program contains additional simulation features that allow the user to input messages to the sender, simulating incoming packets from another process or machine, and two features in the receiver to simulate message corruption/loss and acknowledgement corruption/loss. To simulate message corruption/loss in the receiver the user will be prompted to keep or discard the incoming packet. The simulate acknowledgement loss the user is required to enter the probability of acknowledgement loss when starting the program. During execution acknowledgements will be sent of discarded based on the user inputted probability.

The program has been written to run on a Linux machine and is to be executed using the instructions given below. For detailed description of the implementation please see the document *design.md*.

**<ins>Execution instructions</ins>**

To execute the program, run the following commands on two different terminal windows, or on two separate machines.

1. `receiver p_ack`
   
   - `p_ack`: An integer from 0 to 100 indicating the probability an acknowledgement is corrupted/lost. 


2. `sender IP port wsize timeout`

   - `IP`: The IP address of the receiver's machine

   - `port`: The port number the receiver is listening on
   
     > Note: the receiver is hard coded to listen on port **30000**

   - `wsize`: Size of the sending window. Must be an integer greater than 0.
   
   - `timeout`: The timeout value for the sending window. Must be an integer greater than 0.


3. Use ^C to exit both programs when finished.
