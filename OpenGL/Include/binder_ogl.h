/*
 *  binder_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 7/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_BINDER_H
#define _TGEN_OPENGL_BINDER_H

#include "platform.h"
#include "prefix_ogl.h"

#ifndef GL_VERSION_2_0
#define _TGEN_OPENGL_NEED_BINDING
#endif

#ifdef _TGEN_OPENGL_NEED_BINDING
typedef char GLchar;
typedef void *GLhandle;

typedef long GLintptr;
typedef long GLsizeiptr;
typedef long GLintptrARB;
typedef long GLsizeiptrARB;

typedef void (*BindBufferARBPtr)(GLenum target, GLuint buffer);
typedef void (*GenBuffersARBPtr)(GLsizei n, GLuint * buffers);
typedef void (*BufferDataARBPtr)(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
typedef void (*BufferSubDataARBPtr) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
typedef void (*DeleteBuffersARBPtr) (GLsizei n, const GLuint *buffers);
typedef void * (*MapBufferARBPtr)(GLenum target, GLenum access);
typedef GLboolean (*UnmapBufferARBPtr)(GLenum target);

typedef void (*ActiveTextureARBPtr) (GLenum target);
typedef void (*ClientActiveTextureARBPtr) (GLenum target);

typedef void (*DrawRangeElementsEXTPtr) (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *);

typedef void (*DeleteShaderPtr) (GLuint shader);
typedef void (*AttachShaderPtr) (GLuint program, GLuint shader);
typedef void (*LinkProgramPtr) (GLuint program);
typedef void (*GetProgramivPtr) (GLuint program, GLenum pname, GLint *params);
typedef void (*GetProgramInfoLogPtr) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef GLint (*GetUniformLocationPtr) (GLuint program, const GLchar *name);
typedef GLint (*GetAttribLocationPtr) (GLuint program, const GLchar *name);
typedef void (*UseProgramPtr) (GLuint program);
typedef void (*Uniform1iPtr) (GLint location, GLint v0);
typedef void (*Uniform4fPtr) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (*VertexAttrib4fPtr) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (*Uniform3fPtr) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (*VertexAttrib3fPtr) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (*Uniform2fPtr) (GLint location, GLfloat v0, GLfloat v1);
typedef void (*VertexAttrib2fPtr) (GLuint index, GLfloat x, GLfloat y);
typedef GLuint (*CreateShaderPtr) (GLenum type);
typedef void (*ShaderSourcePtr) (GLuint shader, GLsizei count, const GLchar* *string, const GLint *length);
typedef void (*CompileShaderPtr) (GLuint shader);
typedef void (*GetShaderivPtr) (GLuint shader, GLenum pname, GLint *params);
typedef void (*GetShaderInfoLogPtr) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef GLuint (*CreateProgramPtr) (void);

typedef void (*DeleteFramebuffersEXTPtr) (GLsizei n, const GLuint *framebuffers);
typedef void (*GenFramebuffersEXTPtr) (GLsizei n, GLuint *framebuffers);
typedef void (*FramebufferTexture2DEXTPtr) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (*BindFramebufferEXTPtr) (GLenum target, GLuint framebuffer);
typedef GLenum (*CheckFramebufferStatusEXTPtr) (GLenum target);

typedef void (*DrawBuffersARBPtr) (GLsizei n, const GLenum *bufs);

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892    // VBO ARB
#endif

#ifndef GL_BGR
#define GL_BGR 0x80E0		// BGR EXT
#endif

#ifndef GL_BGRA
#define GL_BGRA 0x80E1	// BGRA EXT
#endif

#ifndef GL_TEXTURE0
#define GL_TEXTURE0 0x84C0  // multitexture ARB
#define GL_MAX_TEXTURE_UNITS 0x84E2
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F  // GL_SGIS_texture_edge_clamp
#endif

#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER 0x812D  // GL_SGIS_texture_border_clamp
#endif

#ifndef GL_DEPTH_COMPONENT16  // GL_SGIX_depth_texture
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#endif

#ifndef GL_RGB32F // GL_ARB_texture_float
#define GL_TEXTURE_RED_TYPE             0x8C10
#define GL_TEXTURE_GREEN_TYPE           0x8C11
#define GL_TEXTURE_BLUE_TYPE            0x8C12
#define GL_TEXTURE_ALPHA_TYPE           0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE       0x8C14
#define GL_TEXTURE_INTENSITY_TYPE       0x8C15
#define GL_TEXTURE_DEPTH_TYPE           0x8C16
#define GL_UNSIGNED_NORMALIZED          0x8C17
#define GL_RGBA32F_ARB                      0x8814
#define GL_RGB32F_ARB                       0x8815
#define GL_ALPHA32F                     0x8816
#define GL_INTENSITY32F                 0x8817
#define GL_LUMINANCE32F                 0x8818
#define GL_LUMINANCE_ALPHA32F           0x8819
#define GL_RGBA16F_ARB                      0x881A
#define GL_RGB16F_ARB                       0x881B
#define GL_ALPHA16F                     0x881C
#define GL_INTENSITY16F                 0x881D
#define GL_LUMINANCE_ALPHA16F           0x881F
#endif

#ifndef GL_COLOR_MATRIX // GL_SGI_color_matrix
#define GL_COLOR_MATRIX 0x80B1
#endif

#ifndef GL_ARRAY_BUFFER_ARB  // GL_ARB_vertex_buffer_object
#define GL_ARRAY_BUFFER_ARB                                0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB                        0x8893
#define GL_STREAM_DRAW_ARB                                 0x88E0
#define GL_STREAM_READ_ARB                                 0x88E1
#define GL_STREAM_COPY_ARB                                 0x88E2
#define GL_STATIC_DRAW_ARB                                 0x88E4
#define GL_STATIC_READ_ARB                                 0x88E5
#define GL_STATIC_COPY_ARB                                 0x88E6
#define GL_DYNAMIC_DRAW_ARB                                0x88E8
#define GL_DYNAMIC_READ_ARB                                0x88E9
#define GL_DYNAMIC_COPY_ARB                                0x88EA
#define GL_READ_ONLY_ARB                                   0x88B8
#define GL_WRITE_ONLY_ARB                                  0x88B9
#define GL_READ_WRITE_ARB                                  0x88BA

#endif

#ifndef GL_MAX_ELEMENTS_VERTICES // GL_EXT_draw_range_elements 
#define GL_MAX_ELEMENTS_VERTICES      0x80E8
#define GL_MAX_ELEMENTS_INDICES       0x80E9
#endif

#ifndef GL_VERSION_2_0
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_SHADER_TYPE                    0x8B4F
#define GL_DELETE_STATUS                  0x8B80
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_SHADER_SOURCE_LENGTH           0x8B88
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C

#define GL_FRAMEBUFFER_EXT                 0x8D40
#define GL_MAX_COLOR_ATTACHMENTS_EXT       0x8CDF

#define GL_COLOR_ATTACHMENT0_EXT           0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT           0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT           0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT           0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT           0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT           0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT           0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT           0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT           0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT           0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT          0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT          0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT          0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT          0x8CED
#define GL_COLOR_ATTACHMENT14_EXT          0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT          0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT            0x8D00
#define GL_STENCIL_ATTACHMENT_EXT          0x8D20
#define GL_FRAMEBUFFER_COMPLETE_EXT                        0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT           0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT   0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT           0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT              0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT          0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT          0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT                     0x8CDD
#define GL_FRAMEBUFFER_BINDING_EXT         0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT        0x8CA7
#define GL_MAX_COLOR_ATTACHMENTS_EXT       0x8CDF
#define GL_MAX_RENDERBUFFER_SIZE_EXT       0x84E8
#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT 0x0506

#endif

extern BindBufferARBPtr glBindBufferARB;
extern GenBuffersARBPtr glGenBuffersARB;
extern BufferDataARBPtr glBufferDataARB;
extern MapBufferARBPtr glMapBufferARB;
extern UnmapBufferARBPtr glUnmapBufferARB;
extern BufferSubDataARBPtr glBufferSubDataARB;
extern DeleteBuffersARBPtr glDeleteBuffersARB;

extern ActiveTextureARBPtr glActiveTexture;
extern ClientActiveTextureARBPtr glClientActiveTexture;

extern DrawRangeElementsEXTPtr glDrawRangeElements;

extern DeleteShaderPtr glDeleteShader;
extern AttachShaderPtr glAttachShader;
extern LinkProgramPtr glLinkProgram;
extern GetProgramivPtr glGetProgramiv;
extern GetProgramInfoLogPtr glGetProgramInfoLog;
extern GetUniformLocationPtr glGetUniformLocation;
extern GetAttribLocationPtr glGetAttribLocation;
extern UseProgramPtr glUseProgram;
extern Uniform1iPtr glUniform1i;
extern Uniform4fPtr glUniform4f;
extern VertexAttrib4fPtr glVertexAttrib4f;
extern Uniform3fPtr glUniform3f;
extern VertexAttrib3fPtr glVertexAttrib3f;
extern Uniform2fPtr glUniform2f;
extern VertexAttrib2fPtr glVertexAttrib2f;
extern CreateShaderPtr glCreateShader;
extern ShaderSourcePtr glShaderSource;
extern CompileShaderPtr glCompileShader;
extern GetShaderivPtr glGetShaderiv;
extern GetShaderInfoLogPtr glGetShaderInfoLog;
extern CreateProgramPtr glCreateProgram;

extern DeleteFramebuffersEXTPtr glDeleteFramebuffersEXT;
extern GenFramebuffersEXTPtr glGenFramebuffersEXT;
extern FramebufferTexture2DEXTPtr glFramebufferTexture2DEXT;
extern BindFramebufferEXTPtr glBindFramebufferEXT;
extern CheckFramebufferStatusEXTPtr glCheckFramebufferStatusEXT;

extern DrawBuffersARBPtr glDrawBuffersARB;



#endif // !_TGEN_OPENGL_NEED_BINDING

namespace TGen {
	namespace OpenGL {
		void BindFunctions();

	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_BINDER_H