#include <iostream>
#include<cstdlib>
#include <string>
#include<fstream>
#include "../include/wether_api.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;



void logError(const std::string & message) {
    std::ofstream errorLog("weather_errors.log", std::ios_base::app);
    if(errorLog.is_open()) {
        errorLog <<"Error" << message << std::endl;
        errorLog.close();
    } 
}
void parseWetherData(const json & weatherJson){
    try{
        double temprature =weatherJson.value("main.temp",0.0);
        int humidity = weatherJson.value("main.humidity", 0);
        double windSpeed = weatherJson.value("wind.speed", 0.0);

        if(weatherJson.contains("main") && weatherJson["main"].contains("temp")){
            temprature = weatherJson["main"]["temp"].get<double>();
        }else{
            logError("Temperature data not found in JSON response.");
        }

    }catch (const json::parse_error& e){
        logError("JSON parsing error: " + std::string(e.what()));
    }catch (const json::type_error& e) {
        // Handle type errors (e.g., expecting object but got array)
        logError("JSON type error: " + std::string(e.what()));
    } catch (const json::out_of_range& e) {
        // Handle out of range errors (e.g., accessing non-existent field)
        logError("JSON out of range error: " + std::string(e.what()));
    }
}



Wetherdata parseWetherData(const std::string & jsonResponse) {
    Wetherdata data;
    try {
        json j = json::parse(jsonResponse);
        data.temperature = j["main"]["temp"].get<double>();
        data.humidity = j["main"]["humidity"].get<double>();
        data.windSpeed = j["wind"]["speed"].get<double>();
        data.description = j["weather"][0]["description"].get<std::string>();
    }
    
    catch (const nlohmann::json:: parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
    catch (const nlohmann::json::out_of_range& e) {
        std::cerr << "JSON out of range error: " << e.what() << std::endl;
    }
    catch (const nlohmann::json::invalid_iterator& e) {
        std::cerr << "General exception: " << e.what() << std::endl;
    }
    return data;
}

