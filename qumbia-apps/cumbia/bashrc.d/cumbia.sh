#!/bin/bash


# the menu is printed like this
# +---------------------------------------------------------------------------------+
# |                              cumbia launcher usage:                             |
# +---------------------------------------------------------------------------------+
# |                                                                                 |
# |cumbia new project            | write a new cumbia project                       |
# |cumbia new control            | write a new cumbia control widget                |
# |cumbia ui make                | call cuuimake                                    |
# |cumbia client                 | run the cumbia generic client                    |
# +---------------------------------------------------------------------------------+

print_menu() {

		printf "\n"
		printf "+%.24s--%.38s--%.58s+\n" "--------------------------------" "--------------------------------------------" "-----------------------------------------------------------------"
		printf "|%-24s| %-38s| %-58s|\n" "cumbia launcher options" "description" "options"
		printf "|%.24s| %.38s| %.58s|\n" "--------------------------------" "---------------------------------------------" "------------------------------------------------------------------"
		printf "|%-24s| %-38s| %-58s|\n" "" ""
		printf "|%-24s| %-38s| %-58s|\n" "cumbia new project" "write a new cumbia project" ""
		printf "|%-24s| %-38s| %-58s|\n" "cumbia new control" "write a new cumbia control widget"
		printf "|%-24s| %-38s| %-58s|\n" "cumbia import" "try to migrate a qtango project" ""
		printf "|%-24s| %-38s| %-58s|\n" "cumbia ui make" "call cuuimake" "[--help] | [--clean] | [--pre-clean | --qmake | --make]"
		printf "|%-24s| %-38s| %-58s|\n" "cumbia client" "run the cumbia generic client" "list of sources: test/device/1 [a/src/2 source:ai1]"
		printf "+%.24s--%.38s--%.58s+\n" "--------------------------------" "--------------------------------------------" "--------------------------------------------------------------------"
		printf "\n"
}

cumbia() {
	if [ $# -lt 1 ]; then
		print_menu
	
	elif [ $# -gt 1 ] && [ $1 == "new" ] && [ $2 == "project" ]; then
		qumbiaprojectwizard ${@:3}
	elif [ $# -gt 1 ] && [ $1 == "new" ] && [ $2 == "control" ]; then
		qumbianewcontrolwizard ${@:3}
	elif [ $# -gt 0 ] && [ $1 == "ui" ] && [ $2 == "make" ]; then
		cuuimake  ${@:3}
	elif [ $# -gt 0 ] && [ $1 == "client" ]; then
		cumbia_client ${@:2}
	elif [ $# -gt 0 ] && [ $1 == "import" ]; then
		qumbiaprojectwizard --from-qtango ${@:2}
	else
		print_menu
		echo -e "\e[1;31;4mcumbia\e[0m: unrecognized command line arguments \"\e[1;31m$@\e[0m\"\n"
	fi
}

export -f cumbia
