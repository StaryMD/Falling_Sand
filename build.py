import sys
import os

SOURCE_DIR = 'src'
INCLUDE_DIR = 'include'
BUILD_PATH = 'build'
BINARY_PATH = 'build/bin'
EXECUTABLE_PATH = 'build/bin/runme.exe'

SFML_FLAGS = '-lsfml-graphics -lsfml-window -lsfml-system'
MY_FLAGS = '-Wall -Wextra -std=c++20 -Ofast'

if len(sys.argv) == 2 and sys.argv[1] == 'clean':
	if os.path.exists(BUILD_PATH):
		from shutil import rmtree

		rmtree(BUILD_PATH)
	print('Cleaned build directory')
else:
	from subprocess import check_output as subprocess_check_output
	from glob import iglob
	from json import dump as json_dump, load as json_load
	from re import findall as regex_findall

#region Recursively get all files in ./src and ./include

	cpp_paths_set = [path.replace('\\', '/') for path in iglob(SOURCE_DIR + '/**/*.cpp', recursive=True)]
	hpp_paths_set = [path.replace('\\', '/') for path in iglob(INCLUDE_DIR + '/**/*.hpp', recursive=True)]

#endregion

#region Get dependencies for each file
	def read_direct_dependencies(src_file: str) -> set[str]:
		with open(src_file, 'r') as file:
			return set(regex_findall(r'#include[^"\n]*"([^"]+)"', file.read()))

	def get_cpp_dependencies(cpp_paths_set: list[str], hpp_paths_set: list[str]) -> dict[str, set[str]]:
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

#region Build the necessary object files

	os.makedirs(BUILD_PATH, exist_ok=True)

	for i, cpp_path in enumerate(cpp_path_to_build_list):
		include_list: set[str] = set(os.path.dirname(dep) for dep in cpp_dependencies[cpp_path])
		INCLUDE_FLAGS = ' '.join(['-I ' + header for header in include_list])

		built_obj_path = '{0}/{1}.o'.format(BUILD_PATH, cpp_path)

		os.makedirs(os.path.dirname(built_obj_path), exist_ok=True)

		command = 'g++ {0} -c -o {1} {2} {3} {4}'.format(MY_FLAGS, built_obj_path, cpp_path, SFML_FLAGS, INCLUDE_FLAGS)

		print('%{0:} : {1}'.format(i * 100 // (len(cpp_path_to_build_list) + 1), built_obj_path))

		try:
			subprocess_check_output(command, shell=True)
			
			current_status[cpp_path] = modification_times[cpp_path]
		except:
			pass

#endregion

#region Build the executable
	if len(cpp_path_to_build_list) != 0 or not os.path.exists(EXECUTABLE_PATH):
		os.makedirs(BINARY_PATH, exist_ok=True)

		full_obj_list = ' '.join(['{0}/{1}.o'.format(BUILD_PATH, cpp_path) for cpp_path in cpp_paths_set])

		command = 'g++ {0} -o {1} {2} {3}'.format(MY_FLAGS, EXECUTABLE_PATH, full_obj_list, SFML_FLAGS)

		print('%{0} : {1}'.format(len(cpp_path_to_build_list) * 100 // (len(cpp_path_to_build_list) + 1), EXECUTABLE_PATH))

		try:
			output = subprocess_check_output(command, shell=True)
		except:
			pass

	with open(STATUS_FILE_PATH, 'w') as status_file:
		json_dump(current_status, status_file, indent=2)

	print('Build finished!')

#endregion
