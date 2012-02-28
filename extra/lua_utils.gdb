define luastack
	set $top = lua_gettop($arg0)
	set $i = 1
	while $i <= $top
		if lua_type($arg0,$i) == -1
			printf "stack - %i : No Value \n", $i
		else
			call lua_pushvalue($arg0,$i)
			set $str = lua_tolstring($arg0,-1,0x0)
			set $type = lua_typename($arg0,lua_type($arg0,$i))
			if $str
				printf "stack - %i : %s -> '%s'\n", $i, $type , $str
			else
				printf "stack - %i : %s\n", $i, $type
			end	
			call lua_settop($arg0,-2)
		end
		set $i = $i + 1
	end
