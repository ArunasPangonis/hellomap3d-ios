/*
 * Copyright 2014 Nutiteq Llc. All rights reserved.
 * Copying and using this code is allowed only according
 * to license terms, as given in https://www.nutiteq.com/license/
 */

#ifndef _NUTI_MAPNIKVT_FEATUREDATA_H_
#define _NUTI_MAPNIKVT_FEATUREDATA_H_

#include "Value.h"

#include <string>
#include <algorithm>
#include <vector>

namespace Nuti { namespace MapnikVT {
	class FeatureData {
	public:
		enum class GeometryType {
			NULL_GEOMETRY = 0, POINT_GEOMETRY = 1, LINE_GEOMETRY = 2, POLYGON_GEOMETRY = 3
		};

        explicit FeatureData(GeometryType geomType, std::vector<std::pair<std::string, Value>> vars) : _geometryType(geomType), _variables(std::move(vars)) { }

        GeometryType getGeometryType() const { return _geometryType; }

        bool getVariable(const std::string& name, Value& value) const {
            auto it = std::find_if(_variables.begin(), _variables.end(), [name](const std::pair<std::string, Value>& var) { return var.first == name; });
            if (it == _variables.end()) {
                return false;
            }
            value = it->second;
            return true;
        }

    private:
        GeometryType _geometryType;
        std::vector<std::pair<std::string, Value>> _variables;
	};
} }

#endif
