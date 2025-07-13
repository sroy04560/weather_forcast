#include<iostream>
#include<fstream>
#include <curl/curl.h>




void handleWeatherError(int errorCode){
    switch(errorCode) {
        case CURLE_COULDNT_CONNECT:
            std::cerr << "Error: Could not connect to the weather service. "
                      << "Please check your internet connection and try again." << std::endl;
            break;
        case 404:
            std::cerr << "Error: Location not found. Please check the location name." << std::endl;
            break;
        case 401:
            std::cerr << "Error: Unauthorized access. Please check your API key." << std::endl;
            break;
        case 500:
            std::cerr << "Error: Internal server error. Please try again later." << std::endl;
            break;
        default:
            std::cerr << "Error: An unexpected error occurred. Error code: " << errorCode << std::endl;
    }
    std::ofstream logError("error.log", std::ios_base::app);
    if(logError.is_open()) {
        logError << "Error code: " << errorCode << " - " << __TIMESTAMP__<< std::endl;
        logError.close();
    }

}