/**
 * Nomic
 * Copyright (C) 2017 David Jolly
 *
 * Nomic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nomic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/graphic/fbo.h"
#include "../../include/trace.h"
#include "./fbo_type.h"

namespace nomic {

	namespace graphic {

		fbo::fbo(void) :
			nomic::core::primitive(PRIMITIVE_FBO)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		fbo::fbo(
			__in GLenum target,
			__in GLint level,
			__in GLint internal,
			__in GLsizei width,
			__in GLsizei height,
			__in GLint border,
			__in GLenum format,
			__in GLenum type,
			__in const GLvoid *data,
			__in GLenum attachment,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			) :
				nomic::core::primitive(PRIMITIVE_FBO)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,
	"Target=%x, Level=%i, Internal=%i, Dimensions={%i, %i}, Border=%i, Format=%x, Type=%x, Data=%p, Attachment=%x, Wrap={%x, %x}, Filter={%x, %x}",
				target, level, internal, width, height, border, format, type, data, attachment, wrap_s, wrap_t, 
				filter_min, filter_mag);

			set(target, level, internal, width, height, border, format, type, data, wrap_s, wrap_t, filter_min, filter_mag);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		fbo::fbo(
			__in const fbo &other
			) :
				nomic::core::primitive(other),
				m_texture(other.m_texture)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		fbo::~fbo(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		fbo &
		fbo::operator=(
			__in const fbo &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::core::primitive::operator=(other);
				m_texture = other.m_texture;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		fbo::bind(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glBindFramebuffer, GL_FRAMEBUFFER, m_handle);
			GL_CHECK(LEVEL_WARNING, glActiveTexture, GL_TEXTURE1);
			m_texture.bind();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		fbo::set(
			__in GLenum target,
			__in GLint level,
			__in GLint internal,
			__in GLsizei width,
			__in GLsizei height,
			__in GLint border,
			__in GLenum format,
			__in GLenum type,
			__in const GLvoid *data,
			__in GLenum attachment,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			)
		{
			GLenum result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,
	"Target=%x, Level=%i, Internal=%i, Dimensions={%i, %i}, Border=%i, Format=%x, Type=%x, Data=%p, Attachment=%x, Wrap={%x, %x}, Filter={%x, %x}",
				target, level, internal, width, height, border, format, type, data, attachment, wrap_s, wrap_t, 
				filter_min, filter_mag);

			GL_CHECK(LEVEL_WARNING, glBindFramebuffer, GL_FRAMEBUFFER, m_handle);
			m_texture.set(target, level, internal, width, height, border, format, type, data, wrap_s, wrap_t, filter_min, filter_mag);
			GL_CHECK(LEVEL_WARNING, glFramebufferTexture, GL_FRAMEBUFFER, attachment, m_texture.handle(), 0);
			GL_CHECK(LEVEL_WARNING, glDrawBuffer, GL_NONE);
			GL_CHECK(LEVEL_WARNING, glReadBuffer, GL_NONE);

			GL_CHECK_RESULT(LEVEL_WARNING, glCheckFramebufferStatus, result, GL_FRAMEBUFFER);
			if(result != GL_FRAMEBUFFER_COMPLETE) {
				THROW_NOMIC_GRAPHIC_FBO_EXCEPTION(NOMIC_GRAPHIC_FBO_EXCEPTION_INCOMPLETE);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		fbo::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_FBO_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::primitive::to_string(verbose)
					<< ", Texture=" << m_texture.to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
