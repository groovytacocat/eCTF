#!/bin/bash

WORK_DIR="$(find ~ -type d -name "eCTF" 2> /dev/null)"

main(){
	if [[ $(pwd) != "$WORK_DIR" ]]; then
		cd $WORK_DIR
	fi

	if [[ "$VIRTUAL_ENV" == "" ]]; then
		source ./.venv/bin/activate
	fi

	echo -e "Installing build tools\n"
	python -m pip install ./tools/
	python -m pip install -e ./design/
}
main
