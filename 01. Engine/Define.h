#pragma once

// ���� : �� ��ũ�θ� ����ϸ� �̱��� Ŭ������ �����Ǵ� ��ũ���Դϴ�.
#define SINGLETON(classname)				\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}