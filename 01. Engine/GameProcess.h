#pragma once

	class GameProcess
	{
	public:
		virtual void Init() abstract;
		virtual void Update() abstract;
		virtual void Render() abstract;
		virtual void LateUpdate() abstract;
		virtual void FixedUpdate() abstract;
	};