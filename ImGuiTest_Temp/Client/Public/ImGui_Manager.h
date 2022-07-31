#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CImGui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager)

private:
	CImGui_Manager();
	virtual ~CImGui_Manager() = default;

public:
	HRESULT Init(LPDIRECT3DDEVICE9 pDevice);
	void Tick(_float fTimeDelta);
	void Render();

private:
	LPDIRECT3DDEVICE9 m_pGraphic_device;

private:
	bool m_bShow_Demo_Window;
	bool m_bShow_Another_Window;
	ImVec4 m_Clear_Color;

public:
	virtual void Free() override;

};

END
