#ifndef STK_KEYCODE_H
#define STK_KEYCODE_H

namespace stk
{
	// FIXME: add all the keycodes
	enum keycode
	{
		// ascii
		key_null, key_soh, key_stk, key_etx, key_eot, key_enq, key_ack, key_bell, 
		key_backspace, key_tab, key_linefeed, key_vtab, key_ff, key_enter, key_so, key_si, // FIXME: I use enter for ascii carriagereturn, is this right?
		key_dle, key_dc1, key_dc2, key_dc3, key_dc4, key_nak, key_syn, key_etb,
		key_can, key_em, key_sub, key_esc, key_fs, key_gs, key_rs, key_us,
		key_space, key_exclamation, key_dquote, key_pound, key_dollar, key_percent, key_ampersand, key_squote, 
		key_lparen, key_rparen, key_asterik, key_plus, key_comma, key_hyphen, key_period, key_slash,
		key_0, key_1, key_2, key_3, key_4, key_5, key_6, key_7, 
		key_8, key_9, key_colon, key_semicolon, key_lt, key_equal, key_gt, key_question,
		key_at, key_A, key_B, key_C, key_D, key_E, key_F, key_G,
		key_H, key_I, key_J, key_K, key_L, key_M, key_N, key_O,
		key_P, key_Q, key_R, key_S, key_T, key_U, key_V, key_W,
		key_X, key_Y, key_Z, key_lbracket, key_backslash, key_rbracket, key_carot, key_underscore,
		key_tick, key_a, key_b, key_c, key_d, key_e, key_f, key_g, 
		key_h, key_i, key_j, key_k, key_l, key_m, key_n, key_o,
		key_p, key_q, key_r, key_s, key_t, key_u, key_v, key_w,
		key_x, key_y, key_z, key_lbrace, key_pipe, key_rbrace, key_tilde, key_del,

		// extended ascii
		key_128, key_129, key_130, key_131, key_132, key_133, key_134, key_135, 
		key_136, key_137, key_138, key_139, key_140, key_141, key_142, key_143,
		key_144, key_145, key_146, key_147, key_148, key149_, key_150, key_151,
		key_152, key_153, key_154, key_155, key_156, key_157, key_158, key_159,
		key_160, key_161, key_162, key_163, key_164, key_165, key_166, key_167,
		key_168, key_169, key_170, key_171, key_172, key_173, key_174, key_175,
		key_176, key_177, key_178, key_179, key_180, key_181, key_182, key_183,
		key_184, key_185, key_186, key_187, key_188, key_189, key_190, key_191,
		key_192, key_193, key_194, key_195, key_196, key_197, key_198, key_199,
		key_200, key_201, key_202, key_203, key_204, key_205, key_206, key_207,
		key_208, key_209, key_210, key_211, key_212, key_213, key_214, key_215,
		key_216, key_217, key_218, key_219, key_220, key_221, key_222, key_223,
		key_224, key_225, key_226, key_227, key_228, key_229, key_230, key_231,
		key_232, key_233, key_234, key_235, key_236, key_237, key_238, key_239,
		key_240, key_241, key_242, key_243, key_244, key_245, key_246, key_247,
		key_248, key_249, key_250, key_251, key_252, key_253, key_254, key_255,
		
		// standard 104 keys
		left_arrow,
		right_arrow,
		up_arrow,
		down_arrow,
		
		// special keys (ie remote controls, etc) 
		next,
		prev,
		play,
		pause,
		forward,
		rewind,
		stop,

		// an unkown key
		key_unknown
	};
} // namespace stk

#endif

