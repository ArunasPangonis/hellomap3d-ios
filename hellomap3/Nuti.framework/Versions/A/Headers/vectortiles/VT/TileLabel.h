/*
 * Copyright 2014 Nutiteq Llc. All rights reserved.
 * Copying and using this code is allowed only according
 * to license terms, as given in https://www.nutiteq.com/license/
 */

#ifndef _NUTI_VT_TILELABEL_H_
#define _NUTI_VT_TILELABEL_H_

#include "Color.h"
#include "Bitmap.h"
#include "Font.h"
#include "VertexArray.h"
#include "TileLayerStyles.h"

#include <memory>
#include <array>
#include <list>
#include <vector>
#include <limits>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include <cglib/vec.h>
#include <cglib/mat.h>
#include <cglib/bbox.h>
#include <cglib/frustum3.h>

namespace Nuti { namespace VT {
	class TileLabel {
	public:
		struct ViewState {
			float scale;
			float aspect;
			std::array<cglib::vec3<float>, 3> orientation;
			cglib::vec3<double> origin;
			cglib::frustum3<double> frustum;

			explicit ViewState(const cglib::mat4x4<double>& projectionMatrix, const cglib::mat4x4<double>& cameraMatrix, float zoom, float aspectRatio, float scaleFactor) {
				cglib::mat4x4<double> invCameraMatrix = cglib::inverse(cameraMatrix);
				scale = std::pow(2.0f, -zoom) * scaleFactor;
				aspect = aspectRatio;
				for (int i = 0; i < 3; i++) {
					orientation[i] = cglib::vec3<float>::convert(cglib::proj_o(cglib::col_vector(invCameraMatrix, i)));
				}
				origin = cglib::proj_p(cglib::col_vector(invCameraMatrix, 3));
				frustum = cglib::gl_projection_frustum(projectionMatrix * cameraMatrix);
			}
		};

        explicit TileLabel(long long id, long long groupId, std::shared_ptr<const Font> font, std::vector<Font::Glyph> glyphs, boost::optional<cglib::vec3<double>> position, std::vector<cglib::vec3<double>> vertices, LabelOrientation orientation, const boost::optional<cglib::mat3x3<float>>& transform, float scale, const Color& color);

		long long getId() const { return _id; }
		long long getGroupId() const { return _groupId; }
		bool isValid() const { return (bool) _placement; }
		const Color& getColor() const { return _color; }

		int getPriority() const { return _priority; }
		void setPriority(int priority) { _priority = priority; }

		float getMinimumGroupDistance() const { return _minimumGroupDistance; }
		void setMinimumGroupDistance(float distance) { _minimumGroupDistance = distance; }

		float getOpacity() const { return _opacity; }
		void setOpacity(float opacity) { _opacity = opacity; }

		bool isVisible() const { return _visible; }
		void setVisible(bool visible) { _visible = visible; }

		bool isActive() const { return _active; }
		void setActive(bool active) { _active = active; }

		void transformGeometry(const cglib::mat4x4<double>& transform);
		void mergeGeometries(const TileLabel& label);

		LabelOrientation getOrientation() const { return _orientation; }
		const std::shared_ptr<const Font>& getFont() const { return _font; }

		void snapPlacement(const TileLabel& label);
		bool updatePlacement(const ViewState& viewState);

		bool calculateCenter(cglib::vec3<double>& pos) const;
		bool calculateEnvelope(const ViewState& viewState, std::array<cglib::vec3<float>, 4>& envelope) const;
		bool calculateVertexData(const ViewState& viewState, VertexArray<cglib::vec3<float>>& vertices, VertexArray<cglib::vec2<float>>& texCoords, VertexArray<unsigned short>& indices) const;

	private:
        using Vertex = cglib::vec3<double>;
        using Vertices = std::vector<Vertex>;
        using VerticesList = std::list<Vertices>;

		struct Placement {
			struct Edge {
				cglib::vec2<float> pos0;
				cglib::vec2<float> pos1;
				cglib::vec2<float> xAxis;
                cglib::vec2<float> yAxis;
                float length;
				
                explicit Edge(const cglib::vec3<double>& p0, const cglib::vec3<double>& p1, const cglib::vec3<double>& origin) {
                    pos0 = cglib::vec2<float>::convert(cglib::proj_o(p0 - origin));
                    pos1 = cglib::vec2<float>::convert(cglib::proj_o(p1 - origin));
                    length = static_cast<float>(cglib::length(pos1 - pos0));
					xAxis = cglib::vec2<float>::convert(pos1 - pos0) * (1.0f / length);
                    yAxis = cglib::vec2<float>(-xAxis(1), xAxis(0));
				}
			};
			
			std::vector<Edge> edges;
			std::size_t index;
			cglib::vec3<double> pos;
			
            explicit Placement(std::vector<Edge> edges, std::size_t index, const cglib::vec3<double>& pos) : edges(std::move(edges)), index(index), pos(pos) { }
		};
		
        void setupCoordinateSystem(const ViewState& viewState, const std::shared_ptr<Placement>& placement, cglib::vec3<float>& origin, cglib::vec3<float>& xAxis, cglib::vec3<float>& yAxis) const;
		void buildPointVertexData(VertexArray<cglib::vec2<float>>& vertices, VertexArray<cglib::vec2<float>>& texCoords, VertexArray<unsigned short>& indices) const;
		bool buildLineVertexData(const std::shared_ptr<Placement>& placement, float scale, VertexArray<cglib::vec2<float>>& vertices, VertexArray<cglib::vec2<float>>& texCoords, VertexArray<unsigned short>& indices) const;

        std::shared_ptr<Placement> getPlacement(const ViewState& viewState) const;
        std::shared_ptr<Placement> reversePlacement(const std::shared_ptr<Placement>& placement) const;
        std::shared_ptr<Placement> findSnappedPointPlacement(const Vertex& position, const Vertices& vertices) const;
        std::shared_ptr<Placement> findSnappedLinePlacement(const Vertex& position, const VerticesList& verticesList) const;
        std::shared_ptr<Placement> findClippedPointPlacement(const ViewState& viewState, const Vertices& vertices) const;
		std::shared_ptr<Placement> findClippedLinePlacement(const ViewState& viewState, const VerticesList& verticesList) const;

		const long long _id;
		const long long _groupId;
		const std::shared_ptr<const Font> _font;
		const std::vector<Font::Glyph> _glyphs;
		const LabelOrientation _orientation;
        const boost::optional<Vertex> _originalPosition;
        const Vertices _originalVertices;
        const float _scale;
        const Color _color;

        boost::optional<cglib::mat3x3<float>> _transform;
        Vertices _transformedPositions;
        VerticesList _transformedVerticesList;
        cglib::bbox2<float> _bbox = cglib::bbox2<float>::smallest();

		int _priority = 0;
		float _minimumGroupDistance = std::numeric_limits<float>::infinity();
		float _opacity = 0.0f;
		bool _visible = false;
		bool _active = false;

		std::shared_ptr<Placement> _placement;
		std::shared_ptr<Placement> _flippedPlacement;

        mutable bool _cachedValid = false;
        mutable float _cachedScale = 0;
        mutable cglib::vec3<double> _cachedOrigin;
        mutable std::shared_ptr<Placement> _cachedPlacement;
        mutable VertexArray<cglib::vec2<float>> _cachedVertices;
        mutable VertexArray<cglib::vec2<float>> _cachedTexCoords;
		mutable VertexArray<unsigned short> _cachedIndices;
	};
} }

#endif
