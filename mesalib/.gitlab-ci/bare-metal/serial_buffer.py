#!/usr/bin/env python3
#
# Copyright © 2020 Google LLC
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

import argparse
from datetime import datetime, timezone
import queue
import serial
import threading
import time


class SerialBuffer:
    def __init__(self, dev, filename, prefix):
        self.filename = filename
        self.dev = dev

        if dev:
            self.f = open(filename, "wb+")
            self.serial = serial.Serial(dev, 115200, timeout=10)
        else:
            self.f = open(filename, "rb")

        self.byte_queue = queue.Queue()
        self.line_queue = queue.Queue()
        self.prefix = prefix
        self.sentinel = object()

        if self.dev:
            self.read_thread = threading.Thread(
                target=self.serial_read_thread_loop, daemon=True)
        else:
            self.read_thread = threading.Thread(
                target=self.serial_file_read_thread_loop, daemon=True)
        self.read_thread.start()

        self.lines_thread = threading.Thread(
            target=self.serial_lines_thread_loop, daemon=True)
        self.lines_thread.start()

    # Thread that just reads the bytes from the serial device to try to keep from
    # buffer overflowing it.
    def serial_read_thread_loop(self):
        greet = "Serial thread reading from %s\n" % self.dev
        self.byte_queue.put(greet.encode())

        while True:
            try:
                self.byte_queue.put(self.serial.read())
            except Exception as err:
                print(self.prefix + str(err))
                self.byte_queue.put(self.sentinel)
                break

    # Thread that just reads the bytes from the file of serial output that some
    # other process is appending to.
    def serial_file_read_thread_loop(self):
        greet = "Serial thread reading from %s\n" % self.filename
        self.byte_queue.put(greet.encode())

        while True:
            line = self.f.readline()
            if line:
                self.byte_queue.put(line)
            else:
                time.sleep(0.1)

    # Thread that processes the stream of bytes to 1) log to stdout, 2) log to
    # file, 3) add to the queue of lines to be read by program logic

    def serial_lines_thread_loop(self):
        line = bytearray()
        while True:
            bytes = self.byte_queue.get(block=True)

            if bytes == self.sentinel:
                self.read_thread.join()
                self.line_queue.put(self.sentinel)
                break

            # Write our data to the output file if we're the ones reading from
            # the serial device
            if self.dev:
                self.f.write(bytes)
                self.f.flush()

            for b in bytes:
                line.append(b)
                if b == b'\n'[0]:
                    line = line.decode(errors="replace")

                    time = datetime.now().strftime('%y-%m-%d %H:%M:%S')
                    print("{endc}{time} {prefix}{line}".format(
                        time=time, prefix=self.prefix, line=line, endc='\033[0m'), flush=True, end='')

                    self.line_queue.put(line)
                    line = bytearray()

    def get_line(self):
        line = self.line_queue.get()
        if line == self.sentinel:
            self.lines_thread.join()
        return line

    def lines(self):
        return iter(self.get_line, self.sentinel)


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('--dev', type=str, help='Serial device')
    parser.add_argument('--file', type=str,
                        help='Filename for serial output', required=True)
    parser.add_argument('--prefix', type=str,
                        help='Prefix for logging serial to stdout', nargs='?')

    args = parser.parse_args()

    ser = SerialBuffer(args.dev, args.file, args.prefix or "")
    for line in ser.lines():
        # We're just using this as a logger, so eat the produced lines and drop
        # them
        pass


if __name__ == '__main__':
    main()
