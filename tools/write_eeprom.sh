#!/bin/bash

SCRIPT=`basename "$0"`
echo -e "\tFRAMOS GmBH script $SCRIPT"

function show_usage() {
    echo "Writes contest of bin file to eeprom memory"
    echo "Script Usage: sudo ./write_eeprom.sh BUS_ID I2C_ADDR BIN_FILE"
    echo -e "\tBUS_ID      - BUS ID where EEPROM sits"
    echo -e "\tI2C_ADDR    - I2C address of EEPROM"
    echo -e "\tBIN_FILE    - path to BIN_FILE\n"
}

function err() 
{
    echo -e "[ERR]: \t$@" >&2
    echo "Exiting script..."
    exit 1
}

function parse_arguments() 
{
    # check if required arguments are provided
    if [ $# -lt 3 ]; then
        show_usage
        err "Required parameters are not provided!"
    fi

    I2C_BUS=$1
    SLAVE_ADD=$2
    BIN_FILE=$3
}

detect_platform() {
  local model m
  model="$(tr -d '\0' </proc/device-tree/model 2>/dev/null)"
  m="${model,,}"  # lowercase

  if [[ "$m" == *"agx orin"* ]]; then
    PLATFORM="AGX Orin"
    echo "Platform is AGX Orin"
  elif [[ "$m" == *"orin nano"* ]]; then
    PLATFORM="Nano"
    echo "Platform is Orin Nano/NX"
  else
    PLATFORM="Unknown"
    err "Unable to detect platfrom"
  fi
}

function validate_i2c_bus_selection()
{
    if ! $(i2cdetect -y ${I2C_BUS} 1> /dev/null 2>&1); then
        err "Selected I2C bus is not initialized!"
    fi
    if ! $(i2cget -y ${I2C_BUS} ${SLAVE_ADD} 1> /dev/null 2>&1); then
        err "No response from selected I2C slave address!"
    fi
}

function is_write_permitted()
{
    # Available eproom devices used by Framos changing this may harm your system
    AVB_SLAVE_ADD=( 0x54 0x55 0x56 )

    # Available I2C busses 
    if [ "$PLATFORM" = "AGX Orin" ]; then
        AVB_I2C_BUS=( 9 11 13 15 )
    elif [ "$PLATFORM" = "Nano" ]; then
        AVB_I2C_BUS=( 9 10 )
    fi


    # list contains selected i2c bus
    if [[ ! " ${AVB_I2C_BUS[@]} " =~ " ${I2C_BUS} " ]]; then
        err "Not permitted to write on selcted I2C bus: ${I2C_BUS}!
\tAvailable I2C address: [ ${AVB_I2C_BUS[@]} ]"
    fi

    # list contains selected slave address
    if [[ ! " ${AVB_SLAVE_ADD[@]} " =~ " ${SLAVE_ADD} " ]]; then
        err "Not permitted to write on I2C slave address: ${SLAVE_ADD}!
\tAvailable slave address: [ ${AVB_SLAVE_ADD[@]} ]"
    fi
}

write_eeprom_bin() {
  [[ -f "$BIN_FILE" ]] || err "Binary file not found: $BIN_FILE"

  local size
  size="$(stat -c%s "$BIN_FILE" 2>/dev/null)" || err "Cannot read file size: $BIN_FILE"
  (( size == 256 )) || err "Binary size must be exactly 256 bytes (got $size)."

  echo "Writing 256 bytes to EEPROM offsets 0..255 from '$BIN_FILE'..."

  local addr=0
  while read -r byte; do
    i2cset -y "$I2C_BUS" "$SLAVE_ADD" "$addr" "0x$byte" >/dev/null \
      || err "i2cset failed at EEPROM offset $addr"
    addr=$((addr + 1))
    sleep 0.002
  done < <(hexdump -v -e '1/1 "%02X\n"' "$BIN_FILE")

  echo "Done."
}

parse_arguments $@
validate_i2c_bus_selection
detect_platform
is_write_permitted
write_eeprom_bin

