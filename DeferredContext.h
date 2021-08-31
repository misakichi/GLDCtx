#pragma once
#ifndef _GRAPHICS_DEFERRED_CONTEXT_H
#define _GRAPHICS_DEFERRED_CONTEXT_H

namespace mkgl {
	namespace Graphics {

		typedef struct _ObjectID {
			GLuint id = 0;
		} *ObjectID;

		enum class BufferTarget : GLenum {
			Array = GL_ARRAY_BUFFER,
			Index = GL_ELEMENT_ARRAY_BUFFER,
			Uniform = GL_UNIFORM_BUFFER,
		};

		enum class DeferredCommand : uint32_t
		{
			glBindBuffer,
			glBufferData,
			glBufferSubData,

			glClear,
			glClearColor,
			glClearDepthf,
			glClearStencil,
			glColorMask,

			glCreateProgram,
			glCreateShader,

			glDeleteBuffers,
			glDeleteFramebuffers,
			glDeleteProgram,
			glDeleteRenderbuffers,
			glDeleteShader,
			glDeleteTextures,

			glGenBuffers,
			glGenFramebuffers,
			glGenTextures,

			glBindFramebuffer,
			glBindTexture,
			
			glFramebufferTexture2D,

			glTexImage2D,

			glShaderSource,
			glCompileShader,
			glGetShaderiv,
			glAttachShader,
			glLinkProgram,

			allocateBufferMemory,
			freeMemory,
			signalEvent,
			addCallback,
		};

		#define REGIST_FUNC0(func)															void func(){registCommand(DeferredCommand::func);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}
		#define REGIST_FUNC1(func,T1,v1)													void func(T1 v1){registCommand(DeferredCommand::func,v1);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}
		#define REGIST_FUNC2(func,T1,v1,T2,v2)												void func(T1 v1,T2 v2){registCommand(DeferredCommand::func,v1,v2);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}
		#define REGIST_FUNC3(func,T1,v1,T2,v2,T3,v3)										void func(T1 v1,T2 v2,T3 v3){registCommand(DeferredCommand::func,v1,v2,v3);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}
		#define REGIST_FUNC4(func,T1,v1,T2,v2,T3,v3,T4,v4)									void func(T1 v1,T2 v2,T3 v3,T4 v4){registCommand(DeferredCommand::func,v1,v2,v3,v4);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}
		#define REGIST_FUNC5(func,T1,v1,T2,v2,T3,v3,T4,v4,T5,v5)							void func(T1 v1,T2 v2,T3 v3,T4 v4,T5 v5){registCommand(DeferredCommand::func,v1,v2,v3,v4,v5);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}
		#define REGIST_FUNC6(func,T1,v1,T2,v2,T3,v3,T4,v4,T5,v5,T6,v6)						void func(T1 v1,T2 v2,T3 v3,T4 v4,T5 v5, T6 v6){registCommand(DeferredCommand::func,v1,v2,v3,v4,v5,v6);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}
		#define REGIST_FUNC7(func,T1,v1,T2,v2,T3,v3,T4,v4,T5,v5,T6,v6,T7,v7)				void func(T1 v1,T2 v2,T3 v3,T4 v4,T5 v5, T6 v6, T7 v7){registCommand(DeferredCommand::func,v1,v2,v3,v4,v5,v6,v7);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}
		#define REGIST_FUNC8(func,T1,v1,T2,v2,T3,v3,T4,v4,T5,v5,T6,v6,T7,v7,T8,v8)			void func(T1 v1,T2 v2,T3 v3,T4 v4,T5 v5, T6 v6, T7 v7, T8 v8){registCommand(DeferredCommand::func,v1,v2,v3,v4,v5,v6,v7,v8);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}
		#define REGIST_FUNC9(func,T1,v1,T2,v2,T3,v3,T4,v4,T5,v5,T6,v6,T7,v7,T8,v8,T9,v9)	void func(T1 v1,T2 v2,T3 v3,T4 v4,T5 v5, T6 v6, T7 v7, T8 v8,T9 v9){registCommand(DeferredCommand::func,v1,v2,v3,v4,v5,v6,v7,v8,v9);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}

		#define REGIST_GEN(func,T1,v1,T2,v2)												void func(T1 v1,T2 v2){ std::for_each(v2,v2+v1,[](ObjectID& obj) { obj = new _ObjectID();}); registCommand(DeferredCommand::func,v1,v2);MKGL_ASSERT(m_bufferCurrent<m_bufferEnd);}
		template<typename T>
		inline uint32_t* writeOne(uint32_t* buffer, T v)
		{
			static_assert(sizeof(T) % sizeof(uint32_t) == 0, "writeOne<T>. sizeof(T) must be a multiple of 4.");
			constexpr auto num = sizeof(T) / sizeof(uint32_t);
			if constexpr (num==1) {
				*buffer++ = *(uint32_t*)&v;
			}
			else if constexpr (num == 2) {
				*buffer++ = *((uint32_t*)&v);
				*buffer++ = *((uint32_t*)&v + 1);
			}
			else {
				auto p = (uint32_t*)&v;
				for (unsigned i = 0; i < num; i++)
				{
					*buffer++ = *p++;
				}
			}
			return buffer;
		}
		//“ÁŽê‰»
		template<> inline uint32_t* writeOne(uint32_t* buffer, bool v) { return writeOne(buffer, v ? 1 : 0);  }
		template<> inline uint32_t* writeOne(uint32_t* buffer, GLboolean v) { return writeOne(buffer, (int)v); }


		class CDeferredContext {
		public:
			CDeferredContext();
			CDeferredContext(size_t bufferSize);
			~CDeferredContext();
			void initialize(size_t bufferSize);
			void submit();

			REGIST_FUNC2(glBindBuffer, BufferTarget, target, ObjectID, buffer)
			REGIST_FUNC4(glBufferData, BufferTarget, target, GLsizeiptr, size, const void*, data, GLenum, usage)
			REGIST_FUNC4(glBufferSubData, BufferTarget, target, GLintptr, offset, GLsizeiptr, size, const void*, data)

			REGIST_FUNC1(glClear,GLbitfield,mask)
			REGIST_FUNC4(glClearColor,GLfloat,red,GLfloat, green, GLfloat, blue, GLfloat, alpha)
			REGIST_FUNC1(glClearDepthf,GLfloat, d)
			REGIST_FUNC1(glClearStencil,GLint,s)
			REGIST_FUNC4(glColorMask,GLboolean, red, GLboolean, green, GLboolean,blue, GLboolean, alpha)

			ObjectID glCreateProgram(void) {
				auto newProg = (ObjectID)new _ObjectID();
				registCommand(DeferredCommand::glCreateProgram, newProg);
				return newProg;
			};
			ObjectID glCreateShader(GLenum type) {
				auto newShader = (ObjectID)new _ObjectID();
				registCommand(DeferredCommand::glCreateShader, newShader, type);
				return newShader;
			};


			REGIST_FUNC2(glDeleteBuffers,GLsizei, n, const ObjectID*, buffers)
			REGIST_FUNC2(glDeleteFramebuffers,GLsizei, n, const ObjectID*, framebuffers)
			REGIST_FUNC1(glDeleteProgram,GLuint, program)
			REGIST_FUNC2(glDeleteRenderbuffers,GLsizei, n, const ObjectID*, renderbuffers)
			REGIST_FUNC1(glDeleteShader,GLuint, shader)
			REGIST_FUNC2(glDeleteTextures,GLsizei, n, const ObjectID*, textures)


			REGIST_GEN(glGenBuffers, GLsizei, n, ObjectID*, framebuffers)
			REGIST_GEN(glGenFramebuffers, GLsizei, n, ObjectID*, framebuffers)
			REGIST_GEN(glGenTextures, GLsizei, n, ObjectID*, textures)

			REGIST_FUNC2(glBindTexture, GLenum, target, ObjectID, texture)
			REGIST_FUNC2(glBindFramebuffer, GLenum, target, ObjectID, framebuffer)
			REGIST_FUNC5(glFramebufferTexture2D, GLenum, target, GLenum, attachment, GLenum, textarget, ObjectID, texture, GLint, level);
			REGIST_FUNC9(glTexImage2D,GLenum, target, GLint, level, GLint, internalformat, GLsizei, width, GLsizei, height, GLint, border, GLenum, format, GLenum, type, const void*, pixels)

			REGIST_FUNC4(glShaderSource,ObjectID, shader, GLsizei, count, const GLchar* const*, string, const GLint*, length)
			REGIST_FUNC1(glCompileShader, ObjectID, shader)
			REGIST_FUNC3(glGetShaderiv, ObjectID, shader, GLenum, pname, GLint*, params)
			REGIST_FUNC2(glAttachShader,ObjectID, program, ObjectID, shader);
			REGIST_FUNC1(glLinkProgram,ObjectID, program);

			void* allocateBufferMemory(size_t size) {
				uint32_t intCnt = uint32_t(size + 3) / sizeof(uint32_t);
				registCommand(DeferredCommand::allocateBufferMemory, intCnt);
				auto p = m_bufferCurrent;
				m_bufferCurrent += intCnt;
				return p;
			}
			REGIST_FUNC1(freeMemory, void*, mem)
			REGIST_FUNC1(signalEvent, System::CSignalEvent*, evt)
			void addCallback(std::function<void(void)> function) {
				auto pFunc = new (allocateBufferMemory(sizeof(function))) std::function<void(void)>(function);
				registCommand(DeferredCommand::addCallback, pFunc);
			}

		private:
			uint32_t* m_bufferStart;
			uint32_t* m_bufferEnd;
			uint32_t* m_bufferCurrent;

		private:

			void write() {}//dummy
			template<typename T, typename... Types>
			void write(T v, Types... types) {
				m_bufferCurrent = writeOne(m_bufferCurrent, v);
				write(types...);
			}

			template<typename... Types>
			void registCommand(DeferredCommand command, Types... types) {
				write(command, types...);
			}



		private:
		};

	}
}


#endif //!_GRAPHICS_DEFERRED_CONTEXT_H