#include "Oscillator.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <strstream>
#include <vector>
#include <ctime>

std::string build_url( time_t timestamp ) {
    char        buffer[21];
    struct tm   datetime;
    // If no time specified the use the current time
    if (!timestamp)
        time(&timestamp);
    datetime = *localtime(&timestamp);
    std::strftime(buffer, 21, "%FT%TZ", &datetime);
    std::string end_time(buffer);
    // Calculate a week minus one hour previous (in seconds)
    timestamp -= 601200;
    datetime = *localtime(&timestamp);
    std::strftime(buffer, 21, "%FT%TZ", &datetime);
    std::string start_time(buffer);
    // Build the url for the API call
    std::stringstream url;
    url << "https://opendata.fmi.fi/wfs?request=getFeature&"
    // Usage specified here: https://en.ilmatieteenlaitos.fi/open-data-manual-fmi-wfs-services
    << "storedquery_id=urban::observations::airquality::hourly::timevaluepair&"
    // Hourly readings from the Mannerheimintie station, more info here: https://www.ilmatieteenlaitos.fi/havaintoasemat
    << "timestep=60&" << "fmisid=100742&"
    << "starttime=" << start_time
    << "&endtime=" << end_time;
    return url.str();
}

void  DirtyManner::fetch_wavetable( time_t timestamp ) {
    cURLpp::initialize(CURL_GLOBAL_DEFAULT);
    cURLpp::Easy request; // EasyHandle for the session
    std::ostrstream response; // Stream to write into
    std::string     url = build_url(timestamp);
    try {
        request.setOpt(curlpp::options::Url(url));
        request.setOpt(curlpp::options::WriteStream(&response));
        request.perform();
    }
    catch (cURLpp::RuntimeError &e) {
        std::cerr << e.what() << std::endl;
    }
    catch (cURLpp::LogicError &e) {
        std::cerr << e.what() << std::endl;
    }
    cURLpp::terminate();
    std::string result = response.str(); // Copy data into string
    std::vector<float> wavetable;
    int data_pos;
    int len;
    char buffer[10];
    size_t i = -1;
    // Skip forward in data to entries for PM2.5 Quality
    i = result.find("QBCPM25_PT1H_AVG", i + 1);
    while ((i = result.find("<wml2:value", i + 1)) != std::string::npos) {
        data_pos = result.find(">", i) + 1;
        len = result.find("<", data_pos) - data_pos;
        if (!std::isdigit(result.at(data_pos)))
            continue ;
        result.copy(buffer, len, data_pos);
        buffer[len] = '\0';
        wavetable.push_back(atof(buffer));
    }
    this->wavetable = wavetable;
}
