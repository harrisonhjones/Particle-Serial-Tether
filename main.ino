SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);




char buffer[5][1000];
unsigned int bufferIndex = 0;   // cols index
unsigned char stateIndex = 0;   // rows index


String strRequestType;
String strHostname;
String strPath;

// SYNTAX
TCPClient client;

void setup()
{

    // Setup a serial connection
    Serial.begin(9600);

    // Turn on WiFi
    WiFi.on();

    // Connect to WiFi
    WiFi.connect();

    while(WiFi.ready() != true)
    {}

    Serial.println("VERSION: 0.0.1");
    Serial.println("AUTHOR: Harrison Jones (harrison@hhj.me)");
    Serial.println("READY");

}

void loop()
{
    if (Serial.available() > 0) 
    {
        // read the incoming byte:
        int incomingByte = Serial.read();

        if (incomingByte > 0)
        {
            if((incomingByte != 10) && (incomingByte != 13)) // Line feed (10) or carriage return (13)
            {
                buffer[stateIndex][bufferIndex++] = incomingByte;
            }
            else
            {
                buffer[stateIndex++][bufferIndex] = 0;  // Null terminiate the string; also increment the rows index
                bufferIndex = 0;                        // Reset the cols index
                if(stateIndex > 4)
                {
                    Serial.println("Application>\tVersion: 0.0.17");
                    Serial.print("Application>\tRequest Type: ");
                    Serial.println(buffer[0]);
                    Serial.print("Application>\tHostname: ");
                    Serial.println(buffer[1]);
                    Serial.print("Application>\tPath: ");
                    Serial.println(buffer[2]);
                    Serial.print("Application>\tHeader: ");
                    Serial.println(buffer[3]);
                    Serial.print("Application>\tBody: ");
                    Serial.println(buffer[4]);

                    Serial.println("Application>\tConnecting...");
                    if (client.connect(buffer[1], 80))
                    {
                        Serial.println("Application>\t\tConnected");                        
                        // REQUEST_TYPE PATH HTTP/1.0
                        client.print(buffer[0]);
                        client.print(" ");
                        client.print(buffer[2]);
                        client.println(" HTTP/1.0");

                        // Host: HOST
                        client.print("Host: ");
                        client.println(buffer[1]);

                        if(buffer[3][0] != 0)   // If it isn't empty
                        {
                            Serial.println("Application>\t\tSending special headers");
                            client.println(buffer[3]);
                        }

                        
                        client.println("X-Bot: Particle Serial Proxy 0.0.17");
                        client.println("X-Bot-Author: Harrison Jones (harrison@hhj.me)");
                        
                        

                        if(buffer[4][0] != 0)   // If it isn't empty
                        {
                            // Calc true context length:
                            int cLength = 0;
                            while(buffer[4][cLength++] != 0)
                            {}
                            cLength--;

                            client.println("Content-Type: application/x-www-form-urlencoded");

                            client.println("Content-Length: ");
                            client.println(cLength);

                            client.println();
                            Serial.print("Application>\t\tSending special body (length = ");
                            Serial.print(cLength);
                            Serial.println(")");

                            client.println(buffer[4]);

                            client.println();
                        }
                        else
                        {
                            client.println("Content-Length: 0");
                            client.println();
                        }



                        Serial.println("Application>\t\tData sent");
                    }
                    else
                    {
                        Serial.print("Application>\t\tError\tUnable to connect"); 
                    }

                    stateIndex = 0;
                }
            }
        }
    }

    if (client.available())
    {
        char c = client.read();
        Serial.print(c);
    }

}