#pragma once

#include "ElectricFieldDoc.h"

namespace ChargeConfigurations {

	class CDipoleDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CDipoleDoc)
	public:
		virtual BOOL OnNewDocument();
	};


	class CTwoEqualChargesDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CTwoEqualChargesDoc)
	public:
		virtual BOOL OnNewDocument();
	};


	class CTwoUnequalChargesSameSignDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CTwoUnequalChargesSameSignDoc)
	public:
		virtual BOOL OnNewDocument();
	};

	class CTwoUnequalChargesDifSignDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CTwoUnequalChargesDifSignDoc)
	public:
		virtual BOOL OnNewDocument();
	};

	class CQuadrupoleDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CQuadrupoleDoc)
	public:
		virtual BOOL OnNewDocument();
	};

	class CFourChargesSameSignDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CFourChargesSameSignDoc)
	public:
		virtual BOOL OnNewDocument();
	};

	class CTwoDipolesSameDirDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CTwoDipolesSameDirDoc)
	public:
		virtual BOOL OnNewDocument();
	};

	class CTwoDipolesDifDirDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CTwoDipolesDifDirDoc)
	public:
		virtual BOOL OnNewDocument();
	};

	class CParallelDipolesDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CParallelDipolesDoc)
	public:
		virtual BOOL OnNewDocument();
	};

}
