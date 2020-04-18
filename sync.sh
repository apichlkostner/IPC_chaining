# copies the sources to the raspi
rsync --numeric-ids --exclude=/build/ -avz ~/swdev/communication/ raspi3:~/workspace/communication
