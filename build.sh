RED="[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"

if [[ "$*" == *-install* ]]
then
    cmake -B build
    cd build
    make
    make package
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN} Editor build successfully${ENDCOLOR}"
    else
        echo -e "${RED}There was a problem while building Editor ${ENDCOLOR}"
    fi

else
    cmake -B build
    cd build
    make

    if [ $? -eq 0 ]; then
        echo -e "${GREEN} Editor build successfully${ENDCOLOR}"
    else
        echo -e "${RED}There was a problem while building Editor ${ENDCOLOR}"
    fi
fi
