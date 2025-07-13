# Weather Forecast CLI

A C++ command-line application that fetches and displays current weather information for any city using the OpenWeatherMap API.  
It uses [libcurl](https://curl.se/libcurl/) for HTTP requests and [nlohmann/json](https://github.com/nlohmann/json) for JSON parsing.

## Features

- Fetches live weather data for any city
- Displays temperature, humidity, wind speed, clouds, pressure, sea level, and more
- Colorful and emoji-enhanced terminal output
- Handles API errors and retries with exponential backoff
- Logs errors to `weather_errors.log`

## Requirements

- C++17 or later
- [libcurl](https://curl.se/libcurl/) development files
- [nlohmann/json](https://github.com/nlohmann/json) (included via CMake or manually)
- CMake (recommended for building)

## Setup

1. **Clone the repository:**
   ```sh
   git clone https://github.com/nlohmann/json.git
   GIT_TAG   v3.11.0
   cd wether_forcast
   ```

2. **Set your OpenWeatherMap API key:**
   ```sh
   export WETHER_API_KEY=your_api_key_here
   ```

3. **Build the project (recommended):**

   Create CMakeList.txt
   ``````
   add_executable(wether_forcast src/wether_api.cpp)
   target_link_libraries(wether_forcast  PRIVATE nlohmann_json::nlohmann_json)
   
   ```````
   ```sh
   cmake -S . -B build
   cmake --build build
   ```

4. **Run the program:**
   ```sh
   ./build/wether_forcast
   ```

## Usage

- Enter the city name when prompted.
- View the formatted weather information in your terminal.

## Example Output

```
****************************************************
*               WEATHER INFORMATION              *
****************************************************
Weather in :- London
* Date: Jul 13 2025
* Time: 12:34:56
****************************************************
 🌡️   temperature: 19.7°C
 🥵   feels like: 19.4°C
 ☁️   clouds: 81%
 💧 Humidity: 65.0%
 🎐 Wind Speed: 2.5 m/s
❕Last Update: broken clouds
🗜️  Pressure: 1015
🌊 Sea Level: 1015
****************************************************
```

## Error Handling

- Errors are logged to `errors.log`.
- The program retries failed requests up to 3 times with exponential backoff.

## License

MIT

---

