#ifndef GEOGRAPHICUTILS_H
#define GEOGRAPHICUTILS_H

#include "StreetMap.h"
#include <cmath>

struct SGeographicUtils{
    static double DegreesToRadians(double deg);
    static double RadiansToDegrees(double rad);
    static double HaversineDistanceInMiles(CStreetMap::TLocation loc1, CStreetMap::TLocation loc2);
    //     const double EarthRadiusMiles = 3959.0;  // Radius of the Earth in miles

    //     // Convert latitude and longitude from degrees to radians
    //     double lat1Rad = loc1.first * M_PI / 180.0;
    //     double lon1Rad = loc1.second * M_PI / 180.0;
    //     double lat2Rad = loc2.first * M_PI / 180.0;
    //     double lon2Rad = loc2.second * M_PI / 180.0;

    //     // Calculate differences
    //     double dlat = lat2Rad - lat1Rad;
    //     double dlon = lon2Rad - lon1Rad;

    //     // Haversine formula
    //     double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
    //             cos(lat1Rad) * cos(lat2Rad) *
    //             sin(dlon / 2.0) * sin(dlon / 2.0);

    //     double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    //     // Distance in miles
    //     double distance = EarthRadiusMiles * c;

    //     return distance;
    // }
    static double CalculateBearing(CStreetMap::TLocation src, CStreetMap::TLocation dest);
    static std::string BearingToDirection(double bearing);
    static std::string ConvertLLToDMS(CStreetMap::TLocation loc);
};

#endif
