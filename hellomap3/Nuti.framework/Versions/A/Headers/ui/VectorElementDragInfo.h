/*
 * Copyright 2014 Nutiteq Llc. All rights reserved.
 * Copying and using this code is allowed only according
 * to license terms, as given in https://www.nutiteq.com/license/
 */

#ifndef _NUTI_VECTORELEMENTDRAGINFO_H_
#define _NUTI_VECTORELEMENTDRAGINFO_H_

#ifdef _NUTI_EDITABLE_SUPPORT

#include "components/DirectorPtr.h"
#include "core/MapPos.h"
#include "core/ScreenPos.h"
#include "vectorelements/VectorElement.h"

#include <memory>

namespace Nuti {
    
    namespace VectorElementDragMode {
        /**
         * Possible drag modes.
         */
        enum VectorElementDragMode {
            /**
             * A single vertex is being moved.
             */
            VECTOR_ELEMENT_DRAG_MODE_VERTEX,
            /**
             * The whole vector element is being moved.
             */
            VECTOR_ELEMENT_DRAG_MODE_ELEMENT
        };
    }
    
    /**
     * A container class that provies information about a dragging performed on
     * a vector element or vector element vertex.
     */
    class VectorElementDragInfo {
    public:
        /**
         * Constructs a VectorElementDragInfo object from various attributes.
         * @param vectorElement The element being dragged.
         * @param dragMode The drag mode (vertex or element)
         * @param screenPos Screen coordinates of the drag event
         * @param mapPos The drag event coordinate in projection coordinate system of the layer.
         */
        VectorElementDragInfo(const std::shared_ptr<VectorElement>& vectorElement, VectorElementDragMode::VectorElementDragMode dragMode, const ScreenPos& screenPos, const MapPos& mapPos);
        virtual ~VectorElementDragInfo();
        
        /**
         * Returns the drag mode.
         * @return The drag mode.
         */
        VectorElementDragMode::VectorElementDragMode getDragMode() const;
        
        /**
         * Returns the drag position in screen coordinates.
         * @return The drag position in screen coordinates.
         */
        const ScreenPos& getScreenPos() const;
        /**
         * Returns the drag position in projection coordinate system of the layer.
         * @return The drag position in projection coordinate system of the layer.
         */
        const MapPos& getMapPos() const;

        /**
         * Returns the vector element being dragged.
         * @return The vector element being dragged.
         */
        std::shared_ptr<VectorElement> getVectorElement() const;
        
    private:
        VectorElementDragMode::VectorElementDragMode _dragMode;
        
        ScreenPos _screenPos;
        MapPos _mapPos;
        
        DirectorPtr<VectorElement> _vectorElement;
    };
    
}

#endif

#endif
