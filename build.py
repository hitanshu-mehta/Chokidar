import subprocess
import os
import sys

os.chdir('..')
dir_path = os.getcwd()
subprocess.check_call(['sudo','pip3','install',dir_path + '/IDS'], cwd=dir_path)
if not os.path.exists(dir_path + '/build'):
	os.mkdir(dir_path + '/build')
subprocess.check_call(['conan','install',dir_path + '/IDS'], cwd=dir_path + '/build')
subprocess.check_call(['cmake', dir_path + '/IDS'], cwd=dir_path + '/build')
subprocess.check_call(['cmake', '--build', '.' ], cwd=dir_path + '/build')
