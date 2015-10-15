SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);




char buffer[4][1000];
unsigned int bufferIndex = 0;   // cols index
unsigned char stateIndex = 0;   // rows index


String strRequestType;
String strHostname;
String strPath;

#include "HttpClient.h"
HttpClient http;
// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_request_t request;
http_response_t response;

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
                if(stateIndex > 3)
                {
                    Serial.println("Application>\tVersion: 0.0.4");
                    Serial.print("Application>\tRequest Type: ");
                    Serial.println(buffer[0]);
                    Serial.print("Application>\tHeader: ");
                    Serial.println(buffer[1]);
                    Serial.print("Application>\tHostname: ");
                    Serial.println(buffer[2]);
                    Serial.print("Application>\tPath: ");
                    Serial.println(buffer[3]);

                    request.hostname = String(buffer[2]);
                    request.port = 80;
                    request.path = String(buffer[3]);

                    // GET, POST, PUT, DELETE, PATCH
                    http.get(request, response, headers);
                    /*if(strRequestType == "get")
                    {
                        Serial.println("HTTPClient>\tSending GET request");
                        http.get(request, response, headers);
                    }
                    
                    if(strRequestType == "post")
                    {
                        Serial.println("HTTPClient>\tSending POST request");
                        http.post(request, response, headers);
                    }

                    if(strRequestType == "put")
                    {
                        Serial.println("HTTPClient>\tSending PUT request");
                        http.put(request, response, headers);
                    }

                    if(strRequestType == "delete")
                    {
                        Serial.println("HTTPClient>\tSending DELETE request");
                        http.del(request, response, headers);
                    }    

                    if(strRequestType == "patch")
                    {
                        Serial.println("HTTPClient>\tSending PATCH request");
                        http.patch(request, response, headers);
                    }   */

                    Serial.print("Application>\tResponse status: ");
                    Serial.println(response.status);

                    Serial.print("Application>\tHTTP Response Body: ");
                    Serial.println(response.body);

                    stateIndex = 0;
                }
            }
        }
    }

}