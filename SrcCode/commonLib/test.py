#!/bin/env python
# -*- coding: UTF-8 -*-

import os
import sys
from pexpect import *

from ftplib import FTP


if __name__ == '__main__':
	a = 10
	child = spawn("ssh work@db-testing-ps4005.db01 \"echo abc%s\ > a\"" % a,[], 900)
	response = child.expect([r".*you want to continue connecting.*", r".*password"])

	if response == 1:
		child.sendline("ps-testing!!!")
	elif response == 0:
		child.sendline("yes")
		child.sendline("ps-testing!!!")
	else:
		print "%s error" % responce

	child.sendline("exit")
	child.close()

