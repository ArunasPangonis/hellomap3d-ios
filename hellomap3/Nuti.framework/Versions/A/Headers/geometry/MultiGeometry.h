/*
 * Copyright 2014 Nutiteq Llc. All rights reserved.
 * Copying and using this code is allowed only according
 * to license terms, as given in https://www.nutiteq.com/license/
 */

#ifndef _NUTI_MULTIGEOMETRY_H_
#define _NUTI_MULTIGEOMETRY_H_

#include "geometry/Geometry.h"

#include <memory>
#include <vector>

namespace Nuti {

    /**
     * A generic multi geometry container.
     */
    class MultiGeometry : public Geometry {
    public:
        /**
         * Constructs a MultiGeometry from a vector of geometry objects.
         * @param geometries The geometries for multi geometry.
         */
        MultiGeometry(const std::vector<std::shared_ptr<Geometry> >& geometries);
        virtual ~MultiGeometry();

        virtual MapPos getCenterPos() const;

        /**
         * Returns the number of geometry objects in this multi geometry container.
         * @return The number of geometry objects.
         */
        int getGeometryCount() const;

        /**
         * Returns the geometry at the specified index. Index must be between 0 and getGeometryCount (exclusive)
         * @return The geometry at specified index.
         */
        std::shared_ptr<Geometry> getGeometry(int index) const;

    protected:
        std::vector<std::shared_ptr<Geometry> > _geometries;
    };

}

#endif
