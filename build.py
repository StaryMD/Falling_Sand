COMPILER: str = 'g++'
SOURCE_DIR: str = 'src'
INCLUDE_DIR: str = 'include'
BUILD_PATH: str = 'build'
BINARY_PATH: str = 'build/bin'
EXECUTABLE_PATH: str = 'build/bin/runme.exe'

MY_FLAGS: str = '-Wall -Wextra -Werror -std=c++17 -Ofast'
INCLUDE_FLAGS: str = '-Iinclude'
OTHER_FLAGS: str = '-lsfml-graphics -lsfml-window -lsfml-system'

RUN_CLANG_FORMAT: bool = True

def clean_build_folder():
	if os.path.exists(BUILD_PATH):
		from shutil import rmtree

		rmtree(BUILD_PATH)
	
	print('Cleaned build directory')


def build(just_export_compile_commands=False):
	from glob import iglob
	from json import dump as json_dump, load as json_load
	from re import findall as regex_findall

	#region Recursively get all files in ./src and ./include

	cpp_paths_set = [path.replace('\\', '/') for path in iglob(SOURCE_DIR + '/**/*.cpp', recursive=True)]
	hpp_paths_set = [path.replace('\\', '/') for path in iglob(INCLUDE_DIR + '/**/*.hpp', recursive=True)]

	#endregion

	#region Get dependencies for each file
	
	def get_cpp_dependencies(cpp_paths_set: list[str], hpp_paths_set: list[str]) -> dict[str, set[str]]:
		def read_direct_dependencies(src_file: str) -> set[str]:
			try:
				with open(src_file, 'r') as file:
					return set(regex_findall(r'#include[^"\n]*"([^"]+)"', file.read()))
			except:
				return set()

		cpp_dependencies: dict[str, set[str]] = {}
		hpp_direct_dependencies: dict[str, set[str]] = {}

		for cpp_path in cpp_paths_set:
			cpp_dependencies[cpp_path] = set(INCLUDE_DIR + '/' + dep for dep in read_direct_dependencies(cpp_path))

		for hpp_path in hpp_paths_set:
			hpp_direct_dependencies[hpp_path] = set(INCLUDE_DIR + '/' + dep for dep in read_direct_dependencies(hpp_path))

		for cpp_path in cpp_paths_set:
			while True:
				new_dependencies: set[str] = set()

				for direct_dependency in cpp_dependencies[cpp_path]:
					direct_dependency_path = direct_dependency

					for indirect_dependency in hpp_direct_dependencies[direct_dependency_path]:
						indirect_dependency_path = indirect_dependency

						if indirect_dependency_path not in cpp_dependencies[cpp_path]:
							new_dependencies.add(indirect_dependency_path)

				if len(new_dependencies) == 0:
					break

				cpp_dependencies[cpp_path].update(new_dependencies)

		return cpp_dependencies

	cpp_dependencies: dict[str, set[str]] = get_cpp_dependencies(cpp_paths_set, hpp_paths_set)

	#endregion

	#region Get modification times of both cpp and hpp files

	modification_times: dict[str, float] = {}

	for cpp_path in cpp_paths_set:
		modification_times[cpp_path] = os.path.getmtime(cpp_path)
	
	for hpp_path in hpp_paths_set:
		modification_times[hpp_path] = os.path.getmtime(hpp_path)

	#endregion

	#region Modify modification times for cpp-s according to their dependencies

	for cpp_path in cpp_paths_set:
		last_modification_time = modification_times[cpp_path]

		for dependency in cpp_dependencies[cpp_path]:
			last_modification_time = max(last_modification_time, modification_times[dependency])
			
		modification_times[cpp_path] = last_modification_time

	#endregion

	#region Decide which files need to be built

	STATUS_FILE_PATH = BUILD_PATH + '/build_status.json'

	cpp_path_to_build_list: list[str] = []
	current_status: dict[str, float] = {}

	if os.path.exists(BUILD_PATH) and os.path.exists(STATUS_FILE_PATH):
		old_status: dict[str, float] = {}

		with open(STATUS_FILE_PATH) as status_file:
			old_status = json_load(status_file)

		for cpp_path in cpp_paths_set:
			cpp_modified = False

			if cpp_path in old_status.keys():
				if old_status[cpp_path] < modification_times[cpp_path]:
					# print(cpp_path, "was modified")
					cpp_modified = True
			else:
				# print(cpp_path, "was added")
				cpp_modified = True

			if cpp_modified:
				cpp_path_to_build_list.append(cpp_path)
			else:
				current_status[cpp_path] = old_status[cpp_path]
	else:
		print("Old config not present, building everything")
		cpp_path_to_build_list = cpp_paths_set

	#endregion

	#region Create the commands for building the files

	COMPILE_COMMANDS_FILE_PATH = BUILD_PATH + '/compile_commands.json'
	CURRENT_DIRECTORY = os.getcwd().replace('\\', '/')

	compile_commands: list[dict[str, str]] = []

	for cpp_path in cpp_paths_set:
		built_obj_path = '{0}/{1}.o'.format(BUILD_PATH, cpp_path)

		command = '{0} {1} -c -o {2} {3} {4} {5}'.format(COMPILER, MY_FLAGS, built_obj_path, cpp_path, OTHER_FLAGS, INCLUDE_FLAGS)

		compile_commands.append({'directory' : CURRENT_DIRECTORY, 'command' : command, 'file' : built_obj_path})
	
	os.makedirs(BUILD_PATH, exist_ok=True)

	with open(COMPILE_COMMANDS_FILE_PATH, 'w') as file:
		json_dump(compile_commands, file, indent=2)

	if just_export_compile_commands:
		print('Exported compile_commands.json')
		exit()

	#endregion

	#region Build the necessary object files

	from subprocess import check_output as subprocess_check_output

	operation_count = len(cpp_path_to_build_list) + 1 + int(RUN_CLANG_FORMAT)

	for i, cpp_path in enumerate(cpp_path_to_build_list):
		built_obj_path = '{0}/{1}.o'.format(BUILD_PATH, cpp_path)

		os.makedirs(os.path.dirname(built_obj_path), exist_ok=True)

		command = '{0} {1} -c -o {2} {3} {4} {5}'.format(COMPILER, MY_FLAGS, built_obj_path, cpp_path, OTHER_FLAGS, INCLUDE_FLAGS)

		print('[{0:3}%] {1}'.format(i * 100 // operation_count, built_obj_path))

		try:
			subprocess_check_output(command, shell=True)
			
			if RUN_CLANG_FORMAT:
				format_cmd = 'clang-format -style=file -i ' + cpp_path
				subprocess_check_output(format_cmd, shell=True)

			current_status[cpp_path] = modification_times[cpp_path]
		except:
			pass

	#endregion

	#region Build the executable

	success = True

	if len(cpp_path_to_build_list) != 0 or not os.path.exists(EXECUTABLE_PATH):
		os.makedirs(BINARY_PATH, exist_ok=True)

		full_obj_list = ' '.join(['{0}/{1}.o'.format(BUILD_PATH, cpp_path) for cpp_path in cpp_paths_set])

		command = '{0} {1} -o {2} {3} {4}'.format(COMPILER, MY_FLAGS, EXECUTABLE_PATH, full_obj_list, OTHER_FLAGS)

		print('[{0:3}%] {1}'.format(len(cpp_path_to_build_list) * 100 // operation_count, EXECUTABLE_PATH))

		try:
			subprocess_check_output(command, shell=True)
		except:
			success = False

	with open(STATUS_FILE_PATH, 'w') as status_file:
		json_dump(current_status, status_file, indent=2)
	
	if success and RUN_CLANG_FORMAT:
		try:
			# command = 'find . -regex \'/(\.cpp|\.c|\.hpp|\.h)$/gm\' -exec clang-format -style=file -i {} \\;'
			# subprocess_check_output(command, shell=True)
			pass
		except:
			success = False

	if success:
		print('[100%] Build finished {0}! - {1}'.format('successfully' if success else 'badly', EXECUTABLE_PATH))
	else:
		print('Build failed')

	#endregion


if __name__ == '__main__':
	import argparse

	argparser = argparse.ArgumentParser()
	argparser.add_argument('-c', '--clean', action='store_true', help='Clean build directory')
	argparser.add_argument('-b', '--build', action='store_true', help='Build the project')
	argparser.add_argument('-r', '--rebuild', action='store_true', help='Clean build directory and build anew')
	argparser.add_argument('-e', '--export_compile_commands', action='store_true', help='Export compile commands without building')

	args = argparser.parse_args()
	
	import os

	if args.clean == True:
		clean_build_folder()
	elif args.build == True:
		build(args.export_compile_commands)
	elif args.rebuild == True:
		clean_build_folder()
		build(args.export_compile_commands)
	elif args.export_compile_commands == True:
		build(True)
	else:
		print('No option given, aborting')
