/*
 * Copyright 2014 Nutiteq Llc. All rights reserved.
 * Copying and using this code is allowed only according
 * to license terms, as given in https://www.nutiteq.com/license/
 */

#ifndef _NUTI_ROUTINGPROXY_H_
#define _NUTI_ROUTINGPROXY_H_

#include "core/MapPos.h"
#include "routing/RoutingService.h"

#include <memory>
#include <string>
#include <vector>

namespace Nuti {
    namespace Routing {
        class RouteFinder;
    }
    
    class HTTPClient;

    class RoutingProxy {
    public:
        static std::shared_ptr<RoutingResult> CalculateRoute(const std::shared_ptr<Routing::RouteFinder>& routeFinder, const std::shared_ptr<RoutingRequest>& request);
        
        static std::shared_ptr<RoutingResult> CalculateRoute(HTTPClient& httpClient, const std::string& url, const std::shared_ptr<RoutingRequest>& request);

    private:
        RoutingProxy();
        
        static float CalculateTurnAngle(const std::vector<MapPos>& epsg3857Points, int pointIndex);
        
        static float CalculateAzimuth(const std::vector<MapPos>& epsg3857Points, int pointIndex);
        
        static bool TranslateInstructionCode(int instructionCode, RoutingAction::RoutingAction& action);
        
        static std::vector<MapPos> DecodeGeometry(const std::string& encodedGeometry);
        
        static const double COORDINATE_SCALE;
    };
    
}

#endif
