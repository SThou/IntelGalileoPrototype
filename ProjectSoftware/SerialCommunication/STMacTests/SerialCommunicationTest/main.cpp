//
//  main.cpp
//  SerialCommunicationTest
//
//  Created by ST on 11/19/14.
//  Copyright (c) 2014 ST. All rights reserved.
//
//  based on works from soerenbnoergaard
//
// Note: subtracting - '0' from char casts into an int value
//       chars are 1 byte, if considered as short they are 2 bytes
//

#include <iostream>
#include <boost/asio.hpp>
#include "SimpleSerial.h"

#define Serial_Port "/dev/tty.usbmodemfa141"

using namespace std;
using namespace::boost::asio;

// serial settings
serial_port_base::baud_rate BAUD(9600);
serial_port_base::flow_control  FLOW(serial_port_base::flow_control::none);
serial_port_base::parity PARITY( serial_port_base::parity::none);
serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one);




int main(){
    
    io_service io;
    serial_port port(io, Serial_Port);
    
    // setup port
    port.set_option(BAUD);
    port.set_option(FLOW);
    port.set_option(PARITY);
    port.set_option(STOP);
    
    /*
    if(port.is_open()){ // simple handshake
        try{
            char buffered[2];
            buffered[0] = 'A';
            buffered[1] = 'B';
           
            
            write(port, buffer(buffered, 1));
          //  write(port, buffer(buffered, 2));
           // write(port, buffer(buffered, 3));
            
        } // end try

        catch(boost::system::system_error& e)
        {
            cout<<"Error: "<<e.what()<<endl;
            return 1;
        }
        
        
    }
    else cout<<"Port is closed" << endl;
    // end simple handshake
    
    */
    
    
    unsigned char input;
    char c;
    
    if(port.is_open()){
        
        while(1){
            cin>> input;
            
            // Output buffer
            unsigned char command[2] = {0};
            
            // convert and send
            command[0] = static_cast<unsigned char>(input);
            write(port, buffer(command, 1) );
            
            // read response
            read(port, buffer(&c, 1)); // read blocks until it has something
            cout<<"Read: "<<c<<endl;
            
        } // end while loop
    } // end if
    
    
} // end main