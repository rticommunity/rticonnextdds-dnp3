#!/bin/bash
# ------------------------------------------------------------------------------
# (c) 2005-2014 Copyright, Real-Time Innovations, Inc.  All rights reserved.
# RTI grants Licensee a license to use, modify, compile, and create derivative
# works of the Software.  Licensee has the right to distribute object form only
# for use with RTI products.  The Software is provided "as is", with no warranty
# of any type, including any warranty for fitness for any purpose. RTI is under
# no obligation to maintain or support the Software.  RTI shall not be liable for
# any incidental or consequential damages arising out of the use or inability to
# use the software.
# ------------------------------------------------------------------------------

if [[ -z "$BASE_DIR" ]]; then
    BASE_DIR=$(cd $(dirname "$0"); cd ../../../../; pwd -P)
fi

WORKING_DIR=${BASE_DIR}/rs_adapter/resource/dnp3/simple-test

DDS_DOMAIN=${DDS_DOMAIN:-42}
echo DDS_DOMAIN=${DDS_DOMAIN}

echo NDDSHOME=${NDDSHOME}
echo ROUTER_HOME=${ROUTER_HOME}
echo PLATFORM=${PLATFORM}

DNP3_HOME=${DNP3_HOME:-${BASE_DIR}/dnp3}
echo DNP3_HOME=${DNP3_HOME}

echo BASE_DIR=${BASE_DIR}
echo WORKING_DIR=${WORKING_DIR}

LD_LIBRARY_PATH=${ROUTER_HOME}/bin/${PLATFORM}
LD_LIBRARY_PATH=${BASE_DIR}/lib/${PLATFORM}:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH
echo LD_LIBRARY_PATH=${LD_LIBRARY_PATH}

#------------------------------------------------------------------------------

if [[ "$DEBUG" = "1" ]]; then
	RS_CONFIG_FILE=simple-test-RS-debug.xml
	SIMPLE=0
else
	if [[ "$SIMPLE" = "1" ]]; then
		RS_CONFIG_FILE=simple-test-RS-simpler.xml
	else
		RS_CONFIG_FILE=simple-test-RS.xml
		SIMPLE=0
	fi
	DEBUG=0
fi

if [[ "$DEBUG" = "1" ]]; then
	DNP3_RELEASE=debug
else
	DNP3_RELEASE=release
	DEBUG=0
fi

RTIDDSSPY_CMD=${NDDSHOME}/lib/${PLATFORM}/rtiddsspy
if [[ -z "$RTIDDSSPY_CMD" ]]; then
    RTIDDSSPY_CMD=${NDDSHOME}/scripts/rtiddsspy
fi

RS_VERBOSITY=3
RS_CONF1=rs1
RS_CONF2=rs2

DDS_DOMAIN1=${DDS_DOMAIN1:-$DDS_DOMAIN}
DDS_TOPIC1_NAME=DNP3Bus

DDS_DOMAIN2=${DDS_DOMAIN2:-$DDS_DOMAIN}
DDS_TOPIC2_NAME=DNP3BusReplica

DNP3_DOMAIN=$DDS_DOMAIN1; # this is read in the router's xml config

MASTER_ADDR=102
SLAVE_ADDR=11

SLAVE_TEST_LOG=slave_testset.log
MASTER_TEST_LOG=master_testset.log

function print_run_config {

	echo "Configuration:"
	echo "    BASE_DIR=$BASE_DIR"
	echo "    RS_CONFIG_FILE=$RS_CONFIG_FILE"
	echo "    PLATFORM=$PLATFORM"
	echo "    DNP3_RELEASE=$DNP3_RELEASE"

}

for pid in `ps aux | grep rtiroutingservice | grep -v grep | awk '{print $2;}'`; do
    echo "WARNING: existing Routing Service process: "$pid
done

TAB1_TITLE="Routing Service 1 (DNP3 to DDS)"
TAB1_CMD="env \
            LD_LIBRARY_PATH=${LD_LIBRARY_PATH} \
			PLATFORM=${PLATFORM} \
            DNP3_DOMAIN=${DNP3_DOMAIN} \
                ${ROUTER_HOME}/bin/${PLATFORM}/rtiroutingservice \
                    -cfgFile ${WORKING_DIR}/${RS_CONFIG_FILE} \
                    -cfgName ${RS_CONF1} -verbosity ${RS_VERBOSITY}"

TAB2_TITLE="Routing Service 2 (DDS back to DDS)"
TAB2_CMD="env \
            LD_LIBRARY_PATH=${LD_LIBRARY_PATH} \
			PLATFORM=${PLATFORM} \
            DNP3_DOMAIN=${DNP3_DOMAIN} \
                ${ROUTER_HOME}/bin/${PLATFORM}/rtiroutingservice \
                    -cfgFile ${WORKING_DIR}/${RS_CONFIG_FILE} \
                    -cfgName ${RS_CONF2} -verbosity ${RS_VERBOSITY}"

TAB3_TITLE="Master${MASTER_ADDR}"
TAB3_CMD="${DNP3_HOME}/TestSet/Linux_i686/${DNP3_RELEASE}/testset.exe"
TAB3_CMD="${TAB3_CMD} -F ${WORKING_DIR}/dnp3_master_${MASTER_ADDR}_proxied.xml"

TAB4_TITLE="Master${MASTER_ADDR} (log)"
TAB4_CMD="tail -f ${WORKING_DIR}/$MASTER_TEST_LOG"

TAB5_TITLE="Slave${SLAVE_ADDR}"
TAB5_CMD="${DNP3_HOME}/TestSet/Linux_i686/${DNP3_RELEASE}/testset.exe"
TAB5_CMD="${TAB5_CMD} -S -F ${WORKING_DIR}/dnp3_slave_${SLAVE_ADDR}.xml"

TAB6_TITLE="Slave${SLAVE_ADDR} (log)"
TAB6_CMD="tail -f ${WORKING_DIR}/$SLAVE_TEST_LOG"

TAB7_TITLE="Spy(${DDS_TOPIC1_NAME})"
TAB7_CMD="${RTIDDSSPY_CMD} -domainId ${DDS_DOMAIN1}"
TAB7_CMD="${TAB7_CMD} -printSample"

TAB8_TITLE="Shell"
TAB8_CMD="bash"

echo "" > ${WORKING_DIR}/${MASTER_TEST_LOG}
echo "" > ${WORKING_DIR}/${SLAVE_TEST_LOG} 

print_run_config;

if [[ -n "${LAUNCH_WITH_TMUX:+_}" ]]; then

    # gnome-terminal has a bug in some versions where processes aren't killed 
    # when tabs are closed. For this reason we offer tmux as well.

    set -o nounset

    # verify if there's a tmux session already up (we will use the first one 
    # available)
    tmux_sessions=$(tmux ls)
    if [[ -z "$tmux_sessions" ]]; then
        echo 1>&2 "error: check for existing tmux server fails"
        exit 64 ; # service unavailable
    fi

    # create a 2x4 layout of shell panes.
    tmux new-window -n "simple-test"

    # first create 4 columns
    tmux select-pane  -t 0
    tmux split-window -h
    tmux split-window -h
    tmux select-pane  -t 0
    tmux split-window -h

    # now create two rows
    tmux select-pane  -t 0
    tmux split-window -v
    tmux select-pane  -t 2
    tmux split-window -v
    tmux select-pane  -t 4
    tmux split-window -v
    tmux select-pane  -t 6
    tmux split-window -v

    # now put all terminals in the working directory,
    # set their title,
    # and finally, have them run their command
    for (( paneId=0; $paneId < 8; ++paneId )); do

        # first display what we will run
        echo
        eval "echo \"Launching pane $paneId with title \
            '\${TAB$(($paneId+1))_TITLE}' and command:\""
        eval "echo \"\${TAB$(($paneId+1))_CMD}\""

        # change each terminal to the working directory we want
        eval "tmux send-keys \
            -t \"simple-test.${paneId}\" \
            \"cd ${WORKING_DIR}\" \
            C-m"

        # set each pane's title
        eval "tmux send-keys -t \"simple-test.${paneId}\" \
            \"echo -n \$'\\e'\\\"]0;\${TAB$(($paneId+1))_TITLE}\\\"\$'\a'\" \
            C-m"

        # have each pane run its command
        eval "tmux send-keys \
            -t \"simple-test.${paneId}\" \
            \"\${TAB$(($paneId+1))_CMD}\" \
            C-m"
    done

else

    echo "Launching: "
    echo ""
    echo "gnome-terminal"
    echo "    --working-directory=\"${WORKING_DIR}\" "
    echo "    --maximize"
    echo "    --hide-menubar"
    echo "    --tab --title=\"${TAB1_TITLE}\" --command=\"${TAB1_CMD}\""
    echo "    --tab --title=\"${TAB2_TITLE}\" --command=\"${TAB2_CMD}\""
    echo "    --tab --title=\"${TAB3_TITLE}\" --command=\"${TAB3_CMD}\""
    echo "    --tab --title=\"${TAB4_TITLE}\" --command=\"${TAB4_CMD}\""
    echo "    --tab --title=\"${TAB5_TITLE}\" --command=\"${TAB5_CMD}\""
    echo "    --tab --title=\"${TAB6_TITLE}\" --command=\"${TAB6_CMD}\""
    echo "    --tab --title=\"${TAB7_TITLE}\" --command=\"${TAB7_CMD}\""
    echo "    --tab --title=\"${TAB8_TITLE}\" --command=\"${TAB8_CMD}\""

    AVOIDSETTITLE=1 \
    gnome-terminal \
            --working-directory="${WORKING_DIR}" \
            --maximize \
            --hide-menubar \
            --tab --title="${TAB1_TITLE}" --command="${TAB1_CMD}" \
            --tab --title="${TAB2_TITLE}" --command="${TAB2_CMD}" \
            --tab --title="${TAB3_TITLE}" --command="${TAB3_CMD}" \
            --tab --title="${TAB4_TITLE}" --command="${TAB4_CMD}" \
            --tab --title="${TAB5_TITLE}" --command="${TAB5_CMD}" \
            --tab --title="${TAB6_TITLE}" --command="${TAB6_CMD}" \
            --tab --title="${TAB7_TITLE}" --command="${TAB7_CMD}" \
            --tab --title="${TAB8_TITLE}" --command="${TAB8_CMD}" \


    sleep 3
    for pid in `ps aux | grep rtiroutingservice | grep -v grep | awk '{print $2;}'`; do
    echo "New Routing Service process created: "$pid
    done

fi

