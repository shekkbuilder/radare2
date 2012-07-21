/* radare - LGPL - Copyright 2009-2012 - Anton Kochkov */

static int cmd_type(void *data, const char *input) {
	RCore *core = (RCore*)data;
#if USED
	RAnalVarType *var;
	RListIter *iter;
	int i, ret, line = 0;
	ut64 addr_end = 0LL;
	ut64 addr = core->offset;
	char file[1024];
#endif
	switch (*input) {
	case '*':
		r_anal_type_list (core->anal->types, R_ANAL_TYPE_ANY, 1);
		break;
	case 'f':
		switch (input[1]) {
		/* Open $EDITOR and allow type type definition manually */
		// TODO: Show simple rules in ctype or simple template? */
		case '!':
			{
			char *out, *ctype = "";
			out = r_core_editor (core, ctype);
			r_anal_type_loadstring (core->anal->types, out);
			free (out);
			free (ctype);
			}
			break;
		case ' ':
			{
			const char *ptr, *filename = input + 2;
			ptr = strchr (filename, ' ');
			if (ptr && !ptr[1]) {
				r_anal_type_loadfile(core->anal->types, filename);
				eprintf ("Usage: tf name\n");
			} else eprintf ("Usage: tf[!] [name]\n");
			}
			break;
		default:
			eprintf ("Usage: tf[..]\n"
				" tf [path]    : load types from file\n");
			break;
		}
		break;
	case 'h':
		switch (input[1]) {
		case ' ':
			break;
		/* Convert type into format string */
		case '*':
			break;
		default:
			eprintf ("Usage: th[..]\n"
				"th [path] [name] : show definition of type\n");
			break;
		}
		break;
	case '-':
		if (input[1]!='*') {
			ut64 n = r_num_math (core->num, input + ((input[1] == ' ') ? 2 : 1));
			eprintf ("val 0x%"PFMT64x"\n", n);
			//TODO r_anal_type_del (core->anal->types, R_ANAL_TYPE_ANY, core->offset, i, "");
		} else r_anal_type_cleanup (core->anal->types, 0LL, UT64_MAX);
		break;
	case '\0':
	case '!':
		{
		char *out, *ctype = "";
		out = r_core_editor (core, ctype);
		r_anal_type_loadstring (core->anal->types, out);
		free (out);
		free (ctype);
		}
		break;
	case '?':
		eprintf (
		"Usage: t[-LCvsdfm?] [...]\n"
		" t*                     # list types info in r2 commands\n"
		" t- [name]              # delete type by its name. Use t-* to remove all types. Use t-! to open $EDITOR\n"
		" tf [path]              # losd types from C header file. Use tf! to open $EDITOR\n");
		break;
	}
	return R_TRUE;
}