/*
 * Copyright 2014 Nutiteq Llc. All rights reserved.
 * Copying and using this code is allowed only according
 * to license terms, as given in https://www.nutiteq.com/license/
 */

#ifndef _NUTI_VT_GLTILERENDERER_H_
#define _NUTI_VT_GLTILERENDERER_H_

#include "Color.h"
#include "Tile.h"
#include "GLExtensions.h"
#include "GLShaderManager.h"

#include <memory>
#include <array>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <mutex>

namespace Nuti { namespace VT {
	class GLTileRenderer {
	public:
        explicit GLTileRenderer(std::shared_ptr<std::mutex> mutex, std::shared_ptr<GLExtensions> glExtensions, float scale, bool useFBO, bool useDepth, bool useStencil);

		void setViewState(const cglib::mat4x4<double>& projectionMatrix, const cglib::mat4x4<double>& cameraMatrix, float zoom, float aspectRatio, float resolution);
		void setLightDir(const cglib::vec3<float>& lightDir);
		void setFBOClearColor(const Color& clearColor);
		void setBackgroundColor(const Color& backgroundColor);
		void setBackgroundPattern(std::shared_ptr<const BitmapPattern> pattern);
		void setVisibleTiles(const std::map<TileId, std::shared_ptr<const Tile>>& tiles, bool blend);
		std::vector<std::shared_ptr<TileLabel>> getVisibleLabels() const;

		void initializeRenderer();
		void resetRenderer();
		void deinitializeRenderer();

		void startFrame(float dt);
		bool render2D();
		bool renderLabels();
		bool render3D();
		void endFrame();

	private:
		struct BlendNode {
			TileId tileId;
			std::shared_ptr<const Tile> tile;
			float blend;
			std::vector<std::shared_ptr<BlendNode>> childNodes;

			explicit BlendNode(const TileId& tileId, std::shared_ptr<const Tile> tile, float blend) : tileId(tileId), tile(std::move(tile)), blend(blend), childNodes() { }
		};

		struct RenderNode {
			TileId tileId;
			std::shared_ptr<const TileLayer> layer;
			float blend;

            explicit RenderNode(const TileId& tileId, std::shared_ptr<const TileLayer> layer, float blend) : tileId(tileId), layer(std::move(layer)), blend(blend) { }
		};

        struct LayerFBO {
            GLuint colorTexture;
            GLuint stencilRB;
            GLuint fbo;

            LayerFBO() : colorTexture(0), stencilRB(0), fbo(0) { }
        };

        struct ScreenFBO {
            GLuint colorTexture;
            GLuint depthStencilRB;
            GLuint fbo;
            std::vector<GLenum> depthStencilAttachments;

            ScreenFBO() : colorTexture(0), depthStencilRB(0), fbo(0), depthStencilAttachments() { }
        };

        struct CompiledBitmap {
			GLuint texture;

			CompiledBitmap() : texture(0) { }
		};

		struct CompiledGeometry {
			GLuint vertexGeometryVBO;
			GLuint indicesVBO;
			GLuint geometryVAO;

			CompiledGeometry() : vertexGeometryVBO(0), indicesVBO(0), geometryVAO(0) { }
		};

		struct LabelHash {
			std::size_t operator()(const std::pair<int, long long>& labelId) const {
				return labelId.first ^ static_cast<std::size_t>(labelId.second & 0xffffffff) ^ static_cast<std::size_t>(labelId.second >> 32);
			}
		};

		static cglib::mat4x4<double> calculateLocalViewMatrix(const cglib::mat4x4<double>& cameraMatrix);

		cglib::mat4x4<double> calculateTileMatrix(const TileId& tileId, float coordScale = 1.0f) const;
        cglib::mat3x3<double> calculateTileMatrix2D(const TileId& tileId, float coordScale = 1.0f) const;
        cglib::mat4x4<float> calculateTileMVPMatrix(const TileId& tileId, float coordScale = 1.0f) const;
        cglib::vec4<double> calculateTileOrigin(const TileId& tileId) const;
        cglib::bbox3<double> calculateTileBBox(const TileId& tileId) const;

		float calculateBlendNodeOpacity(const BlendNode& blendNode, float blend) const;
		void updateBlendNode(BlendNode& blendNode, float dBlend);
		bool buildRenderNodes(const BlendNode& blendNode, float blend, std::multimap<int, RenderNode>& renderNodeMap);
		void addRenderNode(const RenderNode& renderNode, std::multimap<int, RenderNode>& renderNodeMap);
		void updateLabels(const std::vector<std::shared_ptr<TileLabel>>& labels, float dOpacity);

		bool renderBlendNodes2D(const std::vector<std::shared_ptr<BlendNode>>& blendNodes);
		bool renderBlendNodes3D(const std::vector<std::shared_ptr<BlendNode>>& blendNodes);
		bool renderLabels(const std::shared_ptr<const Bitmap>& bitmap, const std::vector<std::shared_ptr<TileLabel>>& labels);

        void blendScreenTexture(float opacity, GLuint texture);
        void blendTileTexture(const TileId& tileId, float opacity, GLuint texture);
        void renderTileMask(const TileId& tileId);
		void renderTileBackground(const TileId& tileId, float opacity);
		void renderTileGeometry(const TileId& tileId, const TileId& targetTileId, float blend, float opacity, const std::shared_ptr<TileGeometry>& geometry);
        void renderLabelBatch(const std::shared_ptr<const Bitmap>& bitmap);
        void setBlendState(CompOp compOp);
        void checkGLError();

		GLuint createBuffer();
		void deleteBuffer(GLuint& buffer);
		GLuint createVertexArray();
		void deleteVertexArray(GLuint& vertexArray);
		GLuint createTexture();
		void deleteTexture(GLuint& texture);
        LayerFBO createLayerFBO(bool useStencil);
        void deleteLayerFBO(LayerFBO& layerFBO);
        ScreenFBO createScreenFBO(bool useDepth, bool useStencil);
        void deleteScreenFBO(ScreenFBO& screenFBO);

		Color _fboClearColor;
		Color _backgroundColor;
		std::shared_ptr<const BitmapPattern> _backgroundPattern;

		GLShaderManager::ShaderContext _patternTransformContext[2][2];
		GLShaderManager _shaderManager;

		std::vector<LayerFBO> _layerFBOs;
        ScreenFBO _screenFBO;
        ScreenFBO _overlayFBO;

		cglib::vec3<float> _lightDir;
		cglib::mat4x4<double> _projectionMatrix;
		cglib::mat4x4<double> _cameraMatrix;
        cglib::mat4x4<double> _cameraProjMatrix;
		cglib::frustum3<double> _frustum;
		cglib::mat4x4<double> _labelMatrix;
		TileLabel::ViewState _labelViewState;
		VertexArray<cglib::vec3<float>> _labelVertices;
		VertexArray<cglib::vec2<float>> _labelTexCoords;
		VertexArray<cglib::vec4<float>> _labelColors;
		VertexArray<unsigned short> _labelIndices;
		float _zoom = 0;
		float _halfResolution = 0;
		int _screenWidth = 0;
		int _screenHeight = 0;

		std::shared_ptr<std::vector<std::shared_ptr<BlendNode>>> _blendNodes;
        std::shared_ptr<std::vector<std::shared_ptr<BlendNode>>> _renderBlendNodes;
        std::shared_ptr<std::unordered_map<std::shared_ptr<const Bitmap>, std::vector<std::shared_ptr<TileLabel>>>> _bitmapLabelMap;
        std::shared_ptr<std::unordered_map<std::shared_ptr<const Bitmap>, std::vector<std::shared_ptr<TileLabel>>>> _renderBitmapLabelMap;
        std::vector<std::shared_ptr<TileLabel>> _labels;
        std::unordered_map<std::pair<int, long long>, std::shared_ptr<TileLabel>, LabelHash> _labelMap;
		std::unordered_map<std::shared_ptr<const Bitmap>, CompiledBitmap> _compiledBitmapMap;
		std::unordered_map<std::shared_ptr<const TileGeometry>, CompiledGeometry> _compiledGeometryMap;

		const float _scale;
        const bool _useFBO;
        const bool _useDepth;
        const bool _useStencil;
		const std::shared_ptr<GLExtensions> _glExtensions;
        const std::shared_ptr<std::mutex> _mutex;
	};
} }

#endif
