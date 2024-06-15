#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Nagarjuna";
const char* password = "123456789";

int LED = 2; // led connected to D0
WiFiServer server(80);

void setup()
{
 Serial.begin(115200);
 pinMode(LED, OUTPUT);
 digitalWrite(LED, LOW);

 Serial.print("Connecting to the Newtork");
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED)
 {
   delay(500);
   Serial.print(".");
 }
 Serial.println("WiFi connected");
 server.begin();  // Starts the Server
 Serial.println("Server started");

 Serial.print("IP Address of network: ");
 Serial.println(WiFi.localIP());
 Serial.println("/");
}

void loop()
{
 WiFiClient client = server.available();
 if (!client)
 {
   return;
 }
 Serial.println("Waiting for new client");
 while(!client.available())
 {
   delay(1);
 }

 String request = client.readStringUntil('\r');
 Serial.println(request);
 client.flush();

 int value = LOW;
 if(request.indexOf("/LED=ON") != -1)
 {
   digitalWrite(LED, HIGH); // Turn ON LED
   value = HIGH;
   
 }
 if(request.indexOf("/LED=OFF") != -1)
 {
   digitalWrite(LED, LOW); // Turn OFF LED
   value = LOW;
   Weather_calling();
 }


 client.println("HTTP/1.1 200 OK"); // standalone web server with an ESP8266
 client.println("Content-Type: text/html");
 client.println("");
 client.println("<!DOCTYPE HTML>");
 client.println("<html>");

 client.print("LED: ");

 if(value == HIGH)
 {
   client.print("ON");
 }
 else
 {
   client.print("OFF");
 }
 client.println("<br><br>");
 client.println("<a href=\"/LED=ON\"\"><button>ON</button></a>");
 client.println("<a href=\"/LED=OFF\"\"><button>OFF</button></a><br />");
 client.println("</html>");

 delay(1);
 Serial.println("Client disonnected");
 Serial.println("");

    
}


void Weather_calling()
{
  if (WiFi.status() == WL_CONNECTED) 
  { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://api.weatherapi.com/v1/current.json?key=08ac3bf21ce7464a89461528240106&q=Bangalore&aqi=no");  //Specify request destination
    int httpCode = http.GET();                                  //Send the request
 
    if (httpCode > 0) 
    { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload
              // Stream& input;
     JsonDocument doc;

     DeserializationError error = deserializeJson(doc, payload);

      if (error) 
       {
         Serial.print("deserializeJson() failed: ");
         Serial.println(error.c_str());
         return;
       }

       JsonObject location = doc["location"];
       const char* location_name = location["name"]; // "Bangalore"
       Serial.print("This is location:");
       Serial.println(location_name);
       const char* location_region = location["region"]; // "Karnataka"
       Serial.print("This is location_region:");
       Serial.println(location_region);
       const char* location_country = location["country"]; // "India"
       Serial.print("This is location_country:");
       Serial.println(location_country);
       float location_lat = location["lat"]; // 12.98
        Serial.print("This is location_lat:");
       Serial.println(location_lat);
       float location_lon = location["lon"]; // 77.58
          Serial.print("location_lon:");
       Serial.println(location_lon);
       const char* location_tz_id = location["tz_id"]; // "Asia/Kolkata"
          Serial.print("location_tz_id:");
       Serial.println(location_tz_id);
       long location_localtime_epoch = location["localtime_epoch"]; // 1717223635
          Serial.print("location_localtime_epoch:");
       Serial.println(location_localtime_epoch);
       const char* location_localtime = location["localtime"]; // "2024-06-01 12:03"
          Serial.print("location_localtime:");
       Serial.println(location_localtime);

       JsonObject current = doc["current"];
       long current_last_updated_epoch = current["last_updated_epoch"]; // 1717223400
          Serial.print("current_last_updated_epoch:");
       Serial.println(current_last_updated_epoch);
       const char* current_last_updated = current["last_updated"]; // "2024-06-01 12:00" 
          Serial.print("current_last_updated:");
       Serial.println(current_last_updated);
       int current_temp_c = current["temp_c"]; // 30
         Serial.print("current_temp_c:");
       Serial.println(current_temp_c);
       int current_temp_f = current["temp_f"]; // 86
          Serial.print("current_temp_f:");
       Serial.println(current_temp_f);
       int current_is_day = current["is_day"]; // 1
         Serial.print("current_is_day:");
       Serial.println(current_is_day);

       JsonObject current_condition = current["condition"];
       const char* current_condition_text = current_condition["text"]; // "Partly cloudy"
          Serial.print("current_condition_text:");
       Serial.println(current_condition_text);
       const char* current_condition_icon = current_condition["icon"];
          Serial.print("current_condition_icon:");
       Serial.println(current_condition_icon);
       int current_condition_code = current_condition["code"]; // 1003
          Serial.print("current_condition_code:");
       Serial.println(current_condition_code);

       float current_wind_mph = current["wind_mph"]; // 9.4
         Serial.print("current_wind_mph:");
       Serial.println(current_wind_mph);
       float current_wind_kph = current["wind_kph"]; // 15.1
          Serial.print("current_wind_kph:");
       Serial.println(current_wind_kph);
       int current_wind_degree = current["wind_degree"]; // 300
         Serial.print("current_wind_degree:");
       Serial.println(current_wind_degree);
       const char* current_wind_dir = current["wind_dir"]; // "WNW"
          Serial.print("current_wind_dir:");
       Serial.println(current_wind_dir);
       int current_pressure_mb = current["pressure_mb"]; // 1012
          Serial.print("current_pressure_mb:");
       Serial.println(current_pressure_mb);
       float current_pressure_in = current["pressure_in"]; // 29.88
         Serial.print("current_pressure_in:");
       Serial.println(current_pressure_in);
       int current_precip_mm = current["precip_mm"]; // 0
         Serial.print("current_precip_mm:");
       Serial.println(current_precip_mm);
       int current_precip_in = current["precip_in"]; // 0
          Serial.print("current_precip_in:");
       Serial.println(current_precip_in);
       int current_humidity = current["humidity"]; // 59
          Serial.print("current_humidity:");
       Serial.println(current_humidity);
       int current_cloud = current["cloud"]; // 25
          Serial.print("current_cloud:");
       Serial.println(current_cloud);
       float current_feelslike_c = current["feelslike_c"]; // 30.3
          Serial.print("current_feelslike_c:");
       Serial.println(current_feelslike_c);
       float current_feelslike_f = current["feelslike_f"]; // 86.5
          Serial.print("current_feelslike_f:");
       Serial.println(current_feelslike_f);
       int current_windchill_c = current["windchill_c"]; // 33
          Serial.print("current_windchill_c:");
       Serial.println(current_windchill_c);
       float current_windchill_f = current["windchill_f"]; // 91.5
         Serial.print("current_windchill_f:");
       Serial.println(current_windchill_f);
       float current_heatindex_c = current["heatindex_c"]; // 34.8
         Serial.print("current_heatindex_c:");
       Serial.println(current_heatindex_c);
       float current_heatindex_f = current["heatindex_f"]; // 94.6
         Serial.print("current_heatindex_f:");
       Serial.println(current_heatindex_f);
       float current_dewpoint_c = current["dewpoint_c"]; // 18.1
         Serial.print("current_dewpoint_c:");
       Serial.println(current_dewpoint_c);
       float current_dewpoint_f = current["dewpoint_f"]; // 64.6
          Serial.print("current_dewpoint_f:");
       Serial.println(current_dewpoint_f);
       int current_vis_km = current["vis_km"]; // 8
         Serial.print("current_vis_km:");
       Serial.println(current_vis_km);
       int current_vis_miles = current["vis_miles"]; // 4
          Serial.print("current_vis_miles:");
       Serial.println(current_vis_miles);
       int current_uv = current["uv"]; // 8
          Serial.print("current_uv:");
       Serial.println(current_uv);
       float current_gust_mph = current["gust_mph"]; // 13.9
         Serial.print("current_gust_mph:");
       Serial.println(current_gust_mph);
       float current_gust_kph = current["gust_kph"]; // 22.3
          Serial.print("current_gust_kph:");
       Serial.println(current_gust_kph);   
    }
  
    http.end();   //Close connection
 
  }
}
