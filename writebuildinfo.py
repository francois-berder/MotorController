#!/usr/bin/env python3

# Write build information to EEPROM by modifying firmware HEX file.
#
#  offset  |  size  |  description
#    0     |    8   |  release timestamp (big endian)
#    8     |    4   |  XC8 compiler version (ASCII)
#    12    |   20   |  SHA1 hash of last commit of MotorController repo

import argparse
import binascii
import re
import subprocess
import time

RELEASE_TIMESTAMP_OFFSET = 0
RELEASE_TIMESTAMP_SIZE = 8
COMPILER_VERSION_OFFSET = RELEASE_TIMESTAMP_OFFSET + RELEASE_TIMESTAMP_SIZE
COMPILER_VERSION_SIZE = 4
COMMIT_HASH_OFFSET = COMPILER_VERSION_OFFSET + COMPILER_VERSION_SIZE

def get_timestamp():
    return int(time.time())

def get_compiler_version():
    stdoutdata = subprocess.getoutput("xc8 --ver")
    return re.search('Microchip MPLAB XC8 C Compiler V([0-9]\\.[0-9]+)', stdoutdata).group(1)

def get_commit_hash():
    return subprocess.getoutput("git rev-parse HEAD")

def modify_firmware(firmware_filepath, data):
    """Modify firmware to store data in EEPROM

    Each line must have this format:
    :<length in hex>, <address in hex, eg E000, E010, E020><00:byte in hex> <00:checksum in hex>
    No more than 8 bytes per line.
    For more info, see https://en.wikipedia.org/wiki/Intel_HEX
    """
    f = open(firmware_filepath)
    lines = f.readlines()
    f.close()

    index = 0
    address = 0xE000

    while index < len(data):
        nr = 8
        if len(data) - index < 8:
            nr = len(data) - index

        checksum = nr * 2
        checksum += address >> 8
        checksum += address & 0xFF

        l = ':{:02X}{:02X}'.format(nr * 2, address)
        for i in range(nr):
            l += '00'
            l += '{:02X}'.format(data[index + i])
            checksum += data[index + i]

        checksum = ~abs(checksum) + 0b1
        checksum &= 0xFF
        l += '00'
        l += '{:02X}'.format(checksum)
        l += '\n'
        address += nr * 2
        index += nr
        lines.insert(len(lines)-1, l)

    f = open(firmware_filepath, 'w')
    f.writelines(lines)
    f.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("firmware", help="Path to firmware hex file")
    args = parser.parse_args()

    data = [0xFF] * 32

    # Store timestamp in data
    print('timestamp: {}'.format(get_timestamp()))
    for index, b in enumerate(get_timestamp().to_bytes(8, 'big')):
        data[RELEASE_TIMESTAMP_OFFSET + index] = b

    # Store compiler version in data
    print('compiler version: ' + get_compiler_version())
    for index, b in enumerate(str.encode(get_compiler_version())):
        data[COMPILER_VERSION_OFFSET + index] = b

    # Store commit hash in data
    print('commit hash: ' + get_commit_hash())
    for index, b in enumerate(binascii.a2b_hex(get_commit_hash())):
        data[COMMIT_HASH_OFFSET + index] = b

    print('Writing info to firmware...')
    modify_firmware(args.firmware, data)
    print('Done')
