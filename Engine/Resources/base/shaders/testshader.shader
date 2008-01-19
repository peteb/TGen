#section vertex
void main() {
	gl_Position = ftransform();
	
	#ifdef USE_BLACK_COLOR
	gl_Color = vec4(1.0, 0.0, 0.0, 1.0);
	#elif defined(USE_CUSTOM_COLOR)
	gl_FrontColor = vec4(#COLOR_R#, #COLOR_G#, #COLOR_B#, 1.0);
	#else
	gl_Color = vec4(0.0, 1.0, 0.0, 1.0);
	#endif
}

