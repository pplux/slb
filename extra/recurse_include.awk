BEGIN {  }
/#include ".*"/ {
	name =substr($2,2,length($2)-2);
	deps_count[FILENAME]++;
	deps[FILENAME,deps_count[FILENAME]]=name
	result[FILENAME]=1
}
function recursive_walk(name) {
	if (name in result) {
		delete result[name];
		for(j = 1; j <= deps_count[name]; ++j) {
			recursive_walk(deps[name,j]);
		}
		printf("#include \"%s\"\n",name)
	}
}
END {
	for(i in deps_count) {
		recursive_walk(i);
	}
}
