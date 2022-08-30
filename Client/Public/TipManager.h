#pragma once
#ifndef __CLIENT_TIP_MANAGER_H__
#define __CLIENT_TIP_MANAGER_H__

#include "Base.h"
BEGIN(Client)
class CTipManager final : public CBase
{
	DECLARE_SINGLETON(CTipManager)

private:
	explicit CTipManager();
	virtual ~CTipManager() DEFAULT;


public:
	virtual void			Free() override;

};
END
#endif // !__CLIENT_TIP_MANAGER_H__