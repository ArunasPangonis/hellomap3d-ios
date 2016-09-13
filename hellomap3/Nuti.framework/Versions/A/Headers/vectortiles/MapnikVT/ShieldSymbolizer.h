/*
 * Copyright 2014 Nutiteq Llc. All rights reserved.
 * Copying and using this code is allowed only according
 * to license terms, as given in https://www.nutiteq.com/license/
 */

#ifndef _NUTI_MAPNIKVT_SHIELDSYMBOLIZER_H_
#define _NUTI_MAPNIKVT_SHIELDSYMBOLIZER_H_

#include "TextSymbolizer.h"

namespace Nuti { namespace MapnikVT {
	class ShieldSymbolizer : public TextSymbolizer {
	public:
        explicit ShieldSymbolizer(std::vector<std::shared_ptr<FontSet>> fontSets, std::shared_ptr<Logger> logger) : TextSymbolizer(std::move(fontSets), std::move(logger)) { }

		virtual void build(const FeatureCollection& featureCollection, const SymbolizerContext& symbolizerContext, const ExpressionContext& exprContext, VT::TileLayerBuilder& layerBuilder) override;

	protected:
        virtual void bindParameter(const std::string& name, const std::string& value) override;

        std::string _file;
		bool _unlockImage = false;
		float _shieldDx = 0.0f;
		float _shieldDy = 0.0f;
	};
} }

#endif
