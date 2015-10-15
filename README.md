#Particle-Serial-Tether

##Description
A project to allow you to "tether" using your Particle device (Spark Core, Photon, Electron, etc) over serial. You send serial commands, you get raw http responses in return.

##Usage
Format is simple. Follow these steps:

1. Connect to your Particle device which you have already flashed over serial. 9600 baud
1. Send the request type (GET, POST, ETC) and then a line feed or carriage return
1. Send the hostname (`google.com`, `particle.io`, etc) and then a line feed or carriage return. Do **not** send `http://`
1. Send the path (`/`, `/q=ParticleIO`) and then a line feed or carriage return
1. Send the desired headers (`Foo: bar`, `Test: Yep!`, etc) and then a line feed or carriage return. Cannot currently send multiple headers (can't send line feeds since they indicate the end of the input). Not required; can simply enter nothing and then a line feed or carriage return
1. Send the desired bdoy (not supported at the moment) and then a line feed or carriage return. Not required; can simply enter nothing and then a line feed or carriage return

In return you will get some debug info (always starts with `Application>`) and then the http response from the server

##ToDo

1. Get HTTP responses with bodies working
1. Write a local http proxy
1. Remove the debug serial responses