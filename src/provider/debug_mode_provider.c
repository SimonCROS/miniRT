static int	provider(int debug)
{
	static int	saved_debug;

	if (debug != -1)
		saved_debug = debug;
	return (saved_debug);
}

int	is_debug_enabled(void)
{
	return (provider(-1));
}

int	set_debug(int debug)
{
	return (provider(!!debug));
}
