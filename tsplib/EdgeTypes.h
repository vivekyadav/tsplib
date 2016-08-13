#pragma once

namespace TspLib {
	enum EdgeWeightType {
		EXPLICIT,
		EUC_2D,
		EUC_3D,
		MAX_2D,
		MAX_3D,
		MAN_2D,
		MAN_3D,
		CEIL_2D,
		GEO,
		ATT,
		XRAY1,
		XRAY2,
		SPECIAL
	};

	enum EdgeWeightFormat {
		FUNCTION,
		FULL_MATRIX,
		UPPER_ROW,
		LOWER_ROW,
		UPPER_DIAG_ROW,
		LOWER_DIAG_ROW,
		UPPER_COL,
		LOWER_COL,
		UPPER_DIAG_COL,
		LOWER_DIAG_COL
	};

}

