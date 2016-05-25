#include "stdafx.h"

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ElectricField.h"
#endif

#include "ConfigDocs.h"

namespace ChargeConfigurations {


	IMPLEMENT_DYNCREATE(CDipoleDoc, CElectricFieldDoc)

	BOOL CDipoleDoc::OnNewDocument()
	{
		if (!CElectricFieldDoc::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		Charge charge;
		charge.position = Vector2D<double>(-2, 0);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = -1;
		charge.position = Vector2D<double>(2, 0);

		calculator->field.charges.push_back(charge);

		calculator->StartCalculating(&calculator->field);

		return TRUE;
	}



	IMPLEMENT_DYNCREATE(CTwoEqualChargesDoc, CElectricFieldDoc)

	BOOL CTwoEqualChargesDoc::OnNewDocument()
	{
		if (!CElectricFieldDoc::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		Charge charge;
		charge.position = Vector2D<double>(-2, 0);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = 1;
		charge.position = Vector2D<double>(2, 0);

		calculator->field.charges.push_back(charge);

		calculator->StartCalculating(&calculator->field);

		return TRUE;
	}

	IMPLEMENT_DYNCREATE(CTwoUnequalChargesSameSignDoc, CElectricFieldDoc)

	BOOL CTwoUnequalChargesSameSignDoc::OnNewDocument()
	{
		if (!CElectricFieldDoc::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		Charge charge;
		charge.position = Vector2D<double>(-2, 0);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = 2;
		charge.position = Vector2D<double>(2, 0);

		calculator->field.charges.push_back(charge);

		calculator->StartCalculating(&calculator->field);

		return TRUE;
	}


	IMPLEMENT_DYNCREATE(CTwoUnequalChargesDifSignDoc, CElectricFieldDoc)

	BOOL CTwoUnequalChargesDifSignDoc::OnNewDocument()
	{
		if (!CElectricFieldDoc::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		Charge charge;
		charge.position = Vector2D<double>(-2, 0);
		charge.charge = 2;

		calculator->field.charges.push_back(charge);

		charge.charge = -1;
		charge.position = Vector2D<double>(2, 0);

		calculator->field.charges.push_back(charge);

		calculator->StartCalculating(&calculator->field);

		return TRUE;
	}



	IMPLEMENT_DYNCREATE(CQuadrupoleDoc, CElectricFieldDoc)

	BOOL CQuadrupoleDoc::OnNewDocument()
	{
		if (!CElectricFieldDoc::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		Charge charge;
		charge.position = Vector2D<double>(-1.5, -1.5);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = -1;
		charge.position = Vector2D<double>(1.5, -1.5);

		calculator->field.charges.push_back(charge);




		charge.position = Vector2D<double>(-1.5, 1.5);
		charge.charge = -1;

		calculator->field.charges.push_back(charge);

		charge.charge = 1;
		charge.position = Vector2D<double>(1.5, 1.5);

		calculator->field.charges.push_back(charge);


		calculator->StartCalculating(&calculator->field);

		return TRUE;
	}


	IMPLEMENT_DYNCREATE(CFourChargesSameSignDoc, CElectricFieldDoc)

	BOOL CFourChargesSameSignDoc::OnNewDocument()
	{
		if (!CElectricFieldDoc::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		Charge charge;
		charge.position = Vector2D<double>(-3, 0);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = 1;
		charge.position = Vector2D<double>(-1, 0);

		calculator->field.charges.push_back(charge);




		charge.position = Vector2D<double>(1, 0);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = 1;
		charge.position = Vector2D<double>(3, 0);

		calculator->field.charges.push_back(charge);


		calculator->StartCalculating(&calculator->field);

		return TRUE;
	}


	IMPLEMENT_DYNCREATE(CTwoDipolesSameDirDoc, CElectricFieldDoc)

	BOOL CTwoDipolesSameDirDoc::OnNewDocument()
	{
		if (!CElectricFieldDoc::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		Charge charge;
		charge.position = Vector2D<double>(-3, 0);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = -1;
		charge.position = Vector2D<double>(-1, 0);

		calculator->field.charges.push_back(charge);




		charge.position = Vector2D<double>(1, 0);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = -1;
		charge.position = Vector2D<double>(3, 0);

		calculator->field.charges.push_back(charge);


		calculator->StartCalculating(&calculator->field);

		return TRUE;
	}


	IMPLEMENT_DYNCREATE(CTwoDipolesDifDirDoc, CElectricFieldDoc)

	BOOL CTwoDipolesDifDirDoc::OnNewDocument()
	{
		if (!CElectricFieldDoc::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		Charge charge;
		charge.position = Vector2D<double>(-3, 0);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = -1;
		charge.position = Vector2D<double>(-1, 0);

		calculator->field.charges.push_back(charge);




		charge.position = Vector2D<double>(1, 0);
		charge.charge = -1;

		calculator->field.charges.push_back(charge);

		charge.charge = 1;
		charge.position = Vector2D<double>(3, 0);

		calculator->field.charges.push_back(charge);


		calculator->StartCalculating(&calculator->field);

		return TRUE;
	}


	IMPLEMENT_DYNCREATE(CParallelDipolesDoc, CElectricFieldDoc)

	BOOL CParallelDipolesDoc::OnNewDocument()
	{
		if (!CElectricFieldDoc::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		Charge charge;
		charge.position = Vector2D<double>(-1.5, -1.5);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = -1;
		charge.position = Vector2D<double>(1.5, -1.5);

		calculator->field.charges.push_back(charge);




		charge.position = Vector2D<double>(-1.5, 1.5);
		charge.charge = 1;

		calculator->field.charges.push_back(charge);

		charge.charge = -1;
		charge.position = Vector2D<double>(1.5, 1.5);

		calculator->field.charges.push_back(charge);


		calculator->StartCalculating(&calculator->field);

		return TRUE;
	}

}