# hyper-cat

This is a code which uses the mqtt protocol to send files. It is possible to send either files or images using this protocol. 
This has the ability to subscribe to multiple topics and all the topics received will be store in the file system with unique 
and choronological names. The initialise funciton is used to initialse the number of files.

to compile:
  gcc -o object_name pubish.c -lpaho-mqtt3c
  gcc -o object_name subscribe.c -lpaho-mqtt3c
  gcc -o object_name initialise.c 
  
 paho-mqtt is library is required for comiling this code. 
 one has to create file for saving the number of files, in some directory in the file system. 
 
