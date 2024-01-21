#pragma once

// 건재 : 이 매크로를 사용하면 싱글톤 클래스로 생성되는 매크로입니다.
#define SINGLETON(classname)				\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}