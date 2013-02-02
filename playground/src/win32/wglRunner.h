/*
 * videoRunner.h
 *
 *  Created on: 22/11/2012
 *      Author: Lean
 */

#ifndef VIDEORUNNER_H_
#define WGLRUNNER_H_

#include <gl\gl.h>
#include <gl\glu.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "win32apiRunner.h"

class WglRunner: public Win32apiRunner {
	protected:
		HDC hDeviceContext;
		unsigned int bitsPerPixel;
		HGLRC hRenderingContext;
		Logger *logger;

	public:
		WglRunner() :
				Win32apiRunner() {
			hDeviceContext = null;
			bitsPerPixel = 16;
			hRenderingContext = null;

			logger = Logger::getLogger("win32/wglRunner.h");
		}

		virtual ~WglRunner() {
			wglMakeCurrent(null, null);

			logger->debug("Rendering context deselected");

			if (this->hRenderingContext != null) {
				wglDeleteContext(this->hRenderingContext);
				logger->debug("Rendering context deleted");
			}

			if (this->hDeviceContext != null) {
				ReleaseDC(this->getWnd(), this->hDeviceContext);
				logger->debug("Device context released");
			}

		}
		virtual void afterLoop() {
			SwapBuffers(this->hDeviceContext);
		}
		virtual boolean init() {
			if (Win32apiRunner::init()) {
				if (!(this->hDeviceContext = GetDC(this->getWnd()))) {
					printLastError("Could not retrieve device context");
					return false;
				}

				logger->debug("Device context initialized");

				static PIXELFORMATDESCRIPTOR pfd = {
						sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
						1, // Version Number
						PFD_DRAW_TO_WINDOW | // Format Must Support Window
								PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
								PFD_DOUBLEBUFFER, // Must Support Double Buffering
						PFD_TYPE_RGBA, // Request An RGBA Format
						bitsPerPixel, // Select Our Color Depth
						0, 0, 0, 0, 0, 0, // Color Bits Ignored
						0, // No Alpha Buffer
						0, // Shift Bit Ignored
						0, // No Accumulation Buffer
						0, 0, 0, 0, // Accumulation Bits Ignored
						bitsPerPixel, // 16Bit Z-Buffer (Depth Buffer)
						0, // No Stencil Buffer
						0, // No Auxiliary Buffer
						PFD_MAIN_PLANE, // Main Drawing Layer
						0, // Reserved
						0, 0, 0 // Layer Masks Ignored
						};

				GLuint PixelFormat;

				if (!(PixelFormat = ChoosePixelFormat(this->hDeviceContext,
						&pfd))) {
					printLastError(
							"Error al intentar seleccionar el formato de píxel");
					return (false);
				}

				logger->debug("Pixel format selected");

				if (!SetPixelFormat(this->hDeviceContext, PixelFormat, &pfd)) {
					printLastError(
							"Error al intentar elegir el pixel format en el DC");
					return (false);
				}

				logger->debug("Pixel format set");

				if (!(this->hRenderingContext = wglCreateContext(
						this->hDeviceContext))) {
					printLastError("Error al intentar crear Rendering Context");
					return (false);
				}

				logger->debug("Rendering context created");

				if (!wglMakeCurrent(this->hDeviceContext,
						this->hRenderingContext)) {
					printLastError("Error al intentar elegir el RC en el DC");
					return (false);
				}

				logger->debug("Rendering context set");

				return true;
			}
			return false;
		}
};

#endif /* VIDEORUNNER_H_ */
