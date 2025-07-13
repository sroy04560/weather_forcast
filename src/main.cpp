#include <iostream>
#include<cstdlib>
#include <string>
#include "wether_api.cpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include<iomanip>
#include "errorHandle.cpp"
#include <thread>
#include <chrono>
using json = nlohmann::json;



void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

const int MAX_RETRIES = 3;
// Initial delay between retries in seconds
const int INITIAL_DELAY = 1;
// Multiplier for exponential backoff
const int BACKOFF_MULTIPLIER = 2;


size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
std::string contruct_api_url(const std::string & location,const std::string &key){
    std::string base_url="http://api.openweathermap.org/data/2.5/weather?q=";

    std::string url=base_url+location + "&units=metric&appid=" +key;
    return url; 
}

std:: string formatWeatherData(const Wetherdata& data, const std::string& location){
      clearScreen();
const std::string COLOR_RESET = "\033[0m";
const std::string COLOR_CYAN = "\033[36m";
const std::string COLOR_YELLOW = "\033[33m";
const std::string COLOR_BLUE = "\033[34m";
std::ostringstream oss;

std::cout << COLOR_CYAN << "****************************************************" << COLOR_RESET << std::endl;
std::cout << COLOR_CYAN << "*               WEATHER INFORMATION              *" << COLOR_RESET << std::endl;
std::cout << COLOR_CYAN << "****************************************************" << COLOR_RESET << std::endl;

oss << COLOR_YELLOW  <<"Weather in :- "  << location <<COLOR_RESET  <<"\n";
std::cout << "* Date: " << COLOR_YELLOW << __DATE__ << COLOR_RESET << std::endl;
std::cout << "* Time: " << COLOR_YELLOW << __TIME__ << COLOR_RESET << std::endl;

std::cout << COLOR_CYAN << "****************************************************" << COLOR_RESET << std::endl;
//temperature
oss<< COLOR_BLUE << std::setw(20) << std::left << "Temperature:" << COLOR_RESET;
oss <<" 🌡️   temperature: "<<std::fixed<<std::setprecision(1)<< data.temperature<<"°C\n";

//feels like
oss<< COLOR_BLUE << std::setw(20) << std::left << "Feels Like:" << COLOR_RESET;
oss <<" 🥵   feels like: "<<std::fixed<<std::setprecision(1)<< data.feels_like<<"°C\n";

//clouds
oss<< COLOR_BLUE << std::setw(20) << std::left << "Clouds:" << COLOR_RESET;
oss <<" ☁️   clouds: "<<std::fixed<<std::setprecision(1)<< data.clouds<<"%\n";

//humidity
oss<< COLOR_BLUE << std::setw(20) << std::left << "Humidity:" << COLOR_RESET;
oss <<" 💧 Humidity: "<<std::fixed<<std::setprecision(1)<<data.humidity<<"%\n";

//wind speed
oss<< COLOR_BLUE << std::setw(20) << std::left << "Wind Speed:" << COLOR_RESET;
oss <<" 🎐 Wind Speed: "<<std::fixed<<std::setprecision(1)<<data.windSpeed<<" m/s\n";

//last update
oss<< COLOR_BLUE << std::setw(20) << std::left << "Last Update:" << COLOR_RESET;
oss <<"❕Last Update: "<<data.description<<"\n";

std::cout << COLOR_CYAN << "****************************************************" << COLOR_RESET << std::endl;


oss<< COLOR_BLUE << std::setw(20) << std::left << "Pressure:" << COLOR_RESET;
oss<< "🗜️  Pressure: " << COLOR_YELLOW << data.pressure << COLOR_RESET << std::endl;
oss<< COLOR_BLUE << std::setw(20) << std::left << "Sea Level:" << COLOR_RESET;
oss << "🌊 Sea Level: " << COLOR_YELLOW << data.sea_level << COLOR_RESET << std::endl;

oss << COLOR_CYAN << "****************************************************" << COLOR_RESET << std::endl;

return oss.str();

}


int main() {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    int retryCount = 0;
    std::string location ;
    
    

    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        
        std::cout<< "Enter the location for weather forecast: ";
        // Prompt the user to enter a location for the weather forecast.
        std::getline(std::cin, location);
        std::string url =  contruct_api_url(location, getenv("WETHER_API_KEY"));
        // std::cout << "Constructed URL: " << url << std::endl;
    
    
        // Set the URL for the cURL request
        // This is the OpenWeatherMap API endpoint for current weather data.
        // The URL includes the city name and the API key.
        // The API key is retrieved from the environment variable WETHER_API_KEY.
        // The city is set to "London" and the units are set to metric.
        // The URL is constructed as a string and passed to cURL.
        // The cURL library will use this URL to make the HTTP GET request.
        // The response will be written to the readBuffer string.
        // The cURL library is used to perform the HTTP request and handle the response.

        // Initialize cURL and set the URL for the request.
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
        // Tells cURL to use WriteCallback function handle incoming data from the server.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);  
        // Pass the readBuffer string to cURL to store the response data.
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        curl_easy_setopt(curl, CURLOPT_USERAGENT, "WeatherProgram/1.0");

        try{
            res = curl_easy_perform(curl);
            if(res != CURLE_OK){
            handleWeatherError(res);
            retryCount++;
            if(retryCount < MAX_RETRIES) {
                int delay=INITIAL_DELAY* pow(BACKOFF_MULTIPLIER, retryCount);
                std::this_thread::sleep_for(std::chrono::seconds(delay));
            }
        
        }
            // std::cout<<"Api call successful"<<std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            logError("Exception caught: " + std::string(e.what()));
        }
        
        
        // Cleanup cURL resources
        // After the cURL request is complete, we clean up the cURL handle to free
        // any resources that were allocated during the request.
        // This is important to prevent memory leaks and ensure that the cURL library
        // can be used for future requests without issues.

    
        std::cout << formatWeatherData(parseWetherData(readBuffer),location) << std::endl;
        

        // data.name = WetherJson.value("name", "");
    
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();

    // std::cout << readBuffer << std::endl;
    return 0;
}