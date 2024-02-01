#pragma once

#include "ElectricFieldDoc.h"

namespace ChargeConfigurations {

	class CDipoleDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CDipoleDoc)
	public:
		BOOL OnNewDocument() override;
	};


	class CTwoEqualChargesDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CTwoEqualChargesDoc)
	public:
		BOOL OnNewDocument() override;
	};


	class CTwoUnequalChargesSameSignDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CTwoUnequalChargesSameSignDoc)
	public:
		BOOL OnNewDocument() override;
	};

	class CTwoUnequalChargesDifSignDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CTwoUnequalChargesDifSignDoc)
	public:
		BOOL OnNewDocument() override;
	};

	class CQuadrupoleDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CQuadrupoleDoc)
	public:
		BOOL OnNewDocument() override;
	};

	class CFourChargesSameSignDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CFourChargesSameSignDoc)
	public:
		BOOL OnNewDocument() override;
	};

	class CTwoDipolesSameDirDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CTwoDipolesSameDirDoc)
	public:
		BOOL OnNewDocument() override;
	};

	class CTwoDipolesDifDirDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CTwoDipolesDifDirDoc)
	public:
		BOOL OnNewDocument() override;
	};

	class CParallelDipolesDoc : public CElectricFieldDoc {
		DECLARE_DYNCREATE(CParallelDipolesDoc)
	public:
		BOOL OnNewDocument() override;
	};

}
