#pragma once
#include "GameProcess.h"

class TestApp : public GameProcess
{
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void FixedUpdate() override;
	void Render() override;

public:
};