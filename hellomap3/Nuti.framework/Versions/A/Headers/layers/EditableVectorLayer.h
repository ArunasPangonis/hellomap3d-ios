/*
 * Copyright 2014 Nutiteq Llc. All rights reserved.
 * Copying and using this code is allowed only according
 * to license terms, as given in https://www.nutiteq.com/license/
 */

#ifndef _NUTI_EDITABLEVECTORLAYER_H_
#define _NUTI_EDITABLEVECTORLAYER_H_

#ifdef _NUTI_EDITABLE_SUPPORT

#include "components/DirectorPtr.h"
#include "layers/VectorLayer.h"
#include "ui/VectorEditEventListener.h"
#include "ui/TouchHandler.h"

#include <memory>

namespace Nuti {
    class PointStyle;

	/**
	 * A vector layer that supports editing its contents. Editing requires registered listener.
	 */
	class EditableVectorLayer : public VectorLayer {
	public:
		/**
		 * Constructs a VectorLayer object from a data source.
		 * @param dataSource The data source from which this layer loads data.
		 */
		EditableVectorLayer(const std::shared_ptr<VectorDataSource>& dataSource);
		virtual ~EditableVectorLayer();

		/**
		 * Returns the selected vector element. If no element is currently selected, null is returned.
		 * @return The selected vector element or null if no element is selected.
		 */
		std::shared_ptr<VectorElement> getSelectedVectorElement() const;
		/**
		 * Selects the given vector element. Deselects currently selected element is null argument is used.
		 * @param element The vector element to select or null.
		 */
		void setSelectedVectorElement(const std::shared_ptr<VectorElement>& element);

		/**
		 * Returns the current edit event listener for the layer.
		 * @return The current edit event listener or null if no listener is set.
		 */
		std::shared_ptr<VectorEditEventListener> getVectorEditEventListener() const;
		/**
		 * Sets the current edit event listener for the layer. 
		 * Note that valid listener is required for editing operations to work.
		 * @param listener The edit event listener to use.
		 */
		void setVectorEditEventListener(const std::shared_ptr<VectorEditEventListener>& listener);

	protected:
		virtual void setComponents(const std::shared_ptr<CancelableThreadPool>& envelopeThreadPool,
			const std::shared_ptr<CancelableThreadPool>& tileThreadPool,
			const std::weak_ptr<Options>& options,
			const std::weak_ptr<MapRenderer>& mapRenderer,
			const std::weak_ptr<TouchHandler>& touchHandler);

		virtual void offsetLayerHorizontally(double offset);

		virtual void onSurfaceCreated(ShaderManager& shaderManager);
		virtual bool onDrawFrame(float deltaSeconds, BillboardSorter& billboardSorter, LRUTextureCache<std::shared_ptr<Bitmap> >& styleCache, const ViewState& viewState);
		virtual void onSurfaceDestroyed();

        virtual void addRendererElement(const std::shared_ptr<VectorElement>& element);
        virtual bool refreshRendererElements();
        virtual bool syncRendererElement(const std::shared_ptr<VectorElement>& element, const ViewState& viewState, bool remove);
        
		virtual void registerDataSourceListener();
		virtual void unregisterDataSourceListener();

		virtual void registerTouchHandlerListener();
		virtual void unregisterTouchHandlerListener();

	private:
		class DataSourceListener : public VectorDataSource::OnChangeListener {
		public:
			DataSourceListener(const std::shared_ptr<EditableVectorLayer>& layer);

			virtual void onElementAdded(const std::shared_ptr<VectorElement>& element);
			virtual void onElementChanged(const std::shared_ptr<VectorElement>& element);
			virtual void onElementRemoved(const std::shared_ptr<VectorElement>& element);
			virtual void onElementsAdded(const std::vector<std::shared_ptr<VectorElement> >& elements);
			virtual void onElementsChanged();
			virtual void onElementsRemoved();

		private:
			std::weak_ptr<EditableVectorLayer> _layer;
		};

		class TouchHandlerListener : public TouchHandler::OnTouchListener {
		public:
			TouchHandlerListener(const std::shared_ptr<EditableVectorLayer>& layer);

			virtual bool onTouchEvent(int action, const ScreenPos& screenPos1, const ScreenPos& screenPos2);

		private:
			std::weak_ptr<EditableVectorLayer> _layer;
		};

        void updateElementGeometry(std::shared_ptr<VectorElement> element, std::shared_ptr<Geometry> geometry, const MapVec& delta);
        std::shared_ptr<Geometry> updateGeometryPoints(std::shared_ptr<Geometry> geometry, const MapVec& delta);

        void removeElement(std::shared_ptr<VectorElement> element);
        
        void updateElementPoint(std::shared_ptr<VectorElement> element, const std::shared_ptr<Point>& dragPoint, const MapPos& pos);
        std::shared_ptr<Geometry> updateGeometryPoint(std::shared_ptr<Geometry> geometry, int& offset, int index, const MapPos& mapPos);

        void removeElementPoint(std::shared_ptr<VectorElement> element, const std::shared_ptr<Point>& dragPoint);
        std::shared_ptr<Geometry> removeGeometryPoint(std::shared_ptr<Geometry> geometry, int& offset, int index);

        void syncElementOverlayPoints(const std::shared_ptr<VectorElement>& element);
        void createGeometryOverlayPoints(const std::shared_ptr<Geometry>& geometry, int& index, std::vector<std::shared_ptr<Point> >& overlayPoints) const;
		std::shared_ptr<Point> createOverlayPoint(const MapPos& mapPos, bool virtualPoint, int index) const;

        static bool IsSameElement(const std::shared_ptr<VectorElement>& element1, const std::shared_ptr<VectorElement>& element2);
        
		std::shared_ptr<DataSourceListener> _dataSourceListener;

		std::shared_ptr<TouchHandlerListener> _touchHandlerListener;

		DirectorPtr<VectorElement> _selectedVectorElement;

		std::shared_ptr<PointRenderer> _overlayRenderer;
		std::shared_ptr<PointStyle> _overlayStyleNormal;
        std::shared_ptr<PointStyle> _overlayStyleVirtual;
		std::shared_ptr<PointStyle> _overlayStyleSelected;
		std::vector<std::shared_ptr<Point> > _overlayPoints;
		std::shared_ptr<Point> _overlayDragPoint;
        std::shared_ptr<Geometry> _overlayDragGeometry;
        MapPos _overlayDragGeometryPos;
        bool _overlayDragStarted;
		VectorElementDragMode::VectorElementDragMode _overlayDragMode;

		DirectorPtr<VectorEditEventListener> _vectorEditEventListener;
		mutable std::recursive_mutex _vectorEditEventListenerMutex;
	};

}

#endif

#endif
