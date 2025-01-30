#!/bin/bash

WORK_DIR="$(find ~/ -type d -name "eCTF" 2> /dev/null)"

main(){
	echo -e  "Checking for project directory\n"
	if [[ $(pwd) != "$WORK_DIR" ]]; then 
		echo -e "\nMoving to $WORK_DIR\n"
		cd $WORK_DIR
	fi

	echo -e "Checking for venv\n"
	if [[ "$VIRTUAL_ENV" == "" ]]; then
		echo -e "\nEntering venv\n"
		source ./.venv/bin/activate
	fi

	echo -e "Checking for build tools\n"
	if [[ ! $(pip list | grep ectf) == *"ectf"* ]]; then
		echo -e "\nInstalling build tools\n"
		python -m pip install ./tools
		python -m pip install -e ./design
	fi

	echo -e "Checking for secrets directory\n"
	if [[ ! -d ./secrets ]]; then
		echo -e "Creating Secrets directory"
		mkdir secrets
	fi

	echo -e "Checking for secrets.json\n"
	if [[ ! -f ./secrets/secrets.json ]]; then
		echo -e "\nCreating dummy secrets.json\n"
		python -m ectf25_design.gen_secrets secrets/secrets.json 1 3 4
	fi

	echo -e "Beginning Uplink\n"
	python -m ectf25.uplink ./secrets/secrets.json localhost 2000 1:10:frames/x_c0.json
}

main
