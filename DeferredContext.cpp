#include "DeferredContext.h"

CDeferredContext::CDeferredContext()
	: m_bufferStart(nullptr)
	, m_bufferEnd(nullptr)
	, m_bufferCurrent(nullptr)

{
}

CDeferredContext::CDeferredContext(size_t bufferSize)
	: m_bufferStart(nullptr)
	, m_bufferEnd(nullptr)
	, m_bufferCurrent(nullptr)
{
	initialize(bufferSize);
}

CDeferredContext::~CDeferredContext()
{
	if (m_bufferStart)
	{
		delete[]m_bufferStart;
		m_bufferStart = nullptr;
	}
}
void CDeferredContext::initialize(size_t bufferSize)
{
	MKGL_ASSERT(m_bufferStart == nullptr);
	m_bufferCurrent = m_bufferStart = new uint32_t[bufferSize];
	m_bufferEnd = m_bufferStart + bufferSize;
}

template<typename T, typename RT>
inline RT readValue(uint32_t*& buffer)
{
	static_assert(sizeof(T) % sizeof(uint32_t) == 0, "writeOne<T>. sizeof(T) must be a multiple of 4.");
	constexpr auto num = sizeof(T) / sizeof(uint32_t);
	if constexpr (num == 1) {
		return *(T*)(buffer++);
	}
	else if constexpr (num == 2) {
		T v;
		*((uint32_t*)&v) = *buffer++;
		*((uint32_t*)&v + 1) = *buffer++;
		return v;
	}
	else {
		T v;
		auto p = (uint32_t*)&v;
		for (unsigned i = 0; i < num; i++)
		{
			*buffer++ = *p++;
		}
		return v;
	}
}

//キャスト変換特殊化
template<> inline bool readValue<bool>(uint32_t*& buffer) { return (bool)readValue<uint32_t, uint32_t>(buffer); }
template<> inline GLboolean readValue<GLboolean>(uint32_t*& buffer) { return (GLboolean)readValue<uint32_t, uint32_t>(buffer); }

//無変換オーバーロード
template<typename T> inline T readValue(uint32_t*& buffer) { return readValue<T, T>(buffer); }

//特殊型対応特殊化
template<> inline GLuint readValue<ObjectID>(uint32_t*& buffer) { return readValue<ObjectID>(buffer)->id; }
template<> inline GLenum readValue<BufferTarget>(uint32_t*& buffer) { return (GLenum)readValue<BufferTarget>(buffer); }

template<typename... TYPES, typename... TYPES2>
inline void callFunction(uint32_t*& buffer, void(*func)(TYPES...), void (CDeferredContext::* func2)(TYPES2...))
{

}

template<>
void callFunction(uint32_t*& buffer, void (*func)(), void (CDeferredContext::*)())
{
	func();
}
template<typename T1, typename D1>
void callFunction(uint32_t*& buffer, void (*func)(T1), void (CDeferredContext::*)(D1))
{
	auto v1 = readValue<D1, T1>(buffer);
	func(v1);
}
template<typename T1, typename T2, typename D1, typename D2>
void callFunction(uint32_t*& buffer, void (*func)(T1, T2), void (CDeferredContext::*)(D1, D2))
{
	auto v1 = readValue<D1, T1>(buffer); auto v2 = readValue<D2, T2>(buffer);
	func(v1, v2);
}
template<typename T1, typename T2, typename T3, typename D1, typename D2, typename D3>
void callFunction(uint32_t*& buffer, void (*func)(T1, T2, T3), void (CDeferredContext::*)(D1, D2, D3))
{
	auto v1 = readValue<D1, T1>(buffer); auto v2 = readValue<D2, T2>(buffer); auto v3 = readValue<D3, T3>(buffer);
	func(v1, v2, v3);
}
template<typename T1, typename T2, typename T3, typename T4, typename D1, typename D2, typename D3, typename D4>
void callFunction(uint32_t*& buffer, void (*func)(T1, T2, T3, T4), void (CDeferredContext::*)(D1, D2, D3, D4))
{
	auto v1 = readValue<D1, T1>(buffer); auto v2 = readValue<D2, T2>(buffer); auto v3 = readValue<D3, T3>(buffer); auto v4 = readValue<D4, T4>(buffer);
	func(v1, v2, v3, v4);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename D1, typename D2, typename D3, typename D4, typename D5>
void callFunction(uint32_t*& buffer, void (*func)(T1, T2, T3, T4, T5), void (CDeferredContext::*)(D1, D2, D3, D4, D5))
{
	auto v1 = readValue<D1, T1>(buffer); auto v2 = readValue<D2, T2>(buffer); auto v3 = readValue<D3, T3>(buffer); auto v4 = readValue<D4, T4>(buffer);
	auto v5 = readValue<D5, T5>(buffer);
	func(v1, v2, v3, v4, v5);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename D1, typename D2, typename D3, typename D4, typename D5, typename D6>
void callFunction(uint32_t*& buffer, void (*func)(T1, T2, T3, T4, T5, T6), void (CDeferredContext::*)(D1, D2, D3, D4, D5, D6))
{
	auto v1 = readValue<D1, T1>(buffer); auto v2 = readValue<D2, T2>(buffer); auto v3 = readValue<D3, T3>(buffer); auto v4 = readValue<D4, T4>(buffer);
	auto v5 = readValue<D5, T5>(buffer); auto v6 = readValue<D6, T6>(buffer);
	func(v1, v2, v3, v4, v5, v6);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename D1, typename D2, typename D3, typename D4, typename D5, typename D6, typename D7>
void callFunction(uint32_t*& buffer, void (*func)(T1, T2, T3, T4, T5, T6, T7), void (CDeferredContext::*)(D1, D2, D3, D4, D5, D6, D7))
{
	auto v1 = readValue<D1, T1>(buffer); auto v2 = readValue<D2, T2>(buffer); auto v3 = readValue<D3, T3>(buffer); auto v4 = readValue<D4, T4>(buffer);
	auto v5 = readValue<D5, T5>(buffer); auto v6 = readValue<D6, T6>(buffer); auto v7 = readValue<D7, T7>(buffer);
	func(v1, v2, v3, v4, v5, v6, v7);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename D1, typename D2, typename D3, typename D4, typename D5, typename D6, typename D7, typename D8>
void callFunction(uint32_t*& buffer, void (*func)(T1, T2, T3, T4, T5, T6, T7, T8), void (CDeferredContext::*)(D1, D2, D3, D4, D5, D6, D7, D8))
{
	auto v1 = readValue<D1, T1>(buffer); auto v2 = readValue<D2, T2>(buffer); auto v3 = readValue<D3, T3>(buffer); auto v4 = readValue<D4, T4>(buffer);
	auto v5 = readValue<D5, T5>(buffer); auto v6 = readValue<D6, T6>(buffer); auto v7 = readValue<D7, T7>(buffer); auto v8 = readValue<D8, T8>(buffer);
	func(v1, v2, v3, v4, v5, v6, v7, v8);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename D1, typename D2, typename D3, typename D4, typename D5, typename D6, typename D7, typename D8, typename D9>
void callFunction(uint32_t*& buffer, void (*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9), void (CDeferredContext::*)(D1, D2, D3, D4, D5, D6, D7, D8, D9))
{
	auto v1 = readValue<D1, T1>(buffer); auto v2 = readValue<D2, T2>(buffer); auto v3 = readValue<D3, T3>(buffer); auto v4 = readValue<D4, T4>(buffer);
	auto v5 = readValue<D5, T5>(buffer); auto v6 = readValue<D6, T6>(buffer); auto v7 = readValue<D7, T7>(buffer); auto v8 = readValue<D8, T8>(buffer); auto v9 = readValue<D9, T9>(buffer);
	func(v1, v2, v3, v4, v5, v6, v7, v8, v9);
}


//for genObject
template<>
void callFunction(uint32_t*& buffer, void (*func)(GLsizei, GLuint*), void (CDeferredContext::*)(GLsizei, ObjectID*))
{
	auto n = readValue<GLsizei, GLsizei>(buffer); auto objDst = readValue<ObjectID*, ObjectID*>(buffer);
	auto genObj = (GLuint*)alloca(sizeof(GLuint) * n);
	func(n, genObj);
	for (int i = 0; i < n; i++) {
		objDst[i]->id = n;
	}
}
//for deleteObject
template<>
void callFunction(uint32_t*& buffer, void (*func)(GLsizei, const GLuint*), void (CDeferredContext::*)(GLsizei, const ObjectID*))
{
	auto n = readValue<GLsizei, GLsizei>(buffer); auto objSrc = readValue<const ObjectID*, const ObjectID*>(buffer);
	auto objects = (GLuint*)alloca(sizeof(GLuint) * n);
	for (int i = 0; i < n; i++) {
		objects[i] = objSrc[i]->id;
	}
	func(n, objects);
}

#define CASE_GL_CALL(func,...)		case DeferredCommand::func: { callFunction(p,::func, &CDeferredContext::func); break; }

// submit deferred commands.
// rquire eglMakeCurrent call to binded context.
void CDeferredContext::submit()
{
	auto p = m_bufferStart;
	while (p < m_bufferCurrent)
	{
		auto cmd = DeferredCommand(*p++);
		switch (cmd)
		{
			CASE_GL_CALL(glBindBuffer)
				CASE_GL_CALL(glBufferData)
				CASE_GL_CALL(glBufferSubData)

				CASE_GL_CALL(glClear)
				CASE_GL_CALL(glClearColor)
				CASE_GL_CALL(glClearDepthf)
				CASE_GL_CALL(glClearStencil)
				CASE_GL_CALL(glColorMask)

		case DeferredCommand::glCreateProgram:
			{
				ObjectID obj = readValue<ObjectID, ObjectID>(p);
				obj->id = ::glCreateProgram();
				break;
			}
		case DeferredCommand::glCreateShader:
		{
			ObjectID obj = readValue<ObjectID, ObjectID>(p);
			GLenum type = readValue<GLenum, GLenum>(p);
			obj->id = ::glCreateShader(type);
			break;
		}

		CASE_GL_CALL(glDeleteBuffers)
			CASE_GL_CALL(glDeleteFramebuffers)
			CASE_GL_CALL(glDeleteProgram)
			CASE_GL_CALL(glDeleteRenderbuffers)
			CASE_GL_CALL(glDeleteShader)
			CASE_GL_CALL(glDeleteTextures)

			CASE_GL_CALL(glGenBuffers)
			CASE_GL_CALL(glGenFramebuffers)
			CASE_GL_CALL(glGenTextures)

			CASE_GL_CALL(glBindFramebuffer)
			CASE_GL_CALL(glBindTexture)

			CASE_GL_CALL(glFramebufferTexture2D)

			CASE_GL_CALL(glTexImage2D)

			CASE_GL_CALL(glShaderSource)
			CASE_GL_CALL(glCompileShader)
			CASE_GL_CALL(glGetShaderiv)
			CASE_GL_CALL(glAttachShader)
			CASE_GL_CALL(glLinkProgram)

		case DeferredCommand::allocateBufferMemory:
		{
			auto intCnt = readValue<uint32_t>(p);
			p += intCnt;
			break;
		}

		case DeferredCommand::freeMemory:
		{
			void* freePtr = readValue<void*>(p);
			free(freePtr);
			break;
		}
		//case DeferredCommand::signalEvent:
		//{
		//	System::CSignalEvent* evt = readValue<System::CSignalEvent*>(p);
		//	evt->signal();
		//	break;
		//}
		case DeferredCommand::addCallback:
		{
			auto func = readValue<std::function<void(void)>*>(p);
			(*func)();
			break;
		}
		}
	}
}
