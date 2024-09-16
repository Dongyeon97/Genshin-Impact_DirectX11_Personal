#pragma once

BEGIN(Engine)

class ENGINE_DLL CObject abstract : public enable_shared_from_this<CObject>
{
public:
	explicit CObject() = default;
	virtual ~CObject() = default;
};

END